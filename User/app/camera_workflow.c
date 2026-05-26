#include "app/camera_workflow.h"

#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/DCMIPP/dcmipp.h"
#include "./BSP/HX711/hx711.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LED/led.h"
#include "./BSP/OV2640/ov2640.h"
#include "ai/ai_class_labels.h"
#include "app/product_catalog.h"
#include <stdio.h>
#include <string.h>

#define CAMERA_AI_ROI_MIN_SIZE      (96U)
#define CAMERA_INIT_RETRY_DELAY_MS  (100U)
#define CAMERA_EMPTY_THRESHOLD_G    (30U)
#define CAMERA_STABLE_DELTA_G       (30U)
#define CAMERA_STABLE_HOLD_MS       (300U)
#define CAMERA_FAST_STABLE_MIN_MS   (180U)
#define CAMERA_FAST_STABLE_COUNT    (2U)
#define CAMERA_UNSTABLE_RESET_G     (60U)
#define CAMERA_WEIGHT_SAMPLE_MS     (60U)
#define CAMERA_IMAGE_REFRESH_DELAY_MS (50U)
#define HX711_MEDIAN_LEN            (3U)
#define HX711_MEDIAN_INDEX          (HX711_MEDIAN_LEN / 2U)
#define HX711_SCALE_FACTOR          (51000U)
#define CAMERA_FRAME_BUFFER_ADDR    (0x701F4000UL)

static uint32_t g_hx711_buffer[HX711_MEDIAN_LEN];
static uint8_t g_hx711_medlen = 0U;
static uint32_t g_hx711_tare = 0U;

static void camera_set_status(camera_workflow_state_t *state, camera_trigger_state_t next_state, const char *status_text);
static void camera_update_text_only(const camera_workflow_state_t *state);
static uint8_t hx711_measure_weight_gram(uint32_t *weight_gram);
static void camera_log_cart_summary(const cart_context_t *cart);
static void camera_push_notice(camera_workflow_state_t *state, camera_notice_type_t notice_type, const char *notice_text);

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

static uint8_t hx711_measure_weight_gram(uint32_t *weight_gram)
{
    uint8_t i;
    uint32_t median = 0U;
    uint32_t scaled_value;
    uint32_t delta;

    if ((weight_gram == NULL) || (hx711_is_ready() == 0U))
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
        *weight_gram = (uint32_t)((float)delta * 0.00001f * HX711_SCALE_FACTOR);
    }
    else
    {
        *weight_gram = 0U;
    }

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
    lcd_show_string(20, 90, 420, 16, 16, "Auto trigger after stable weight", BLUE);
    lcd_show_string(20, 120, 420, 16, 16, "Place one item and wait", BLUE);
    lcd_show_string(20, 150, 420, 16, 16, "Remove item before next round", BLUE);
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

    snprintf(line1, sizeof(line1), "State: %s", state->status_text);

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
        snprintf(line3,
                 sizeof(line3),
                 "Weight: %lu g  Stable: %lu g  ROI %ux%u",
                 (unsigned long)state->weight_gram,
                 (unsigned long)state->stable_weight_gram,
                 state->roi_size,
                 state->roi_size);
    }
    else
    {
        snprintf(line3, sizeof(line3), "Weight: N/A");
    }

    if (state->recognition_time_valid != 0U)
    {
        snprintf(line4,
                 sizeof(line4),
                 "Stable->Show: %lu.%lu ms  Total: %lu.%lu ms",
                 (unsigned long)(state->recognition_time_ms_x10 / 10U),
                 (unsigned long)(state->recognition_time_ms_x10 % 10U),
                 (unsigned long)(state->total_flow_time_ms_x10 / 10U),
                 (unsigned long)(state->total_flow_time_ms_x10 % 10U));
    }
    else
    {
        snprintf(line4, sizeof(line4), "Stable->Show: N/A  Total: N/A");
    }

    lcd_show_string(20, text_origin_y, lcddev.width - 40U, 20U, 16U, line1, RED);
    lcd_show_string(20, text_origin_y + 24U, lcddev.width - 40U, 20U, 16U, line2, BLUE);
    lcd_show_string(20, text_origin_y + 48U, lcddev.width - 40U, 20U, 16U, line3, BLUE);
    lcd_show_string(20, text_origin_y + 72U, lcddev.width - 40U, 20U, 16U, line4, BLUE);
}

static void camera_update_text_only(const camera_workflow_state_t *state)
{
    (void)state;
}

static void camera_log_cart_summary(const cart_context_t *cart)
{
    uint32_t idx;
    uint32_t total_price_yuan = 0U;
    uint32_t total_price_cent = 0U;

    if (cart == 0)
    {
        return;
    }

    total_price_yuan = cart->total_price_cent / 100U;
    total_price_cent = cart->total_price_cent % 100U;
    printf("[CART] -------------------------------------------------------------\r\n");
    printf("[CART] %-12s %-12s %-12s %-12s\r\n", "Name", "Weight/Qty", "UnitPrice", "Total");
    printf("[CART] -------------------------------------------------------------\r\n");

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        const cart_item_t *item = &cart->items[idx];
        char weight_or_qty[16];
        char unit_price[16];
        char total_price[16];

        if (item->line_valid == 0U)
        {
            continue;
        }

        if (item->pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
        {
            snprintf(weight_or_qty,
                     sizeof(weight_or_qty),
                     "%lu.%03lukg",
                     (unsigned long)(item->total_weight_g / 1000U),
                     (unsigned long)(item->total_weight_g % 1000U));
            snprintf(unit_price,
                     sizeof(unit_price),
                     "%lu.%02lu/kg",
                     (unsigned long)(item->unit_price_cent / 100U),
                     (unsigned long)(item->unit_price_cent % 100U));
        }
        else
        {
            snprintf(weight_or_qty, sizeof(weight_or_qty), "%lu pcs", (unsigned long)item->total_count);
            snprintf(unit_price,
                     sizeof(unit_price),
                     "%lu.%02lu/pc",
                     (unsigned long)(item->unit_price_cent / 100U),
                     (unsigned long)(item->unit_price_cent % 100U));
        }

        snprintf(total_price,
                 sizeof(total_price),
                 "%lu.%02lu",
                 (unsigned long)(item->subtotal_cent / 100U),
                 (unsigned long)(item->subtotal_cent % 100U));

        printf("[CART] %-12s %-12s %-12s %-12s\r\n",
               item->product_name,
               weight_or_qty,
               unit_price,
               total_price);
    }

    printf("[CART] -------------------------------------------------------------\r\n");
    printf("[CART] CartTotal: %lu.%02lu yuan\r\n",
           (unsigned long)total_price_yuan,
           (unsigned long)total_price_cent);
}

static void camera_set_status(camera_workflow_state_t *state, camera_trigger_state_t next_state, const char *status_text)
{
    if ((state == NULL) || (status_text == NULL))
    {
        return;
    }

    state->trigger_state = next_state;
    snprintf(state->status_text, sizeof(state->status_text), "%s", status_text);
    printf("[FLOW] -> %s\r\n", state->status_text);
    camera_update_text_only(state);
}

static void camera_push_notice(camera_workflow_state_t *state, camera_notice_type_t notice_type, const char *notice_text)
{
    if ((state == NULL) || (notice_text == NULL))
    {
        return;
    }

    state->notice_pending = 1U;
    state->notice_type = notice_type;
    snprintf(state->notice_text, sizeof(state->notice_text), "%s", notice_text);
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
    state->capture_counter = 0U;
    state->inference_counter = 0U;
    state->weight_gram = 0U;
    state->weight_valid = 0U;
    state->recognition_time_ms_x10 = 0U;
    state->recognition_time_valid = 0U;
    state->total_flow_time_ms_x10 = 0U;
    state->total_flow_time_valid = 0U;
    state->stable_weight_gram = 0U;
    state->last_sample_weight_gram = 0U;
    state->stable_start_tick_ms = 0U;
    state->stable_sample_count = 0U;
    state->last_weight_sample_tick_ms = 0U;
    state->flow_start_cycles = 0U;
    state->pending_image_refresh_tick_ms = 0U;
    state->pending_image_refresh = 0U;
    state->notice_pending = 0U;
    state->notice_type = CAMERA_NOTICE_NONE;
    cart_service_init(&state->cart);
    memset(&state->last_cart_add, 0, sizeof(state->last_cart_add));
    state->trigger_state = CAMERA_TRIGGER_EMPTY;
    memset(state->status_text, 0, sizeof(state->status_text));
    memset(state->notice_text, 0, sizeof(state->notice_text));

    printf("[CAM] Initializing LCD and weight sensor...\r\n");
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
    ov2640_hmirror(0U);
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
    camera_set_status(state, CAMERA_TRIGGER_EMPTY, "请放置商品");

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
    float wait_stable_ms = 0.0f;
    float stable_to_text_ms;
    float total_to_text_ms;
    const char *ai_label_name;
    const product_info_t *product;
    cart_add_status_t cart_status = CART_ADD_ERR_UNKNOWN_PRODUCT;

    if (state == NULL)
    {
        return;
    }

    printf("[FLOW] Stable weight confirmed: capture and classify\r\n");

    total_start_cycles = perf_cycle_counter_get();
    if (state->flow_start_cycles != 0U)
    {
        wait_stable_ms = perf_cycles_to_ms(total_start_cycles - state->flow_start_cycles);
    }
    printf("[FLOW] Capture start\r\n");
    if (dcmipp_start() != 0)
    {
        state->last_result_valid = 0U;
        state->recognition_time_valid = 0U;
        camera_push_notice(state, CAMERA_NOTICE_RECOGNIZE_FAIL, "相机拍照超时，请重新放置");
        camera_set_status(state, CAMERA_TRIGGER_WAIT_REMOVE, "识别失败，请取走重试");
        return;
    }
    printf("[FLOW] Capture done\r\n");
    capture_end_cycles = perf_cycle_counter_get();
    state->capture_counter++;
    memset(&state->last_cart_add, 0, sizeof(state->last_cart_add));

    printf("[FLOW] AI classify start\r\n");
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
        printf("[FLOW] AI classify done: top1=%s\r\n", ai_class_label_get(state->last_result.top1_index));
    }
    else
    {
        state->last_result_valid = 0U;
        state->recognition_time_valid = 0U;
        camera_push_notice(state, CAMERA_NOTICE_RECOGNIZE_FAIL, "识别失败，请移走商品后重试");
    }
    infer_end_cycles = perf_cycle_counter_get();

    state->weight_gram = state->stable_weight_gram;
    state->weight_valid = 1U;

    ai_label_name = ai_class_label_get(state->last_result.top1_index);
    if (classify_ok != 0U)
    {
        product = product_catalog_find_by_ai_label(ai_label_name);
        if (product != 0)
        {
            cart_status = cart_service_add_item(&state->cart,
                                                product,
                                                state->stable_weight_gram,
                                                &state->last_cart_add);
            if (cart_status == CART_ADD_OK)
            {
                printf("[CART] Added %s, measured=%lu g, counted=%lu, subtotal=%lu.%02lu yuan\r\n",
                       state->last_cart_add.product_name,
                       (unsigned long)state->last_cart_add.measured_weight_g,
                       (unsigned long)state->last_cart_add.counted_units,
                       (unsigned long)(state->last_cart_add.subtotal_cent / 100U),
                       (unsigned long)(state->last_cart_add.subtotal_cent % 100U));
                camera_log_cart_summary(&state->cart);
            }
            else
            {
                printf("[CART] Add failed for %s, status=%d, measured=%lu g\r\n",
                       ai_label_name,
                       (int)cart_status,
                       (unsigned long)state->stable_weight_gram);
                camera_push_notice(state, CAMERA_NOTICE_RECOGNIZE_FAIL, "商品重量异常，请重新放置");
            }
        }
        else
        {
            printf("[CART] Unknown AI label: %s\r\n", ai_label_name);
            cart_status = CART_ADD_ERR_UNKNOWN_PRODUCT;
            camera_push_notice(state, CAMERA_NOTICE_RECOGNIZE_FAIL, "识别到未知商品，请重试");
        }
    }

    weight_end_cycles = perf_cycle_counter_get();

    text_end_cycles = perf_cycle_counter_get();
    state->pending_image_refresh = 0U;
    state->pending_image_refresh_tick_ms = 0U;
    stable_to_text_ms = perf_cycles_to_ms(text_end_cycles - total_start_cycles);
    total_to_text_ms = wait_stable_ms + stable_to_text_ms;

    if (classify_ok != 0U)
    {
        /* Measure the user-visible latency until the recognition text result is
           already rendered to the screen. */
        state->recognition_time_ms_x10 =
            (uint32_t)(stable_to_text_ms * 10.0f + 0.5f);
        state->recognition_time_valid = 1U;

        if (state->flow_start_cycles != 0U)
        {
            /* Measure the perceived end-to-end latency from initial item
               detection until the recognition text is shown on the screen. */
            state->total_flow_time_ms_x10 =
                (uint32_t)(total_to_text_ms * 10.0f + 0.5f);
            state->total_flow_time_valid = 1U;
        }
    }

    printf("[PERF] wait_stable=%.2f ms + stable_to_text=%.2f ms = total_to_text=%.2f ms | "
           "capture=%.2f ms, ai=%.2f ms, weight=%.2f ms, text=%.2f ms, lcd_deferred=%u ms\r\n",
           wait_stable_ms,
           stable_to_text_ms,
           total_to_text_ms,
           perf_cycles_to_ms(capture_end_cycles - total_start_cycles),
           perf_cycles_to_ms(infer_end_cycles - capture_end_cycles),
           perf_cycles_to_ms(weight_end_cycles - infer_end_cycles),
           perf_cycles_to_ms(text_end_cycles - weight_end_cycles),
           (unsigned)CAMERA_IMAGE_REFRESH_DELAY_MS);
}

void camera_workflow_process(camera_workflow_state_t *state)
{
    uint32_t now_tick_ms;
    uint32_t measured_weight_gram;
    uint32_t weight_delta;

    if (state == NULL)
    {
        return;
    }

    now_tick_ms = HAL_GetTick();
    if ((now_tick_ms - state->last_weight_sample_tick_ms) < CAMERA_WEIGHT_SAMPLE_MS)
    {
        return;
    }
    state->last_weight_sample_tick_ms = now_tick_ms;

    if (hx711_measure_weight_gram(&measured_weight_gram) != 0U)
    {
        return;
    }

    state->last_sample_weight_gram = state->weight_gram;
    state->weight_gram = measured_weight_gram;
    state->weight_valid = 1U;

    if (state->weight_gram >= state->last_sample_weight_gram)
    {
        weight_delta = state->weight_gram - state->last_sample_weight_gram;
    }
    else
    {
        weight_delta = state->last_sample_weight_gram - state->weight_gram;
    }

    switch (state->trigger_state)
    {
        case CAMERA_TRIGGER_EMPTY:
            state->stable_weight_gram = 0U;
            if (state->weight_gram > CAMERA_EMPTY_THRESHOLD_G)
            {
                printf("[FLOW] Item detected: %lu g\r\n", (unsigned long)state->weight_gram);
                state->flow_start_cycles = perf_cycle_counter_get();
                state->recognition_time_valid = 0U;
                state->total_flow_time_valid = 0U;
                state->pending_image_refresh = 0U;
                state->stable_sample_count = 0U;
                memset(&state->last_cart_add, 0, sizeof(state->last_cart_add));
                state->last_result_valid = 0U;
                state->recognition_time_valid = 0U;
                state->total_flow_time_valid = 0U;
                camera_set_status(state, CAMERA_TRIGGER_DETECT_PLACEMENT, "检测到物品");
            }
            break;

        case CAMERA_TRIGGER_DETECT_PLACEMENT:
            if (state->weight_gram <= CAMERA_EMPTY_THRESHOLD_G)
            {
                camera_set_status(state, CAMERA_TRIGGER_EMPTY, "请放置商品");
            }
            else
            {
                state->stable_start_tick_ms = now_tick_ms;
                state->stable_weight_gram = state->weight_gram;
                state->stable_sample_count = 0U;
                camera_set_status(state, CAMERA_TRIGGER_WAIT_STABLE, "等待重量稳定");
            }
            break;

        case CAMERA_TRIGGER_WAIT_STABLE:
            if (state->weight_gram <= CAMERA_EMPTY_THRESHOLD_G)
            {
                printf("[FLOW] Item removed before stability\r\n");
                state->stable_sample_count = 0U;
                camera_set_status(state, CAMERA_TRIGGER_EMPTY, "请放置商品");
            }
            else if (weight_delta <= CAMERA_STABLE_DELTA_G)
            {
                uint32_t stable_elapsed_ms;

                state->stable_weight_gram = state->weight_gram;
                if (state->stable_sample_count < 0xFFU)
                {
                    state->stable_sample_count++;
                }

                stable_elapsed_ms = now_tick_ms - state->stable_start_tick_ms;
                if ((stable_elapsed_ms >= CAMERA_STABLE_HOLD_MS) ||
                    ((stable_elapsed_ms >= CAMERA_FAST_STABLE_MIN_MS) &&
                     (state->stable_sample_count >= CAMERA_FAST_STABLE_COUNT)))
                {
                    camera_set_status(state, CAMERA_TRIGGER_RECOGNIZING, "重量稳定，识别中");
                    camera_workflow_handle_capture(state);
                    if (state->last_cart_add.valid != 0U)
                    {
                        camera_set_status(state, CAMERA_TRIGGER_WAIT_REMOVE, "识别成功，请取走商品");
                    }
                    else
                    {
                        camera_set_status(state, CAMERA_TRIGGER_WAIT_REMOVE, "识别失败，请取走重试");
                    }
                }
            }
            else
            {
                if (weight_delta > CAMERA_UNSTABLE_RESET_G)
                {
                    state->stable_start_tick_ms = now_tick_ms;
                }
                state->stable_weight_gram = state->weight_gram;
                state->stable_sample_count = 0U;
                printf("[FLOW] Weight unstable: current=%lu g delta=%lu g\r\n",
                       (unsigned long)state->weight_gram,
                       (unsigned long)weight_delta);
                camera_update_text_only(state);
            }
            break;

        case CAMERA_TRIGGER_RECOGNIZING:
            /* Recognition is handled synchronously, so this state is transient. */
            break;

        case CAMERA_TRIGGER_WAIT_REMOVE:
            if (state->weight_gram <= CAMERA_EMPTY_THRESHOLD_G)
            {
                printf("[FLOW] Item removed, next round enabled\r\n");
                state->stable_weight_gram = 0U;
                state->stable_sample_count = 0U;
                state->flow_start_cycles = 0U;
                state->pending_image_refresh = 0U;
                memset(&state->last_cart_add, 0, sizeof(state->last_cart_add));
                state->last_result_valid = 0U;
                state->recognition_time_valid = 0U;
                state->total_flow_time_valid = 0U;
                camera_set_status(state, CAMERA_TRIGGER_EMPTY, "请放置商品");
            }
            break;

        default:
            camera_set_status(state, CAMERA_TRIGGER_EMPTY, "请放置商品");
            break;
    }
}

int camera_workflow_remove_product(camera_workflow_state_t *state, product_id_t product_id)
{
    if (state == 0)
    {
        return -1;
    }

    return cart_service_remove_item(&state->cart, product_id);
}

int camera_workflow_remove_selected_item(camera_workflow_state_t *state)
{
    uint32_t idx;

    if (state == 0)
    {
        return -1;
    }

    if (state->cart.selected_item_index < 0)
    {
        return -1;
    }

    idx = (uint32_t)state->cart.selected_item_index;
    if ((idx >= PRODUCT_ID_COUNT) || (state->cart.items[idx].line_valid == 0U))
    {
        state->cart.selected_item_index = -1;
        return -1;
    }

    state->cart.selected_item_index = -1;
    return cart_service_remove_item(&state->cart, state->cart.items[idx].product_id);
}

void camera_workflow_select_cart_item(camera_workflow_state_t *state, int32_t selected_item_index)
{
    if (state == 0)
    {
        return;
    }

    if ((selected_item_index < 0) || ((uint32_t)selected_item_index >= PRODUCT_ID_COUNT))
    {
        state->cart.selected_item_index = -1;
    }
    else if (state->cart.items[selected_item_index].line_valid == 0U)
    {
        state->cart.selected_item_index = -1;
    }
    else if (state->cart.selected_item_index == selected_item_index)
    {
        state->cart.selected_item_index = -1;
    }
    else
    {
        state->cart.selected_item_index = selected_item_index;
    }
}

void camera_workflow_clear_cart(camera_workflow_state_t *state)
{
    if (state == 0)
    {
        return;
    }

    cart_service_clear(&state->cart);
}

void camera_workflow_continue_shopping(camera_workflow_state_t *state)
{
    if (state == 0)
    {
        return;
    }

    state->cart.selected_item_index = -1;
    memset(&state->last_cart_add, 0, sizeof(state->last_cart_add));
    state->last_result_valid = 0U;
    state->recognition_time_valid = 0U;
    state->total_flow_time_valid = 0U;
    state->notice_pending = 0U;
    state->notice_type = CAMERA_NOTICE_NONE;
    state->notice_text[0] = '\0';

    if (state->weight_gram <= CAMERA_EMPTY_THRESHOLD_G)
    {
        state->stable_weight_gram = 0U;
        state->stable_sample_count = 0U;
        state->flow_start_cycles = 0U;
        state->pending_image_refresh = 0U;
        camera_set_status(state, CAMERA_TRIGGER_EMPTY, "请放置商品");
    }
    else
    {
        camera_set_status(state, CAMERA_TRIGGER_WAIT_REMOVE, "请取走商品后继续");
    }
}

void camera_workflow_complete_checkout(camera_workflow_state_t *state)
{
    if (state == 0)
    {
        return;
    }

    cart_service_clear(&state->cart);
    camera_workflow_continue_shopping(state);
}

int camera_workflow_pop_notice(camera_workflow_state_t *state,
                               camera_notice_type_t *out_type,
                               char *out_text,
                               uint32_t text_size)
{
    if ((state == 0) || (state->notice_pending == 0U))
    {
        return 0;
    }

    if (out_type != 0)
    {
        *out_type = state->notice_type;
    }

    if ((out_text != 0) && (text_size > 0U))
    {
        snprintf(out_text, text_size, "%s", state->notice_text);
    }

    state->notice_pending = 0U;
    state->notice_type = CAMERA_NOTICE_NONE;
    state->notice_text[0] = '\0';
    return 1;
}

const cart_item_t *camera_workflow_get_cart_items(const camera_workflow_state_t *state, uint32_t *out_count)
{
    if (state == 0)
    {
        return 0;
    }

    return cart_service_get_items(&state->cart, out_count);
}

const uint16_t *camera_workflow_get_frame_buffer(void)
{
    return (const uint16_t *)camera_date_buf;
}
