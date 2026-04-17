/**
  ******************************************************************************
  * @file    app_x-cube-ai.c
  * @brief   X-CUBE-AI inference glue for captured RGB565 camera frames
  ******************************************************************************
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "ai_class_labels.h"
#include "main.h"
#include "network.h"
#include "network_data.h"

#define APP_AI_INPUT_RESIZE_SHORT  (110U)
#define APP_AI_INPUT_IMAGE_SIZE    (96U)
#define APP_AI_TOP_K               (5U)

#define APP_AI_INPUT_SCALE       (0.01865844801068306f)
#define APP_AI_INPUT_ZERO_POINT  (-14)
#define APP_AI_OUTPUT_SCALE      (0.038118213415145874f)
#define APP_AI_OUTPUT_ZERO_POINT (-45)

static const float g_input_mean[3] = {0.485f, 0.456f, 0.406f};
static const float g_input_std[3] = {0.229f, 0.224f, 0.225f};
static const char *g_default_sample_name = "camera snapshot";

AI_ALIGNED(32) static uint8_t g_activation_buffer[AI_NETWORK_DATA_ACTIVATION_1_SIZE];

#if !defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) static ai_i8 g_input_buffer[AI_NETWORK_IN_1_SIZE_BYTES];
static ai_i8 *g_input_ptrs[AI_NETWORK_IN_NUM] = {
  g_input_buffer
};
#else
static ai_i8 *g_input_ptrs[AI_NETWORK_IN_NUM] = {
  NULL
};
#endif

#if !defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) static ai_i8 g_output_buffer[AI_NETWORK_OUT_1_SIZE_BYTES];
static ai_i8 *g_output_ptrs[AI_NETWORK_OUT_NUM] = {
  g_output_buffer
};
#else
static ai_i8 *g_output_ptrs[AI_NETWORK_OUT_NUM] = {
  NULL
};
#endif

static ai_handle g_activations[] = {
  g_activation_buffer
};

static ai_handle g_network = AI_HANDLE_NULL;
static ai_buffer *g_ai_input = NULL;
static ai_buffer *g_ai_output = NULL;
static uintptr_t g_runtime_weights_addr = 0U;

static void ai_cycle_counter_enable(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t ai_cycle_counter_get(void)
{
    return DWT->CYCCNT;
}

static float ai_cycles_to_ms(uint32_t cycles)
{
    const uint32_t core_clock_hz = SystemCoreClock;
    if (core_clock_hz == 0U)
    {
        return 0.0f;
    }

    return ((float)cycles * 1000.0f) / (float)core_clock_hz;
}

static void ai_log_err(const ai_error err, const char *stage)
{
    printf("[AI] Error at %s: type=0x%02x code=0x%02x\r\n",
           (stage != NULL) ? stage : "unknown",
           err.type,
           err.code);

    while (1)
    {
    }
}

static uint32_t ai_topk_count(void)
{
    return (AI_NETWORK_OUT_1_SIZE < APP_AI_TOP_K) ? AI_NETWORK_OUT_1_SIZE : APP_AI_TOP_K;
}

static void ai_find_topk(const float *values, uint32_t count, uint32_t *indices, uint32_t topk)
{
    uint32_t rank;
    uint32_t idx;

    for (rank = 0U; rank < topk; ++rank)
    {
        indices[rank] = UINT32_MAX;
    }

    for (idx = 0U; idx < count; ++idx)
    {
        for (rank = 0U; rank < topk; ++rank)
        {
            if ((indices[rank] == UINT32_MAX) || (values[idx] > values[indices[rank]]))
            {
                uint32_t move;
                for (move = topk - 1U; move > rank; --move)
                {
                    indices[move] = indices[move - 1U];
                }
                indices[rank] = idx;
                break;
            }
        }
    }
}

static int8_t ai_quantize_input_value(float value)
{
    const float scaled = value / APP_AI_INPUT_SCALE;
    int32_t quantized = (int32_t)lroundf(scaled) + APP_AI_INPUT_ZERO_POINT;

    if (quantized > 127)
    {
        quantized = 127;
    }
    else if (quantized < -128)
    {
        quantized = -128;
    }

    return (int8_t)quantized;
}

static void ai_rgb565_unpack(uint16_t pixel, uint8_t *red, uint8_t *green, uint8_t *blue)
{
    const uint8_t r5 = (uint8_t)((pixel >> 11) & 0x1FU);
    const uint8_t g6 = (uint8_t)((pixel >> 5) & 0x3FU);
    const uint8_t b5 = (uint8_t)(pixel & 0x1FU);

    *red = (uint8_t)((r5 * 255U + 15U) / 31U);
    *green = (uint8_t)((g6 * 255U + 31U) / 63U);
    *blue = (uint8_t)((b5 * 255U + 15U) / 31U);
}

static uint16_t ai_rgb565_sample_nearest_roi(const uint16_t *frame,
                                             uint16_t frame_width,
                                             uint16_t frame_height,
                                             uint16_t roi_x,
                                             uint16_t roi_y,
                                             uint16_t roi_width,
                                             uint16_t roi_height,
                                             uint32_t x,
                                             uint32_t y)
{
    const uint32_t roi_end_x = (uint32_t)roi_x + (uint32_t)roi_width;
    const uint32_t roi_end_y = (uint32_t)roi_y + (uint32_t)roi_height;

    if (x >= roi_width)
    {
        x = (uint32_t)roi_width - 1U;
    }
    if (y >= roi_height)
    {
        y = (uint32_t)roi_height - 1U;
    }

    x += roi_x;
    y += roi_y;

    if (x >= roi_end_x)
    {
        x = roi_end_x - 1U;
    }
    if (y >= roi_end_y)
    {
        y = roi_end_y - 1U;
    }
    if (x >= frame_width)
    {
        x = (uint32_t)frame_width - 1U;
    }
    if (y >= frame_height)
    {
        y = (uint32_t)frame_height - 1U;
    }

    return frame[(y * (uint32_t)frame_width) + x];
}

static int ai_prepare_input_from_rgb565_roi(const uint16_t *frame,
                                            uint16_t frame_width,
                                            uint16_t frame_height,
                                            uint16_t roi_x,
                                            uint16_t roi_y,
                                            uint16_t roi_width,
                                            uint16_t roi_height,
                                            const char *sample_name,
                                            uint8_t verbose)
{
    const uint32_t plane_size = APP_AI_INPUT_IMAGE_SIZE * APP_AI_INPUT_IMAGE_SIZE;
    const float short_side = (roi_width < roi_height) ? (float)roi_width : (float)roi_height;
    const float scale = (float)APP_AI_INPUT_RESIZE_SHORT / short_side;
    const float scaled_width = ((float)roi_width) * scale;
    const float scaled_height = ((float)roi_height) * scale;
    const float crop_left = (scaled_width - (float)APP_AI_INPUT_IMAGE_SIZE) * 0.5f;
    const float crop_top = (scaled_height - (float)APP_AI_INPUT_IMAGE_SIZE) * 0.5f;
    uint32_t out_y;

    if ((frame == NULL) ||
        (frame_width == 0U) ||
        (frame_height == 0U) ||
        (roi_width == 0U) ||
        (roi_height == 0U) ||
        (g_input_ptrs[0] == NULL))
    {
        return -1;
    }

    if ((((uint32_t)roi_x + (uint32_t)roi_width) > frame_width) ||
        (((uint32_t)roi_y + (uint32_t)roi_height) > frame_height))
    {
        return -1;
    }

    if (verbose != 0U)
    {
        printf("[AI] Preprocessing %s ROI (%u,%u %ux%u RGB565 -> 96x96 int8)...\r\n",
               sample_name,
               roi_x,
               roi_y,
               roi_width,
               roi_height);
    }

    for (out_y = 0U; out_y < APP_AI_INPUT_IMAGE_SIZE; ++out_y)
    {
        const float scaled_y = crop_top + (float)out_y + 0.5f;
        const int32_t src_y = (int32_t)lroundf((scaled_y / scale) - 0.5f);
        const uint32_t sample_y = (src_y < 0) ? 0U : (uint32_t)src_y;
        uint32_t out_x;

        for (out_x = 0U; out_x < APP_AI_INPUT_IMAGE_SIZE; ++out_x)
        {
            const float scaled_x = crop_left + (float)out_x + 0.5f;
            const int32_t src_x = (int32_t)lroundf((scaled_x / scale) - 0.5f);
            const uint32_t sample_x = (src_x < 0) ? 0U : (uint32_t)src_x;
            const uint16_t pixel = ai_rgb565_sample_nearest_roi(frame,
                                                                frame_width,
                                                                frame_height,
                                                                roi_x,
                                                                roi_y,
                                                                roi_width,
                                                                roi_height,
                                                                sample_x,
                                                                sample_y);
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            float normalized;
            const uint32_t dst_index = (out_y * APP_AI_INPUT_IMAGE_SIZE) + out_x;

            ai_rgb565_unpack(pixel, &red, &green, &blue);

            normalized = (((float)red / 255.0f) - g_input_mean[0]) / g_input_std[0];
            g_input_ptrs[0][dst_index] = ai_quantize_input_value(normalized);

            normalized = (((float)green / 255.0f) - g_input_mean[1]) / g_input_std[1];
            g_input_ptrs[0][plane_size + dst_index] = ai_quantize_input_value(normalized);

            normalized = (((float)blue / 255.0f) - g_input_mean[2]) / g_input_std[2];
            g_input_ptrs[0][(2U * plane_size) + dst_index] = ai_quantize_input_value(normalized);
        }
    }

    if (verbose != 0U)
    {
        printf("[AI] Input tensor ready\r\n");
    }
    return 0;
}

static int ai_bootstrap(void)
{
    ai_error err;

    err = ai_network_create_and_init(&g_network, g_activations, NULL);
    if (err.type != AI_ERROR_NONE)
    {
        ai_log_err(err, "ai_network_create_and_init");
        return -1;
    }

    g_ai_input = ai_network_inputs_get(g_network, NULL);
    g_ai_output = ai_network_outputs_get(g_network, NULL);

#if defined(AI_NETWORK_INPUTS_IN_ACTIVATIONS)
    for (uint32_t idx = 0U; idx < AI_NETWORK_IN_NUM; ++idx)
    {
        g_input_ptrs[idx] = g_ai_input[idx].data;
    }
#else
    for (uint32_t idx = 0U; idx < AI_NETWORK_IN_NUM; ++idx)
    {
        g_ai_input[idx].data = g_input_ptrs[idx];
    }
#endif

#if defined(AI_NETWORK_OUTPUTS_IN_ACTIVATIONS)
    for (uint32_t idx = 0U; idx < AI_NETWORK_OUT_NUM; ++idx)
    {
        g_output_ptrs[idx] = g_ai_output[idx].data;
    }
#else
    for (uint32_t idx = 0U; idx < AI_NETWORK_OUT_NUM; ++idx)
    {
        g_ai_output[idx].data = g_output_ptrs[idx];
    }
#endif

    return 0;
}

static int ai_run(void)
{
    const ai_i32 batch = ai_network_run(g_network, g_ai_input, g_ai_output);
    if (batch != 1)
    {
        ai_log_err(ai_network_get_error(g_network), "ai_network_run");
        return -1;
    }

    return 0;
}

static int ai_post_process(const char *sample_name, app_ai_result_t *out_result, uint8_t verbose)
{
    float logits[AI_NETWORK_OUT_1_SIZE];
    uint32_t top_indices[APP_AI_TOP_K];
    const uint32_t topk = ai_topk_count();
    float max_logit;
    float exp_sum = 0.0f;
    uint32_t idx;

    if (g_output_ptrs[0] == NULL)
    {
        return -1;
    }

    for (idx = 0U; idx < AI_NETWORK_OUT_1_SIZE; ++idx)
    {
        logits[idx] = ((float)((int32_t)g_output_ptrs[0][idx] - APP_AI_OUTPUT_ZERO_POINT)) *
                      APP_AI_OUTPUT_SCALE;
    }

    max_logit = logits[0];
    for (idx = 1U; idx < AI_NETWORK_OUT_1_SIZE; ++idx)
    {
        if (logits[idx] > max_logit)
        {
            max_logit = logits[idx];
        }
    }

    for (idx = 0U; idx < AI_NETWORK_OUT_1_SIZE; ++idx)
    {
        exp_sum += expf(logits[idx] - max_logit);
    }

    ai_find_topk(logits, AI_NETWORK_OUT_1_SIZE, top_indices, topk);

    if (out_result != NULL)
    {
        const uint32_t class_index = top_indices[0];
        out_result->top1_index = class_index;
        out_result->top1_probability = expf(logits[class_index] - max_logit) / exp_sum;
        out_result->top1_logit = logits[class_index];
    }

    if (verbose != 0U)
    {
        printf("[AI] Post processing output...\r\n\r\n");
        printf("[AI] Inference result for %s\r\n", sample_name);
        for (idx = 0U; idx < topk; ++idx)
        {
            const uint32_t class_index = top_indices[idx];
            const float probability = expf(logits[class_index] - max_logit) / exp_sum;

            printf("[AI] TOP%" PRIu32 ": %s (index=%" PRIu32 ", prob=%.4f, logit=%.4f)\r\n",
                   idx + 1U,
                   ai_class_label_get(class_index),
                   class_index,
                   probability,
                   logits[class_index]);
        }

        printf("[AI] Inference complete\r\n");
    }
    return 0;
}

void MX_X_CUBE_AI_Init(void)
{
    ai_cycle_counter_enable();
    g_runtime_weights_addr = (uintptr_t)ai_network_data_weights_get();

    printf("[AI] Bootstrapping network activations from AXI SRAM @ 0x%08" PRIXPTR " (%u bytes)\r\n",
           (uintptr_t)g_activation_buffer,
           (unsigned)AI_NETWORK_DATA_ACTIVATION_1_SIZE);
    printf("[AI] Runtime weights @ 0x%08" PRIXPTR " (%u bytes)\r\n",
           g_runtime_weights_addr,
           (unsigned)AI_NETWORK_DATA_WEIGHTS_SIZE);

    if (ai_bootstrap() != 0)
    {
        ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
        ai_log_err(err, "MX_X_CUBE_AI_Init");
    }

    printf("[AI] Init done. Ready for live ROI inference.\r\n");
}

void MX_X_CUBE_AI_Process(void)
{
    /* Inference is explicitly triggered from the camera loop in main(). */
}

int APP_AI_RunInferenceOnRgb565Frame(const uint16_t *rgb565_frame,
                                     uint16_t frame_width,
                                     uint16_t frame_height,
                                     const char *sample_name)
{
    return APP_AI_ClassifyRgb565Roi(rgb565_frame,
                                    frame_width,
                                    frame_height,
                                    0U,
                                    0U,
                                    frame_width,
                                    frame_height,
                                    sample_name,
                                    NULL);
}

int APP_AI_ClassifyRgb565Roi(const uint16_t *rgb565_frame,
                             uint16_t frame_width,
                             uint16_t frame_height,
                             uint16_t roi_x,
                             uint16_t roi_y,
                             uint16_t roi_width,
                             uint16_t roi_height,
                             const char *sample_name,
                             app_ai_result_t *out_result)
{
    uint32_t run_cycles_start;
    uint32_t run_cycles_end;
    uint32_t e2e_cycles_start;
    uint32_t e2e_cycles_end;
    uint32_t run_cycles;
    uint32_t e2e_cycles;
    int res;
    const char *effective_sample_name = sample_name;
    const uint8_t verbose = (out_result == NULL) ? 1U : 0U;

    if ((g_network == AI_HANDLE_NULL) || (rgb565_frame == NULL))
    {
        return -1;
    }

    if ((effective_sample_name == NULL) || (effective_sample_name[0] == '\0'))
    {
        effective_sample_name = g_default_sample_name;
    }

    if (verbose != 0U)
    {
        printf("[AI] Inferencing %s...\r\n", effective_sample_name);
    }

    e2e_cycles_start = ai_cycle_counter_get();
    res = ai_prepare_input_from_rgb565_roi(rgb565_frame,
                                           frame_width,
                                           frame_height,
                                           roi_x,
                                           roi_y,
                                           roi_width,
                                           roi_height,
                                           effective_sample_name,
                                           verbose);
    if (res == 0)
    {
        if (verbose != 0U)
        {
            printf("[AI] Running network...\r\n");
        }
        run_cycles_start = ai_cycle_counter_get();
        res = ai_run();
        run_cycles_end = ai_cycle_counter_get();
    }
    else
    {
        run_cycles_start = 0U;
        run_cycles_end = 0U;
    }

    if (res == 0)
    {
        run_cycles = run_cycles_end - run_cycles_start;
        if (verbose != 0U)
        {
            printf("[AI] Network runtime: %" PRIu32 " cycles, %.3f ms@coreclk\r\n",
                   run_cycles,
                   ai_cycles_to_ms(run_cycles));
        }
        res = ai_post_process(effective_sample_name, out_result, verbose);
    }

    if (res == 0)
    {
        e2e_cycles_end = ai_cycle_counter_get();
        e2e_cycles = e2e_cycles_end - e2e_cycles_start;
        if (verbose != 0U)
        {
            printf("[AI] End-to-end runtime: %" PRIu32 " cycles, %.3f ms@coreclk\r\n",
                   e2e_cycles,
                   ai_cycles_to_ms(e2e_cycles));
        }
    }

    if (res != 0)
    {
        ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
        ai_log_err(err, "APP_AI_ClassifyRgb565Roi");
    }

    return res;
}

#ifdef __cplusplus
}
#endif
