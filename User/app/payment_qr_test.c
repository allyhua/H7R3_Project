#include "app/payment_qr_test.h"

#include <stdio.h>

#include "./BSP/DCMIPP/dcmipp.h"
#include "app/payment_qr.h"
#include "stm32h7rsxx_hal.h"
#include "ui/checkout_ui.h"

#define PAYMENT_QR_TEST_FRAME_TIMEOUT_MS   (1000U)
#define PAYMENT_QR_TEST_SCAN_INTERVAL_MS   (150U)
#define PAYMENT_QR_TEST_STATUS_LOG_MS      (1000U)

extern volatile uint8_t g_ov_frame;

static uint8_t g_payment_qr_test_started = 0U;
static uint8_t g_payment_qr_test_last_frame_mark = 0U;
static uint32_t g_payment_qr_test_last_scan_tick = 0U;
static uint32_t g_payment_qr_test_last_status_tick = 0U;

static void payment_qr_test_log_error3_hint(void)
{
    printf("QR decode error 3\r\n");
    printf("QR hint: check QR fully inside the recognition box\r\n");
    printf("QR hint: QR width should be 1/3 to 2/3 of the recognition box\r\n");
    printf("QR hint: check overexposure and focus\r\n");
    printf("QR hint: adjust ROI size if it is too large or too small\r\n");
    printf("QR hint: temporarily show grayscale ROI if needed\r\n");
}

void payment_qr_test_start(const camera_workflow_state_t *camera_state)
{
    payment_qr_status_t status;

    if (g_payment_qr_test_started != 0U)
    {
        return;
    }

    if (camera_state == NULL)
    {
        printf("QR test start failed: camera state is null\r\n");
        return;
    }

    printf("QR test mode start, target=%s\r\n", PAYMENT_QR_TARGET_TEXT);
    printf("QR test frame=%ux%u, continuous preview enabled\r\n",
           camera_state->frame_width,
           camera_state->frame_height);

    status = payment_qr_init();
    if (status != PAYMENT_QR_INIT_OK)
    {
        printf("QR decode error: init failed\r\n");
        return;
    }
    printf("QR init ok\r\n");

    if (dcmipp_start_continuous() != 0)
    {
        printf("QR decode error: continuous camera start failed\r\n");
        return;
    }

    g_payment_qr_test_started = 1U;
    g_payment_qr_test_last_frame_mark = g_ov_frame;
    g_payment_qr_test_last_scan_tick = 0U;
    g_payment_qr_test_last_status_tick = 0U;
    printf("QR scanning...\r\n");
}

void payment_qr_test_loop(const camera_workflow_state_t *camera_state)
{
    uint32_t now_tick;
    payment_qr_status_t status;
    const char *last_text;
    uint8_t last_decode_result;

    if ((g_payment_qr_test_started == 0U) || (camera_state == NULL))
    {
        return;
    }

    now_tick = HAL_GetTick();
    if ((g_payment_qr_test_last_scan_tick != 0U) &&
        ((now_tick - g_payment_qr_test_last_scan_tick) < PAYMENT_QR_TEST_SCAN_INTERVAL_MS))
    {
        return;
    }

    if (dcmipp_wait_frame(g_payment_qr_test_last_frame_mark, PAYMENT_QR_TEST_FRAME_TIMEOUT_MS) != 0U)
    {
        if ((now_tick - g_payment_qr_test_last_status_tick) >= PAYMENT_QR_TEST_STATUS_LOG_MS)
        {
            printf("QR scanning... waiting for new camera frame\r\n");
            g_payment_qr_test_last_status_tick = now_tick;
        }
        return;
    }

    g_payment_qr_test_last_frame_mark = g_ov_frame;
    g_payment_qr_test_last_scan_tick = HAL_GetTick();
    dcmipp_invalidate_frame_cache();

    checkout_ui_main_set_image_source(camera_state->frame_width,
                                      camera_state->frame_height,
                                      camera_workflow_get_frame_buffer());

    status = payment_qr_process_current_frame(camera_state->frame_width,
                                              camera_state->frame_height);
    last_text = payment_qr_get_last_text();
    last_decode_result = payment_qr_get_last_decode_result();

    switch (status)
    {
        case PAYMENT_QR_VALID:
            printf("QR detected: %s\r\n", last_text);
            printf("PAYMENT QR VALID\r\n");
            break;

        case PAYMENT_QR_INVALID:
            printf("QR invalid: %s\r\n", last_text);
            break;

        case PAYMENT_QR_SCANNING:
            if ((HAL_GetTick() - g_payment_qr_test_last_status_tick) >= PAYMENT_QR_TEST_STATUS_LOG_MS)
            {
                printf("QR scanning...\r\n");
                g_payment_qr_test_last_status_tick = HAL_GetTick();
            }
            break;

        case PAYMENT_QR_RECO_ERR:
            printf("QR decode ret = %u\r\n", (unsigned)last_decode_result);
            payment_qr_test_log_error3_hint();
            break;

        case PAYMENT_QR_ERROR:
            printf("QR decode ret = %u\r\n", (unsigned)last_decode_result);
            break;

        default:
            break;
    }
}
