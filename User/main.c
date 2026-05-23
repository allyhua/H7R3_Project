#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/DCMIPP/dcmipp.h"
#include "./BSP/HYPERRAM/hyperram.h"
#include "./BSP/NORFLASH/norflash_ex.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "app_x-cube-ai.h"
#include "app/camera_workflow.h"
#include "app/payment_qr.h"
#include "app/payment_qr_test.h"
#include "app/product_catalog.h"
#include "lvgl.h"
#include "ui/lvgl_port.h"
#include "ui/checkout_ui.h"
#include "network.h"
#include "network_data.h"

volatile uint8_t g_ov_frame = 0U;

typedef enum
{
    VISION_MODE_PRODUCT = 0,
    VISION_MODE_PAYMENT_QR
} vision_mode_t;

typedef enum
{
    PAYMENT_FLOW_IDLE = 0,
    PAYMENT_FLOW_SCANNING,
    PAYMENT_FLOW_PAUSED,
    PAYMENT_FLOW_SUCCESS_PENDING,
    PAYMENT_FLOW_DONE
} payment_flow_state_t;

#define PAYMENT_QR_FRAME_TIMEOUT_MS       (20U)
#define PAYMENT_QR_SCAN_INTERVAL_MS       (150U)
#define PAYMENT_QR_SUCCESS_CONFIRM_COUNT  (1U)
#define PAYMENT_QR_SUCCESS_DELAY_MS       (500U)
#define PAYMENT_QR_PREVIEW_INTERVAL_MS    (500U)
#define PAYMENT_QR_FRAME_STALL_RESTART_MS (1000U)
#ifndef PAYMENT_TIMEOUT_MS
#define PAYMENT_TIMEOUT_MS                (30000U)
#endif

#ifndef APP_LOG_LEVEL
#define APP_LOG_LEVEL                     (2U)
#endif
#define APP_LOG_ERROR                     (1U)
#define APP_LOG_INFO                      (2U)
#define APP_LOG_DEBUG                     (3U)
#define APP_LOG(level, ...)               do { if (APP_LOG_LEVEL >= (level)) { printf(__VA_ARGS__); } } while (0)

static vision_mode_t g_vision_mode = VISION_MODE_PRODUCT;
static payment_flow_state_t g_payment_flow_state = PAYMENT_FLOW_IDLE;
static uint8_t g_payment_last_frame_mark = 0U;
static uint8_t g_payment_valid_count = 0U;
static uint32_t g_payment_last_scan_tick = 0U;
static uint32_t g_payment_last_preview_tick = 0U;
static uint32_t g_payment_success_tick = 0U;
static uint32_t g_payment_start_tick = 0U;
static uint32_t g_payment_frame_wait_start_tick = 0U;
static uint32_t g_payment_total_price_cent = 0U;
static uint32_t g_payment_success_total_price_cent = 0U;
static uint8_t g_payment_camera_restart_count = 0U;
static char g_payment_success_order_no[20];
static char g_payment_success_time[24];

static void main_restore_product_mode(void);
static void main_format_runtime_time(uint32_t tick_ms, char *buffer, uint32_t size);
static void main_finish_payment_success(camera_workflow_state_t *state);
static void main_payment_start(void *user_data);
static void main_payment_cancel(void *user_data);
static void main_payment_rescan(void *user_data);
static void main_payment_process(camera_workflow_state_t *state);

void jpeg_data_process(void)
{
    /* The camera workflow uses single-frame RGB565 capture after weight becomes stable. */
}

static void boot_error_halt(void)
{
    while (1)
    {
        LED0_TOGGLE();
        delay_ms(150);
    }
}

static void boot_enable_fault_traps(void)
{
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk |
                  SCB_SHCSR_BUSFAULTENA_Msk |
                  SCB_SHCSR_USGFAULTENA_Msk;
}

static void boot_log_weight_bytes(void)
{
    uint8_t source_header[16];
    const uint8_t *runtime_bytes = (const uint8_t *)ai_network_data_weights_get();
    const uintptr_t runtime_weights_addr = (uintptr_t)runtime_bytes;
    uint32_t idx;

    printf("[BOOT] Embedded model: %s\r\n", AI_NETWORK_ORIGIN_MODEL_NAME);
    printf("[BOOT] Runtime weights @ 0x%08" PRIXPTR " (%u bytes)\r\n",
           runtime_weights_addr,
           (unsigned)AI_NETWORK_DATA_WEIGHTS_SIZE);

    if ((runtime_weights_addr >= 0x70000000UL) && (runtime_weights_addr < 0x80000000UL))
    {
        printf("[BOOT] Weight copy-to-RAM is enabled\r\n");
    }

    if (norflash_ex_read(0x00000000U, source_header, sizeof(source_header)) != 0)
    {
        printf("[BOOT] NOR Flash read failed @ offset 0x00000000\r\n");
        boot_error_halt();
    }

    printf("[BOOT] NOR[0x00000000..0x0000000F] =");
    for (idx = 0U; idx < sizeof(source_header); ++idx)
    {
        printf(" %02X", source_header[idx]);
    }
    printf("\r\n");

    printf("[BOOT] MEM[0x%08" PRIXPTR "..0x%08" PRIXPTR "] =",
           runtime_weights_addr,
           runtime_weights_addr + (uintptr_t)sizeof(source_header) - 1U);
    for (idx = 0U; idx < sizeof(source_header); ++idx)
    {
        printf(" %02X", runtime_bytes[idx]);
    }
    printf("\r\n");
}

static void main_format_money(uint32_t cent, char *buffer, uint32_t size)
{
    if ((buffer == NULL) || (size == 0U))
    {
        return;
    }

    (void)snprintf(buffer,
                   size,
                   "%lu.%02lu 元",
                   (unsigned long)(cent / 100U),
                   (unsigned long)(cent % 100U));
}

static void main_refresh_checkout_ui(const camera_workflow_state_t *state)
{
    static char last_status[48];
    static uint32_t last_weight_gram = 0xFFFFFFFFU;
    static uint32_t last_time_ms_x10 = 0xFFFFFFFFU;
    static uint8_t last_time_valid = 0xFFU;
    static uint32_t last_cart_count = 0xFFFFFFFFU;
    static uint32_t last_total_cent = 0xFFFFFFFFU;
    static uint32_t last_capture_counter = 0xFFFFFFFFU;
    static uint8_t last_result_valid = 0xFFU;
    static product_id_t last_result_product_id = PRODUCT_ID_UNKNOWN;
    static uint32_t last_result_weight_g = 0xFFFFFFFFU;
    static uint32_t last_result_count = 0xFFFFFFFFU;
    static uint32_t last_result_subtotal = 0xFFFFFFFFU;
    const char *status_text;
    uint8_t cart_changed = 0U;

    if (state == NULL)
    {
        return;
    }

    status_text = (state->status_text[0] != '\0') ? state->status_text : "请放置商品";
    if (strncmp(last_status, status_text, sizeof(last_status)) != 0)
    {
        checkout_ui_main_set_status(status_text);
        (void)snprintf(last_status, sizeof(last_status), "%s", status_text);
    }

    if (last_weight_gram != state->weight_gram)
    {
        checkout_ui_main_set_weight_gram(state->weight_gram);
        last_weight_gram = state->weight_gram;
    }

    if ((last_time_valid != state->recognition_time_valid) ||
        (last_time_ms_x10 != state->recognition_time_ms_x10))
    {
        checkout_ui_main_set_recognition_time(state->recognition_time_valid,
                                              state->recognition_time_ms_x10);
        last_time_valid = state->recognition_time_valid;
        last_time_ms_x10 = state->recognition_time_ms_x10;
    }

    if ((last_result_valid != state->last_cart_add.valid) ||
        (last_result_product_id != state->last_cart_add.product_id) ||
        (last_result_weight_g != state->last_cart_add.measured_weight_g) ||
        (last_result_count != state->last_cart_add.counted_units) ||
        (last_result_subtotal != state->last_cart_add.subtotal_cent))
    {
        if (state->last_cart_add.valid != 0U)
        {
            char price_text[32];
            char amount_text[32];
            char subtotal_text[24];
            const product_info_t *product = product_catalog_get(state->last_cart_add.product_id);

            if (product != NULL)
            {
                (void)snprintf(price_text,
                               sizeof(price_text),
                               "%lu.%02lu %s",
                               (unsigned long)(product->unit_price_cent / 100U),
                               (unsigned long)(product->unit_price_cent % 100U),
                               product->price_unit_text);
            }
            else
            {
                (void)snprintf(price_text, sizeof(price_text), "--");
            }

            if (state->last_cart_add.pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
            {
                (void)snprintf(amount_text,
                               sizeof(amount_text),
                               "%lu.%03lu kg",
                               (unsigned long)(state->last_cart_add.measured_weight_g / 1000U),
                               (unsigned long)(state->last_cart_add.measured_weight_g % 1000U));
            }
            else
            {
                (void)snprintf(amount_text,
                               sizeof(amount_text),
                               "%lu %s",
                               (unsigned long)state->last_cart_add.counted_units,
                               (product != NULL) ? product->display_unit_text : "件");
            }

            main_format_money(state->last_cart_add.subtotal_cent, subtotal_text, sizeof(subtotal_text));
            checkout_ui_main_set_result_state("识别完成");
            checkout_ui_main_set_result(state->last_cart_add.product_name,
                                        price_text,
                                        amount_text,
                                        subtotal_text);
        }
        else
        {
            checkout_ui_main_clear_result();
        }

        last_result_valid = state->last_cart_add.valid;
        last_result_product_id = state->last_cart_add.product_id;
        last_result_weight_g = state->last_cart_add.measured_weight_g;
        last_result_count = state->last_cart_add.counted_units;
        last_result_subtotal = state->last_cart_add.subtotal_cent;
    }

    if (last_cart_count != state->cart.item_count)
    {
        checkout_ui_main_set_cart_summary(state->cart.item_count);
        last_cart_count = state->cart.item_count;
        cart_changed = 1U;
    }

    if (last_total_cent != state->cart.total_price_cent)
    {
        checkout_ui_main_set_total_price(state->cart.total_price_cent);
        last_total_cent = state->cart.total_price_cent;
        cart_changed = 1U;
    }

    if (cart_changed != 0U)
    {
        checkout_ui_cart_refresh(&state->cart);
    }

    if (last_capture_counter != state->capture_counter)
    {
        if (state->capture_counter == 0U)
        {
            checkout_ui_main_show_image(0U);
        }
        else
        {
            checkout_ui_main_set_image_source(state->frame_width,
                                              state->frame_height,
                                              camera_workflow_get_frame_buffer());
        }

        last_capture_counter = state->capture_counter;
    }
}

static void main_delete_selected_cart_item(void *user_data)
{
    camera_workflow_state_t *state = (camera_workflow_state_t *)user_data;

    if (state == NULL)
    {
        return;
    }

    camera_workflow_select_cart_item(state, checkout_ui_cart_get_selected_index());
    (void)camera_workflow_remove_selected_item(state);
    main_refresh_checkout_ui(state);
    checkout_ui_refresh_all(&state->cart);
}

static void main_clear_cart(void *user_data)
{
    camera_workflow_state_t *state = (camera_workflow_state_t *)user_data;

    if (state == NULL)
    {
        return;
    }

    camera_workflow_clear_cart(state);
    main_refresh_checkout_ui(state);
    checkout_ui_refresh_all(&state->cart);
}

static void main_restore_product_mode(void)
{
    if (g_vision_mode == VISION_MODE_PAYMENT_QR)
    {
        dcmipp_stop();
    }

    g_vision_mode = VISION_MODE_PRODUCT;
    g_payment_flow_state = PAYMENT_FLOW_IDLE;
    g_payment_valid_count = 0U;
    g_payment_last_scan_tick = 0U;
    g_payment_success_tick = 0U;
    g_payment_start_tick = 0U;
    g_payment_last_preview_tick = 0U;
    g_payment_frame_wait_start_tick = 0U;
}

static void main_format_runtime_time(uint32_t tick_ms, char *buffer, uint32_t size)
{
    uint32_t total_seconds;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;

    if ((buffer == NULL) || (size == 0U))
    {
        return;
    }

    total_seconds = tick_ms / 1000U;
    hour = (total_seconds / 3600U) % 24U;
    minute = (total_seconds / 60U) % 60U;
    second = total_seconds % 60U;
    (void)snprintf(buffer,
                   size,
                   "%02lu:%02lu:%02lu",
                   (unsigned long)hour,
                   (unsigned long)minute,
                   (unsigned long)second);
}

static void main_finish_payment_success(camera_workflow_state_t *state)
{
    const char *order_no;

    if (state == NULL)
    {
        return;
    }

    order_no = checkout_ui_get_payment_order_no();
    (void)snprintf(g_payment_success_order_no,
                   sizeof(g_payment_success_order_no),
                   "%s",
                   (order_no != NULL) ? order_no : "");
    g_payment_success_total_price_cent = g_payment_total_price_cent;
    main_format_runtime_time(HAL_GetTick(), g_payment_success_time, sizeof(g_payment_success_time));

    payment_qr_reset();
    main_restore_product_mode();
    camera_workflow_complete_checkout(state);
    main_refresh_checkout_ui(state);
    checkout_ui_main_clear_result();
    checkout_ui_main_show_image(0U);
    checkout_ui_main_set_status("请放置商品");
    checkout_ui_refresh_all(&state->cart);
    checkout_ui_show_payment_success_page(g_payment_success_order_no,
                                          g_payment_success_total_price_cent,
                                          g_payment_success_time);
}

static void main_complete_checkout(void *user_data)
{
    camera_workflow_state_t *state = (camera_workflow_state_t *)user_data;

    if (state == NULL)
    {
        return;
    }

    main_restore_product_mode();
    camera_workflow_complete_checkout(state);
    main_refresh_checkout_ui(state);
    checkout_ui_main_clear_result();
    checkout_ui_main_set_status("请放置商品");
    checkout_ui_refresh_all(&state->cart);
}

static const cart_context_t *main_get_cart(void *user_data)
{
    camera_workflow_state_t *state = (camera_workflow_state_t *)user_data;

    if (state == NULL)
    {
        return NULL;
    }

    return &state->cart;
}

static void main_payment_start(void *user_data)
{
    camera_workflow_state_t *state = (camera_workflow_state_t *)user_data;

    if (state == NULL)
    {
        return;
    }

    APP_LOG(APP_LOG_INFO, "[PAY] enter payment QR mode\r\n");
    g_vision_mode = VISION_MODE_PAYMENT_QR;
    g_payment_flow_state = PAYMENT_FLOW_SCANNING;
    g_payment_valid_count = 0U;
    g_payment_last_scan_tick = 0U;
    g_payment_last_preview_tick = 0U;
    g_payment_success_tick = 0U;
    g_payment_start_tick = HAL_GetTick();
    g_payment_frame_wait_start_tick = 0U;
    g_payment_camera_restart_count = 0U;
    g_payment_total_price_cent = state->cart.total_price_cent;

    payment_qr_reset();
    checkout_ui_reset_payment_camera_preview();
    if (payment_qr_init() != PAYMENT_QR_INIT_OK)
    {
        APP_LOG(APP_LOG_ERROR, "[PAY] payment QR init failed\r\n");
        checkout_ui_set_payment_invalid();
        checkout_ui_show_payment_invalid_popup();
        g_payment_flow_state = PAYMENT_FLOW_PAUSED;
        return;
    }

    if (dcmipp_start_continuous() != 0)
    {
        APP_LOG(APP_LOG_ERROR, "[PAY] payment camera continuous start failed\r\n");
        checkout_ui_set_payment_invalid();
        checkout_ui_show_payment_invalid_popup();
        g_payment_flow_state = PAYMENT_FLOW_PAUSED;
        return;
    }

    g_payment_last_frame_mark = g_ov_frame;
    checkout_ui_set_payment_scanning();
}

static void main_payment_cancel(void *user_data)
{
    (void)user_data;
    APP_LOG(APP_LOG_INFO, "[PAY] leave payment QR mode\r\n");
    payment_qr_reset();
    main_restore_product_mode();
}

static void main_payment_rescan(void *user_data)
{
    camera_workflow_state_t *state = (camera_workflow_state_t *)user_data;
    uint8_t camera_running;

    if (state == NULL)
    {
        return;
    }

    APP_LOG(APP_LOG_INFO, "[PAY] payment QR rescan\r\n");
    camera_running = ((g_vision_mode == VISION_MODE_PAYMENT_QR) &&
                      (g_payment_flow_state == PAYMENT_FLOW_SCANNING)) ? 1U : 0U;
    payment_qr_reset();
    if (camera_running == 0U)
    {
        checkout_ui_reset_payment_camera_preview();
    }
    if (payment_qr_init() != PAYMENT_QR_INIT_OK)
    {
        APP_LOG(APP_LOG_ERROR, "[PAY] payment QR init failed on rescan\r\n");
        checkout_ui_set_payment_invalid();
        checkout_ui_show_payment_invalid_popup();
        g_payment_flow_state = PAYMENT_FLOW_PAUSED;
        return;
    }

    if (camera_running == 0U)
    {
        dcmipp_stop();
        if (dcmipp_start_continuous() != 0)
        {
            APP_LOG(APP_LOG_ERROR, "[PAY] payment camera continuous start failed on rescan\r\n");
            checkout_ui_set_payment_invalid();
            checkout_ui_show_payment_invalid_popup();
            g_payment_flow_state = PAYMENT_FLOW_PAUSED;
            return;
        }
    }

    g_vision_mode = VISION_MODE_PAYMENT_QR;
    g_payment_flow_state = PAYMENT_FLOW_SCANNING;
    g_payment_valid_count = 0U;
    g_payment_last_scan_tick = 0U;
    g_payment_last_preview_tick = 0U;
    g_payment_success_tick = 0U;
    g_payment_start_tick = HAL_GetTick();
    g_payment_frame_wait_start_tick = 0U;
    g_payment_camera_restart_count = 0U;
    g_payment_total_price_cent = state->cart.total_price_cent;
    g_payment_last_frame_mark = g_ov_frame;
    APP_LOG(APP_LOG_INFO,
            "[PAY] payment QR rescan ready, camera_running=%u, frame=%u\r\n",
            (unsigned)camera_running,
            (unsigned)g_ov_frame);
    checkout_ui_set_payment_scanning();
}

static void main_payment_process(camera_workflow_state_t *state)
{
    uint32_t now_tick;
    payment_qr_status_t status;

    if ((state == NULL) || (g_vision_mode != VISION_MODE_PAYMENT_QR))
    {
        return;
    }

    now_tick = HAL_GetTick();
    if (g_payment_flow_state == PAYMENT_FLOW_SUCCESS_PENDING)
    {
        if ((now_tick - g_payment_success_tick) >= PAYMENT_QR_SUCCESS_DELAY_MS)
        {
            main_finish_payment_success(state);
        }
        return;
    }

    if (g_payment_flow_state != PAYMENT_FLOW_SCANNING)
    {
        return;
    }

    if ((g_payment_valid_count == 0U) &&
        ((now_tick - g_payment_start_tick) >= PAYMENT_TIMEOUT_MS))
    {
        APP_LOG(APP_LOG_ERROR, "[PAY] payment timeout\r\n");
        dcmipp_stop();
        checkout_ui_show_payment_timeout_popup();
        g_payment_flow_state = PAYMENT_FLOW_PAUSED;
        return;
    }

    if ((g_payment_last_scan_tick != 0U) &&
        ((now_tick - g_payment_last_scan_tick) < PAYMENT_QR_SCAN_INTERVAL_MS))
    {
        return;
    }

    if (dcmipp_wait_frame(g_payment_last_frame_mark, PAYMENT_QR_FRAME_TIMEOUT_MS) != 0U)
    {
        uint32_t wait_elapsed;

        if (g_payment_frame_wait_start_tick == 0U)
        {
            g_payment_frame_wait_start_tick = HAL_GetTick();
            return;
        }

        wait_elapsed = HAL_GetTick() - g_payment_frame_wait_start_tick;
        if ((wait_elapsed >= PAYMENT_QR_FRAME_STALL_RESTART_MS) &&
            (g_payment_camera_restart_count == 0U))
        {
            APP_LOG(APP_LOG_ERROR,
                    "[PAY] payment camera frame stalled, restart continuous, frame=%u mark=%u\r\n",
                    (unsigned)g_ov_frame,
                    (unsigned)g_payment_last_frame_mark);
            g_payment_camera_restart_count++;
            dcmipp_stop();
            if (dcmipp_start_continuous() != 0)
            {
                APP_LOG(APP_LOG_ERROR, "[PAY] payment camera restart failed\r\n");
                checkout_ui_show_payment_timeout_popup();
                g_payment_flow_state = PAYMENT_FLOW_PAUSED;
                return;
            }
            g_payment_last_frame_mark = g_ov_frame;
            g_payment_frame_wait_start_tick = HAL_GetTick();
        }
        return;
    }

    g_payment_frame_wait_start_tick = 0U;
    g_payment_last_frame_mark = g_ov_frame;
    g_payment_last_scan_tick = HAL_GetTick();
    dcmipp_invalidate_frame_cache();
    if ((g_payment_last_preview_tick == 0U) ||
        ((g_payment_last_scan_tick - g_payment_last_preview_tick) >= PAYMENT_QR_PREVIEW_INTERVAL_MS))
    {
        checkout_ui_payment_set_camera_source(state->frame_width,
                                              state->frame_height,
                                              (const uint16_t *)camera_date_buf);
        g_payment_last_preview_tick = g_payment_last_scan_tick;
    }
    checkout_ui_set_payment_recognizing();

    status = payment_qr_process_current_frame(state->frame_width, state->frame_height);
    if (status == PAYMENT_QR_VALID)
    {
        g_payment_valid_count++;
        g_payment_start_tick = HAL_GetTick();
        APP_LOG(APP_LOG_INFO, "[PAY] payment QR valid count=%u\r\n", (unsigned)g_payment_valid_count);
        if (g_payment_valid_count >= PAYMENT_QR_SUCCESS_CONFIRM_COUNT)
        {
            dcmipp_stop();
            checkout_ui_set_payment_success();
            g_payment_success_tick = HAL_GetTick();
            g_payment_flow_state = PAYMENT_FLOW_SUCCESS_PENDING;
        }
        return;
    }

    g_payment_valid_count = 0U;
    if (status == PAYMENT_QR_INVALID)
    {
        dcmipp_stop();
        checkout_ui_set_payment_invalid();
        checkout_ui_show_payment_invalid_popup();
        g_payment_flow_state = PAYMENT_FLOW_PAUSED;
    }
    else
    {
        /* Keep the UI in recognizing state to avoid repainting the whole status card every failed decode. */
    }
}

int main(void)
{
    camera_workflow_state_t camera_state;

    sys_mpu_config();
    sys_cache_enable();
    HAL_Init();
    boot_enable_fault_traps();
    sys_stm32_clock_init(300, 6, 2);
    delay_init(600);
    usart_init(115200);
    led_init();

    printf("\r\n============= WEIGHT LABEL DEMO START =============\r\n");
    printf("[BOOT] USART1 ready on PB14/PB15 @115200\r\n");
    printf("[BOOT] Firmware build: auto-weight-stable-trigger-v1\r\n");
    printf("[BOOT] Initializing HyperRAM for copied weights and camera frame buffer...\r\n");

    if (hyperram_init() != 0U)
    {
        printf("[BOOT] HyperRAM init failed\r\n");
        boot_error_halt();
    }

    printf("[BOOT] HyperRAM init ok\r\n");
    printf("[BOOT] Initializing NOR Flash for AI weights...\r\n");

    if (norflash_ex_init() != 0U)
    {
        printf("[BOOT] NOR Flash init failed\r\n");
        boot_error_halt();
    }

    printf("[BOOT] NOR Flash init ok\r\n");
    boot_log_weight_bytes();
    printf("[BOOT] Initializing X-CUBE-AI...\r\n");
    MX_X_CUBE_AI_Init();

    if (camera_workflow_init(&camera_state) != 0)
    {
        printf("[BOOT] Camera init failed\r\n");
        boot_error_halt();
    }

    lv_init();
    lvgl_port_init();
    checkout_ui_framework_init();
    checkout_ui_set_cart_action_callbacks(main_delete_selected_cart_item,
                                          main_clear_cart,
                                          main_complete_checkout,
                                          main_get_cart,
                                          &camera_state);
    checkout_ui_set_payment_action_callbacks(main_payment_start,
                                             main_payment_cancel,
                                             main_payment_rescan,
                                             &camera_state);
    main_refresh_checkout_ui(&camera_state);

    if (PAYMENT_QR_TEST_MODE != 0U)
    {
        payment_qr_test_start(&camera_state);
    }

    printf("[BOOT] System ready: stable weight triggers one capture per load\r\n");

    while (1)
    {
        if (PAYMENT_QR_TEST_MODE != 0U)
        {
            payment_qr_test_loop(&camera_state);
        }
        else
        {
            if (g_vision_mode == VISION_MODE_PAYMENT_QR)
            {
                main_payment_process(&camera_state);
            }
            else
            {
                camera_workflow_process(&camera_state);
                main_refresh_checkout_ui(&camera_state);
            }
        }
        lv_tick_inc(10);
        lv_timer_handler();
        delay_ms(10);
    }
}

void Error_Handler(void)
{
    printf("[BOOT] Error_Handler entered\r\n");
    boot_error_halt();
}
