#include "app/camera_workflow.h"

#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/DCMIPP/dcmipp.h"
#include "./BSP/HX711/hx711.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LED/led.h"
#include "./BSP/OV2640/ov2640.h"
#include "ai_class_labels.h"
#include <stdio.h>

#define CAMERA_AI_ROI_MIN_SIZE      (96U)
#define CAMERA_INIT_RETRY_DELAY_MS  (100U)
#define HX711_MEDIAN_LEN            (5U)
#define HX711_MEDIAN_INDEX          (HX711_MEDIAN_LEN / 2U)
#define HX711_SCALE_FACTOR          (51000U)
#define CAMERA_FRAME_BUFFER_ADDR    (0x701F4000UL)

static uint32_t g_hx711_buffer[HX711_MEDIAN_LEN];
static uint8_t g_hx711_medlen = 0U;
static uint32_t g_hx711_tare = 0U;

static uint32_t perf_cycle_counter_get(void)
{
    return DWT->CYCCNT;
}

static float perf_cycles_to_ms(uint32_t cycles)
{
    if (SystemCoreClock == 0U)
    {
        return 0.0f;
    }

    return ((float)cycles * 1000.0f) / (float)SystemCoreClock;
}

static void hx711_filter_reset(void)
{
    g_hx711_medlen = 0U;
}

static uint8_t hx711_filter_push(uint32_t sample, uint32_t *median)
{
    uint8_t i = g_hx711_medlen;

    if (g_hx711_medlen >= HX711_MEDIAN_LEN)
    {
        g_hx711_medlen = 0U;
        i = 0U;
    }

    while ((i > 0U) && (g_hx711_buffer[i - 1U] > sample))
    {
        g_hx711_buffer[i] = g_hx711_buffer[i - 1U];
        i--;
    }

    g_hx711_buffer[i] = sample;
    g_hx711_medlen++;

    if (g_hx711_medlen >= HX711_MEDIAN_LEN)
    {
        *median = g_hx711_buffer[HX711_MEDIAN_INDEX];
        g_hx711_medlen = 0U;
        return 1U;
    }

    return 0U;
}

static void hx711_get_tare(void)
{
    uint8_t i;
    uint32_t hx711_dat = 0U;

    hx711_filter_reset();

    for (i = 0U; i < HX711_MEDIAN_LEN; ++i)
    {
        hx711_filter_push(hx711_read(), &hx711_dat);
    }

    g_hx711_tare = hx711_dat / 100U;
}

static uint8_t hx711_update_weight(camera_workflow_state_t *state)
{
    uint8_t i;
    uint32_t median = 0U;
    uint32_t scaled_value;
    uint32_t delta;

    if ((state == NULL) || (hx711_is_ready() == 0U))
    {
        return 1U;
    }

    hx711_filter_reset();

    for (i = 0U; i < HX711_MEDIAN_LEN; ++i)
    {
        if (hx711_filter_push(hx711_read(), &median) == 0U)
        {
            continue;
        }
    }

    scaled_value = median / 100U;
    if (scaled_value > g_hx711_tare)
    {
        delta = scaled_value - g_hx711_tare;
        state->weight_gram = (uint32_t)((float)delta * 0.00001f * HX711_SCALE_FACTOR);
    }
    else
    {
        state->weight_gram = 0U;
    }

    state->weight_valid = 1U;
    return 0U;
}

static void camera_prepare_geometry(camera_workflow_state_t *state)
{
    uint16_t fullheight;
    uint16_t min_dimension;

    state->frame_width = lcddev.width;

    fullheight = (uint16_t)((state->frame_width * 3U) / 4U);
    fullheight &= 0xFFFCU;

    if (fullheight > lcddev.height)
    {
        fullheight = lcddev.height & 0xFFFCU;
    }

    state->frame_height = fullheight;
    state->display_yoffset = (uint16_t)((lcddev.height - state->frame_height) / 2U);

    min_dimension = (state->frame_width < state->frame_height) ? state->frame_width : state->frame_height;
    state->roi_size = (uint16_t)((min_dimension / 2U) & 0xFFFCU);
    if (state->roi_size < CAMERA_AI_ROI_MIN_SIZE)
    {
        state->roi_size = CAMERA_AI_ROI_MIN_SIZE;
    }
    if (state->roi_size > min_dimension)
    {
        state->roi_size = min_dimension & 0xFFFCU;
    }

    state->roi_x = (uint16_t)((state->frame_width - state->roi_size) / 2U);
    state->roi_y = (uint16_t)((state->frame_height - state->roi_size) / 2U);
}

static void camera_show_idle_screen(void)
{
    lcd_clear(WHITE);
    lcd_show_string(20, 40, 320, 24, 24, "Weight Label Demo", RED);
    lcd_show_string(20, 90, 420, 16, 16, "KEY0: capture label + weight", BLUE);
    lcd_show_string(20, 120, 420, 16, 16, "Single snapshot workflow", BLUE);
    lcd_show_string(20, 150, 420, 16, 16, "Center box = classify ROI", BLUE);
    lcd_show_string(20, 200, 320, 16, 16, "Initializing camera...", RED);
}

static void camera_draw_text_overlay(const camera_workflow_state_t *state)
{
    char line1[64];
    char line2[96];
    char line3[96];
    char line4[96];
    uint16_t text_origin_y;

    if (state->display_yoffset > 0U)
    {
        lcd_fill(0, 0, lcddev.width - 1U, state->display_yoffset - 1U, WHITE);
        text_origin_y = 30U;
    }
    else
    {
        lcd_fill(0, 0, lcddev.width - 1U, 62U, WHITE);
        text_origin_y = 12U;
    }

    snprintf(line1, sizeof(line1), "ROI %ux%u  KEY0 = capture", state->roi_size, state->roi_size);

    if (state->last_result_valid != 0U)
    {
        snprintf(line2, sizeof(line2), "Label: %s", ai_class_label_get(state->last_result.top1_index));
    }
    else
    {
        snprintf(line2, sizeof(line2), "Label: N/A");
    }

    if (state->weight_valid != 0U)
    {
        snprintf(line3, sizeof(line3), "Weight: %lu g", (unsigned long)state->weight_gram);
    }
    else
    {
        snprintf(line3, sizeof(line3), "Weight: N/A");
    }

    if (state->recognition_time_valid != 0U)
    {
        snprintf(line4,
                 sizeof(line4),
                 "Recognize: %lu.%lu ms",
                 (unsigned long)(state->recognition_time_ms_x10 / 10U),
                 (unsigned long)(state->recognition_time_ms_x10 % 10U));
    }
    else
    {
        snprintf(line4, sizeof(line4), "Recognize: N/A");
    }

    lcd_show_string(20, text_origin_y, lcddev.width - 40U, 20U, 16U, line1, RED);
    lcd_show_string(20, text_origin_y + 24U, lcddev.width - 40U, 20U, 16U, line2, BLUE);
    lcd_show_string(20, text_origin_y + 48U, lcddev.width - 40U, 20U, 16U, line3, BLUE);
    lcd_show_string(20, text_origin_y + 72U, lcddev.width - 40U, 20U, 16U, line4, BLUE);
}

static void camera_draw_overlay(const camera_workflow_state_t *state)
{
    uint16_t box_x1;
    uint16_t box_y1;
    uint16_t box_x2;
    uint16_t box_y2;

    camera_draw_text_overlay(state);

    box_x1 = state->roi_x;
    box_y1 = (uint16_t)(state->display_yoffset + state->roi_y);
    box_x2 = (uint16_t)(box_x1 + state->roi_size - 1U);
    box_y2 = (uint16_t)(box_y1 + state->roi_size - 1U);

    lcd_draw_rectangle(box_x1, box_y1, box_x2, box_y2, RED);
    lcd_draw_rectangle((uint16_t)(box_x1 + 1U),
                       (uint16_t)(box_y1 + 1U),
                       (uint16_t)(box_x2 - 1U),
                       (uint16_t)(box_y2 - 1U),
                       RED);
}

static void camera_display_snapshot(const camera_workflow_state_t *state)
{
    lcd_color_fill(0,
                   state->display_yoffset,
                   state->frame_width - 1U,
                   state->display_yoffset + state->frame_height - 1U,
                   (uint16_t *)camera_date_buf);
}

int camera_workflow_init(camera_workflow_state_t *state)
{
    if (state == NULL)
    {
        return -1;
    }

    state->last_result.top1_index = 0U;
    state->last_result.top1_probability = 0.0f;
    state->last_result.top1_logit = 0.0f;
    state->last_result_valid = 0U;
    state->inference_counter = 0U;
    state->weight_gram = 0U;
    state->weight_valid = 0U;
    state->recognition_time_ms_x10 = 0U;
    state->recognition_time_valid = 0U;

    printf("[CAM] Initializing KEY and LCD...\r\n");
    key_init();
    lcd_init();
    hx711_init();
    camera_prepare_geometry(state);
    camera_show_idle_screen();

    printf("[CAM] Initializing OV2640 sensor...\r\n");
    while (ov2640_init() != 0U)
    {
        printf("[CAM] OV2640 init failed, retrying...\r\n");
        delay_ms(CAMERA_INIT_RETRY_DELAY_MS);
        LED1_TOGGLE();
    }

    ov2640_rgb565_mode();
    ov2640_focus_init();
    ov2640_light_mode(1U);
    ov2640_color_saturation(2U);
    ov2640_brightness(4U);
    ov2640_contrast(2U);
    ov2640_sharpness(33U);
    ov2640_focus_constant();

    printf("[CAM] Initializing DCMIPP...\r\n");
    dcmipp_init();

    delay_ms(100);
    hx711_get_tare();

    if (ov2640_outsize_set(4U, 0U, state->frame_width, state->frame_height) != 0U)
    {
        printf("[CAM] Failed to configure OV2640 output size\r\n");
        return -1;
    }

    printf("[CAM] Camera ready: RGB565 %ux%u, frame buffer @ 0x%08lX\r\n",
           state->frame_width,
           state->frame_height,
           CAMERA_FRAME_BUFFER_ADDR);
    camera_draw_overlay(state);

    return 0;
}

void camera_workflow_handle_capture(camera_workflow_state_t *state)
{
    uint8_t classify_ok = 0U;
    uint32_t total_start_cycles;
    uint32_t capture_end_cycles;
    uint32_t infer_end_cycles;
    uint32_t weight_end_cycles;
    uint32_t text_end_cycles;
    uint32_t display_end_cycles;

    if (state == NULL)
    {
        return;
    }

    printf("[BOOT] KEY0 pressed: capture and classify\r\n");

    total_start_cycles = perf_cycle_counter_get();
    dcmipp_start();
    capture_end_cycles = perf_cycle_counter_get();

    if (APP_AI_ClassifyRgb565Roi((const uint16_t *)camera_date_buf,
                                 state->frame_width,
                                 state->frame_height,
                                 state->roi_x,
                                 state->roi_y,
                                 state->roi_size,
                                 state->roi_size,
                                 "live roi",
                                 &state->last_result) == 0)
    {
        state->last_result_valid = 1U;
        state->inference_counter++;
        classify_ok = 1U;
    }
    else
    {
        state->last_result_valid = 0U;
        state->recognition_time_valid = 0U;
    }
    infer_end_cycles = perf_cycle_counter_get();

    if (classify_ok != 0U)
    {
        state->recognition_time_ms_x10 =
            (uint32_t)(perf_cycles_to_ms(infer_end_cycles - total_start_cycles) * 10.0f + 0.5f);
        state->recognition_time_valid = 1U;
    }

    hx711_update_weight(state);
    weight_end_cycles = perf_cycle_counter_get();

    camera_draw_text_overlay(state);
    text_end_cycles = perf_cycle_counter_get();

    camera_display_snapshot(state);
    camera_draw_overlay(state);
    display_end_cycles = perf_cycle_counter_get();

    printf("[PERF] capture=%.2f ms, ai=%.2f ms, weight=%.2f ms, text=%.2f ms, lcd=%.2f ms, total=%.2f ms\r\n",
           perf_cycles_to_ms(capture_end_cycles - total_start_cycles),
           perf_cycles_to_ms(infer_end_cycles - capture_end_cycles),
           perf_cycles_to_ms(weight_end_cycles - infer_end_cycles),
           perf_cycles_to_ms(text_end_cycles - weight_end_cycles),
           perf_cycles_to_ms(display_end_cycles - text_end_cycles),
           perf_cycles_to_ms(display_end_cycles - total_start_cycles));
}
