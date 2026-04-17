#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/HYPERRAM/hyperram.h"
#include "./BSP/NORFLASH/norflash_ex.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/HX711/hx711.h"
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/DCMIPP/dcmipp.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "app_x-cube-ai.h"
#include "ai_class_labels.h"
#include "network.h"
#include "network_data.h"

#define CAMERA_FRAME_BUFFER_ADDR  (0x701F4000UL)
#define CAMERA_AI_ROI_MIN_SIZE    (96U)
#define CAMERA_INIT_RETRY_DELAY_MS  (100U)
#define HX711_MEDIAN_LEN             (5U)
#define HX711_MEDIAN_INDEX           (HX711_MEDIAN_LEN / 2U)
#define HX711_SCALE_FACTOR           (51000U)

volatile uint8_t g_ov_frame = 0U;

static uint16_t g_camera_frame_width = 0U;
static uint16_t g_camera_frame_height = 0U;
static uint16_t g_camera_display_yoffset = 0U;
static uint16_t g_camera_roi_x = 0U;
static uint16_t g_camera_roi_y = 0U;
static uint16_t g_camera_roi_size = 0U;
static app_ai_result_t g_camera_last_result = {0U, 0.0f, 0.0f};
static uint8_t g_camera_last_result_valid = 0U;
static uint32_t g_camera_inference_counter = 0U;
static uint32_t g_camera_weight_gram = 0U;
static uint8_t g_camera_weight_valid = 0U;
static uint32_t g_camera_recognition_time_ms_x10 = 0U;
static uint8_t g_camera_recognition_time_valid = 0U;
static uint32_t g_hx711_buffer[HX711_MEDIAN_LEN];
static uint8_t g_hx711_medlen = 0U;
static uint32_t g_hx711_tare = 0U;

static void camera_draw_overlay(void);
static void camera_draw_text_overlay(void);

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

static uint8_t hx711_update_weight(void)
{
    uint8_t i;
    uint32_t median = 0U;
    uint32_t scaled_value;
    uint32_t delta;

    if (hx711_is_ready() == 0U)
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
        g_camera_weight_gram = (uint32_t)((float)delta * 0.00001f * HX711_SCALE_FACTOR);
    }
    else
    {
        g_camera_weight_gram = 0U;
    }

    g_camera_weight_valid = 1U;
    return 0U;
}

void jpeg_data_process(void)
{
    /* The camera workflow uses single-frame RGB565 capture triggered by KEY0. */
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

static void camera_prepare_geometry(void)
{
    uint16_t fullheight;
    uint16_t min_dimension;

    g_camera_frame_width = lcddev.width;

    /* Match the proven experiment-38 geometry: OV2640 native aspect is 4:3,
       and the output height must stay aligned to 4 pixels. */
    fullheight = (uint16_t)((g_camera_frame_width * 3U) / 4U);
    fullheight &= 0xFFFCU;

    if (fullheight > lcddev.height)
    {
        fullheight = lcddev.height & 0xFFFCU;
    }

    g_camera_frame_height = fullheight;
    g_camera_display_yoffset = (uint16_t)((lcddev.height - g_camera_frame_height) / 2U);

    min_dimension = (g_camera_frame_width < g_camera_frame_height) ? g_camera_frame_width : g_camera_frame_height;
    g_camera_roi_size = (uint16_t)((min_dimension / 2U) & 0xFFFCU);
    if (g_camera_roi_size < CAMERA_AI_ROI_MIN_SIZE)
    {
        g_camera_roi_size = CAMERA_AI_ROI_MIN_SIZE;
    }
    if (g_camera_roi_size > min_dimension)
    {
        g_camera_roi_size = min_dimension & 0xFFFCU;
    }
    g_camera_roi_x = (uint16_t)((g_camera_frame_width - g_camera_roi_size) / 2U);
    g_camera_roi_y = (uint16_t)((g_camera_frame_height - g_camera_roi_size) / 2U);
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

static int camera_init(void)
{
    printf("[CAM] Initializing KEY and LCD...\r\n");
    key_init();
    lcd_init();
    hx711_init();
    camera_prepare_geometry();
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

    if (ov2640_outsize_set(4U, 0U, g_camera_frame_width, g_camera_frame_height) != 0U)
    {
        printf("[CAM] Failed to configure OV2640 output size\r\n");
        return -1;
    }

    printf("[CAM] Camera ready: RGB565 %ux%u, frame buffer @ 0x%08lX\r\n",
           g_camera_frame_width,
           g_camera_frame_height,
           CAMERA_FRAME_BUFFER_ADDR);
    camera_draw_overlay();

    return 0;
}

static void camera_draw_text_overlay(void)
{
    char line1[64];
    char line2[96];
    char line3[96];
    char line4[96];
    uint16_t text_origin_y;

    if (g_camera_display_yoffset > 0U)
    {
        lcd_fill(0, 0, lcddev.width - 1U, g_camera_display_yoffset - 1U, WHITE);
        text_origin_y = 30U;
    }
    else
    {
        lcd_fill(0, 0, lcddev.width - 1U, 62U, WHITE);
        text_origin_y = 12U;
    }

    snprintf(line1, sizeof(line1), "ROI %ux%u  KEY0 = capture",
             g_camera_roi_size,
             g_camera_roi_size);

    if (g_camera_last_result_valid != 0U)
    {
        snprintf(line2, sizeof(line2), "Label: %s",
                 ai_class_label_get(g_camera_last_result.top1_index));
    }
    else
    {
        snprintf(line2, sizeof(line2), "Label: N/A");
    }

    if (g_camera_weight_valid != 0U)
    {
        snprintf(line3, sizeof(line3), "Weight: %lu g", (unsigned long)g_camera_weight_gram);
    }
    else
    {
        snprintf(line3, sizeof(line3), "Weight: N/A");
    }

    if (g_camera_recognition_time_valid != 0U)
    {
        snprintf(line4, sizeof(line4), "Recognize: %lu.%lu ms",
                 (unsigned long)(g_camera_recognition_time_ms_x10 / 10U),
                 (unsigned long)(g_camera_recognition_time_ms_x10 % 10U));
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

static void camera_draw_overlay(void)
{
    uint16_t box_x1;
    uint16_t box_y1;
    uint16_t box_x2;
    uint16_t box_y2;

    camera_draw_text_overlay();

    box_x1 = g_camera_roi_x;
    box_y1 = (uint16_t)(g_camera_display_yoffset + g_camera_roi_y);
    box_x2 = (uint16_t)(box_x1 + g_camera_roi_size - 1U);
    box_y2 = (uint16_t)(box_y1 + g_camera_roi_size - 1U);

    lcd_draw_rectangle(box_x1, box_y1, box_x2, box_y2, RED);
    lcd_draw_rectangle((uint16_t)(box_x1 + 1U), (uint16_t)(box_y1 + 1U),
                       (uint16_t)(box_x2 - 1U), (uint16_t)(box_y2 - 1U), RED);
}

static void camera_display_snapshot(void)
{
    lcd_color_fill(0,
                   g_camera_display_yoffset,
                   g_camera_frame_width - 1U,
                   g_camera_display_yoffset + g_camera_frame_height - 1U,
                   (uint16_t *)camera_date_buf);
}

int main(void)
{
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
    printf("[BOOT] Firmware build: key0-label-weight-v1\r\n");
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

    if (camera_init() != 0)
    {
        printf("[BOOT] Camera init failed\r\n");
        boot_error_halt();
    }

    printf("[BOOT] System ready: KEY0 captures one frame and updates label + weight\r\n");

    while (1)
    {
        const uint8_t key = key_scan(0U);

        if (key == KEY0_PRES)
        {
            uint8_t classify_ok = 0U;
            uint32_t total_start_cycles;
            uint32_t capture_end_cycles;
            uint32_t infer_end_cycles;
            uint32_t weight_end_cycles;
            uint32_t text_end_cycles;
            uint32_t display_end_cycles;

            printf("[BOOT] KEY0 pressed: capture and classify\r\n");

            total_start_cycles = perf_cycle_counter_get();
            dcmipp_start();
            capture_end_cycles = perf_cycle_counter_get();

            if (APP_AI_ClassifyRgb565Roi((const uint16_t *)camera_date_buf,
                                         g_camera_frame_width,
                                         g_camera_frame_height,
                                         g_camera_roi_x,
                                         g_camera_roi_y,
                                         g_camera_roi_size,
                                         g_camera_roi_size,
                                         "live roi",
                                         &g_camera_last_result) == 0)
            {
                g_camera_last_result_valid = 1U;
                g_camera_inference_counter++;
                classify_ok = 1U;
            }
            else
            {
                g_camera_last_result_valid = 0U;
                g_camera_recognition_time_valid = 0U;
            }
            infer_end_cycles = perf_cycle_counter_get();

            if (classify_ok != 0U)
            {
                g_camera_recognition_time_ms_x10 =
                    (uint32_t)(perf_cycles_to_ms(infer_end_cycles - total_start_cycles) * 10.0f + 0.5f);
                g_camera_recognition_time_valid = 1U;
            }

            hx711_update_weight();
            weight_end_cycles = perf_cycle_counter_get();

            camera_draw_text_overlay();
            text_end_cycles = perf_cycle_counter_get();

            camera_display_snapshot();
            camera_draw_overlay();
            display_end_cycles = perf_cycle_counter_get();

            printf("[PERF] capture=%.2f ms, ai=%.2f ms, weight=%.2f ms, text=%.2f ms, lcd=%.2f ms, total=%.2f ms\r\n",
                   perf_cycles_to_ms(capture_end_cycles - total_start_cycles),
                   perf_cycles_to_ms(infer_end_cycles - capture_end_cycles),
                   perf_cycles_to_ms(weight_end_cycles - infer_end_cycles),
                   perf_cycles_to_ms(text_end_cycles - weight_end_cycles),
                   perf_cycles_to_ms(display_end_cycles - text_end_cycles),
                   perf_cycles_to_ms(display_end_cycles - total_start_cycles));
        }

        LED0_TOGGLE();
    }
}

void Error_Handler(void)
{
    printf("[BOOT] Error_Handler entered\r\n");
    boot_error_halt();
}
