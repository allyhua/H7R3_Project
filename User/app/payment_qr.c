#include "payment_qr.h"

#include <stdio.h>
#include <string.h>

#include "./BSP/ATKQR/atk_qrdecode.h"
#include "./BSP/DCMIPP/dcmipp.h"
#include "stm32h7rsxx_hal.h"

#define PAYMENT_QR_ATK_TYPE_QR      (0U)
#define PAYMENT_QR_SCAN_MAX_SIZE    (280U)
#define PAYMENT_QR_SCAN_FIRST_SIZE  (240U)
#define PAYMENT_QR_SCAN_MIN_SIZE    (160U)
#define PAYMENT_QR_SCAN_SIZE_COUNT  (4U)
#ifndef PAYMENT_QR_SCAN_TRY_HMIRROR
#define PAYMENT_QR_SCAN_TRY_HMIRROR (0U)
#endif
#ifndef PAYMENT_QR_VERBOSE_SCAN_LOG
#define PAYMENT_QR_VERBOSE_SCAN_LOG (0U)
#endif
#ifndef PAYMENT_QR_LOG_LEVEL
#define PAYMENT_QR_LOG_LEVEL        (1U)
#endif
#define PAYMENT_QR_LOG_ERROR        (1U)
#define PAYMENT_QR_LOG_INFO         (2U)
#define PAYMENT_QR_LOG_DEBUG        (3U)
#define PAYMENT_QR_LOG(level, ...)  do { if (PAYMENT_QR_LOG_LEVEL >= (level)) { printf(__VA_ARGS__); } } while (0)

typedef struct
{
    payment_qr_status_t status;
    char last_text[PAYMENT_QR_RESULT_MAX_LEN];
    uint32_t last_text_length;
    uint16_t last_success_scan_size;
    uint8_t last_decode_result;
    uint8_t initialized;
} payment_qr_context_t;

static payment_qr_context_t g_payment_qr;

static uint8_t *payment_qr_get_gray_buffer(void)
{
    return (uint8_t *)camera_date_buf + PAYMENT_QR_GRAY_OFFSET_BYTES;
}

static void payment_qr_print_error3_hint(void)
{
    PAYMENT_QR_LOG(PAYMENT_QR_LOG_DEBUG, "[PAY_QR] QR decode error 3\r\n");
}

static void payment_qr_clear_last_text(void)
{
    memset(g_payment_qr.last_text, 0, sizeof(g_payment_qr.last_text));
    g_payment_qr.last_text_length = 0U;
}

static void payment_qr_store_printable_text(const uint8_t *text)
{
    uint32_t src_idx;
    uint32_t dst_idx = 0U;

    payment_qr_clear_last_text();
    if (text == NULL)
    {
        return;
    }

    for (src_idx = 0U; src_idx < (PAYMENT_QR_RESULT_MAX_LEN - 1U); src_idx++)
    {
        uint8_t ch = text[src_idx];

        if (ch == '\0')
        {
            break;
        }

        if ((ch >= 0x20U) && (ch <= 0x7EU))
        {
            g_payment_qr.last_text[dst_idx++] = (char)ch;
        }
        else
        {
            g_payment_qr.last_text[dst_idx++] = '?';
        }

        if (dst_idx >= (PAYMENT_QR_RESULT_MAX_LEN - 1U))
        {
            break;
        }
    }

    g_payment_qr.last_text[dst_idx] = '\0';
    g_payment_qr.last_text_length = dst_idx;
}

static uint16_t payment_qr_get_scan_size(uint16_t frame_width, uint16_t frame_height)
{
    uint16_t roi_size = (frame_width < frame_height) ? frame_width : frame_height;

    if (roi_size > PAYMENT_QR_SCAN_MAX_SIZE)
    {
        roi_size = PAYMENT_QR_SCAN_MAX_SIZE;
    }
    roi_size &= 0xFFFCU;

    return roi_size;
}

static uint16_t payment_qr_clamp_scan_size(uint16_t scan_size, uint16_t max_scan_size)
{
    if (scan_size > max_scan_size)
    {
        scan_size = max_scan_size;
    }

    scan_size &= 0xFFFCU;
    if (scan_size < PAYMENT_QR_SCAN_MIN_SIZE)
    {
        return 0U;
    }

    return scan_size;
}

static uint8_t payment_qr_scan_size_exists(const uint16_t *scan_sizes,
                                           uint8_t scan_count,
                                           uint16_t scan_size)
{
    uint8_t idx;

    for (idx = 0U; idx < scan_count; idx++)
    {
        if (scan_sizes[idx] == scan_size)
        {
            return 1U;
        }
    }

    return 0U;
}

static void payment_qr_add_scan_size(uint16_t *scan_sizes,
                                     uint8_t *scan_count,
                                     uint16_t max_scan_size,
                                     uint16_t scan_size)
{
    uint16_t clamped_size;

    if ((scan_sizes == NULL) || (scan_count == NULL) || (*scan_count >= PAYMENT_QR_SCAN_SIZE_COUNT))
    {
        return;
    }

    clamped_size = payment_qr_clamp_scan_size(scan_size, max_scan_size);
    if ((clamped_size == 0U) ||
        (payment_qr_scan_size_exists(scan_sizes, *scan_count, clamped_size) != 0U))
    {
        return;
    }

    scan_sizes[*scan_count] = clamped_size;
    (*scan_count)++;
}

static uint8_t payment_qr_build_scan_sizes(uint16_t max_scan_size, uint16_t *scan_sizes)
{
    uint8_t scan_count = 0U;

    if (scan_sizes == NULL)
    {
        return 0U;
    }

    if (g_payment_qr.last_success_scan_size >= PAYMENT_QR_SCAN_MIN_SIZE)
    {
        payment_qr_add_scan_size(scan_sizes,
                                 &scan_count,
                                 max_scan_size,
                                 g_payment_qr.last_success_scan_size);
    }

    payment_qr_add_scan_size(scan_sizes, &scan_count, max_scan_size, PAYMENT_QR_SCAN_FIRST_SIZE);
    payment_qr_add_scan_size(scan_sizes, &scan_count, max_scan_size, PAYMENT_QR_SCAN_MAX_SIZE);
    payment_qr_add_scan_size(scan_sizes, &scan_count, max_scan_size, 200U);
    payment_qr_add_scan_size(scan_sizes, &scan_count, max_scan_size, PAYMENT_QR_SCAN_MIN_SIZE);

    return scan_count;
}

static uint8_t payment_qr_rgb565_to_gray_roi(const uint16_t *frame,
                                             uint16_t frame_width,
                                             uint16_t frame_height,
                                             uint16_t roi_size,
                                             uint8_t hmirror,
                                             uint16_t *out_width,
                                             uint16_t *out_height)
{
    uint8_t *gray = payment_qr_get_gray_buffer();
    uint16_t start_x;
    uint16_t start_y;
    uint16_t y;
    uint8_t min_gray = 255U;
    uint8_t max_gray = 0U;

    if ((frame == NULL) ||
        (out_width == NULL) ||
        (out_height == NULL) ||
        (roi_size < PAYMENT_QR_SCAN_MIN_SIZE) ||
        (roi_size > frame_width) ||
        (roi_size > frame_height) ||
        (((uint32_t)roi_size * (uint32_t)roi_size) > PAYMENT_QR_GRAY_MAX_BYTES))
    {
        return 1U;
    }

    roi_size &= 0xFFFCU;
    start_x = (uint16_t)((frame_width - roi_size) / 2U);
    start_y = (uint16_t)((frame_height - roi_size) / 2U);

    for (y = 0U; y < roi_size; y++)
    {
        uint16_t x;
        const uint16_t *src_row = frame + ((uint32_t)(start_y + y) * (uint32_t)frame_width) + start_x;
        uint8_t *dst_row = gray + ((uint32_t)y * (uint32_t)roi_size);

        for (x = 0U; x < roi_size; x++)
        {
            uint16_t src_x = (hmirror != 0U) ? (uint16_t)(roi_size - 1U - x) : x;
            uint16_t pixel = src_row[src_x];
            uint32_t r = (uint32_t)((pixel >> 11U) & 0x1FU) << 3U;
            uint32_t g = (uint32_t)((pixel >> 5U) & 0x3FU) << 2U;
            uint32_t b = (uint32_t)(pixel & 0x1FU) << 3U;
            uint8_t gray_value = (uint8_t)((r * 77U + g * 150U + b * 29U) >> 8U);

            dst_row[x] = gray_value;
            if (gray_value < min_gray)
            {
                min_gray = gray_value;
            }
            if (gray_value > max_gray)
            {
                max_gray = gray_value;
            }
        }
    }

    if ((uint8_t)(max_gray - min_gray) > 20U)
    {
        uint32_t idx;
        uint32_t count = (uint32_t)roi_size * (uint32_t)roi_size;
        uint32_t range = (uint32_t)(max_gray - min_gray);

        for (idx = 0U; idx < count; idx++)
        {
            uint32_t value = gray[idx];

            if (value <= min_gray)
            {
                gray[idx] = 0U;
            }
            else if (value >= max_gray)
            {
                gray[idx] = 255U;
            }
            else
            {
                gray[idx] = (uint8_t)(((value - min_gray) * 255U) / range);
            }
        }
    }

    *out_width = roi_size;
    *out_height = roi_size;
    return 0U;
}

payment_qr_status_t payment_qr_init(void)
{
    uint8_t result;

    if (g_payment_qr.initialized != 0U)
    {
        g_payment_qr.status = PAYMENT_QR_INIT_OK;
        return g_payment_qr.status;
    }

    payment_qr_clear_last_text();
    atk_qr_heap_reset();
    result = atk_qr_init();
    if (result == ATK_QR_OK)
    {
        g_payment_qr.initialized = 1U;
        g_payment_qr.last_decode_result = 0xFFU;
        g_payment_qr.status = PAYMENT_QR_INIT_OK;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_INFO, "[PAY_QR] QR init ok\r\n");
        return g_payment_qr.status;
    }

    g_payment_qr.initialized = 0U;
    g_payment_qr.last_decode_result = result;
    g_payment_qr.status = PAYMENT_QR_ERROR;
    PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR init fail, res=%u\r\n", (unsigned)result);

    return g_payment_qr.status;
}

void payment_qr_reset(void)
{
    payment_qr_clear_last_text();
    g_payment_qr.last_decode_result = 0xFFU;
    g_payment_qr.status = g_payment_qr.initialized ? PAYMENT_QR_INIT_OK : PAYMENT_QR_IDLE;
    PAYMENT_QR_LOG(PAYMENT_QR_LOG_DEBUG, "[PAY_QR] QR reset\r\n");
}

payment_qr_status_t payment_qr_process_frame(const uint8_t *gray_image,
                                             uint16_t width,
                                             uint16_t height)
{
    uint8_t decode_result;
    uint32_t start_tick;
    uint32_t elapsed_ms;
    uint8_t decoded_text[PAYMENT_QR_RESULT_MAX_LEN];

    g_payment_qr.last_decode_result = 0xFFU;

    if (g_payment_qr.initialized == 0U)
    {
        g_payment_qr.status = PAYMENT_QR_ERROR;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR process rejected: not initialized\r\n");
        return g_payment_qr.status;
    }

    if ((gray_image == NULL) || (width == 0U) || (height == 0U))
    {
        payment_qr_clear_last_text();
        g_payment_qr.status = PAYMENT_QR_ERROR;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR process rejected: invalid grayscale frame\r\n");
        return g_payment_qr.status;
    }

    memset(decoded_text, 0, sizeof(decoded_text));
#if PAYMENT_QR_VERBOSE_SCAN_LOG
    PAYMENT_QR_LOG(PAYMENT_QR_LOG_DEBUG, "[PAY_QR] QR scanning\r\n");
#endif

    start_tick = HAL_GetTick();
    decode_result = atk_qr_decode(width,
                                  height,
                                  (uint8_t *)gray_image,
                                  PAYMENT_QR_ATK_TYPE_QR,
                                  decoded_text);
    elapsed_ms = HAL_GetTick() - start_tick;
    g_payment_qr.last_decode_result = decode_result;

    if ((decode_result == ATK_QR_OK) || (PAYMENT_QR_VERBOSE_SCAN_LOG != 0U))
    {
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_INFO,
                       "[PAY_QR] QR decode ret = %u, elapsed=%lu ms\r\n",
                       (unsigned)decode_result,
                       (unsigned long)elapsed_ms);
    }

    if (decode_result != ATK_QR_OK)
    {
        payment_qr_clear_last_text();
        if (decode_result == ATK_QR_RECO_ERR)
        {
            g_payment_qr.status = PAYMENT_QR_RECO_ERR;
            if (PAYMENT_QR_VERBOSE_SCAN_LOG != 0U)
            {
                payment_qr_print_error3_hint();
            }
        }
        else
        {
            g_payment_qr.status = PAYMENT_QR_ERROR;
        }
        return g_payment_qr.status;
    }

    if (decoded_text[0] == '\0')
    {
        payment_qr_clear_last_text();
        g_payment_qr.status = PAYMENT_QR_SCANNING;
#if PAYMENT_QR_VERBOSE_SCAN_LOG
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_DEBUG, "[PAY_QR] QR scanning: no text detected\r\n");
#endif
        return g_payment_qr.status;
    }

    payment_qr_store_printable_text(decoded_text);
    g_payment_qr.status = PAYMENT_QR_DETECTED;
    PAYMENT_QR_LOG(PAYMENT_QR_LOG_INFO, "[PAY_QR] QR detected text: %s\r\n", g_payment_qr.last_text);

    if (strcmp(g_payment_qr.last_text, PAYMENT_QR_TARGET_TEXT) == 0)
    {
        g_payment_qr.status = PAYMENT_QR_VALID;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_INFO, "[PAY_QR] PAYMENT QR VALID\r\n");
    }
    else
    {
        g_payment_qr.status = PAYMENT_QR_INVALID;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR invalid: %s\r\n", g_payment_qr.last_text);
    }

    return g_payment_qr.status;
}

payment_qr_status_t payment_qr_process_current_frame(uint16_t frame_width,
                                                     uint16_t frame_height)
{
    uint16_t max_scan_size;
    uint16_t scan_sizes[PAYMENT_QR_SCAN_SIZE_COUNT];
    uint8_t scan_count;
    uint8_t scan_idx;
    payment_qr_status_t last_status = PAYMENT_QR_RECO_ERR;

    if (payment_qr_init() != PAYMENT_QR_INIT_OK)
    {
        return g_payment_qr.status;
    }

    if ((frame_width == 0U) || (frame_height == 0U))
    {
        g_payment_qr.status = PAYMENT_QR_ERROR;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR process rejected: invalid RGB565 frame size\r\n");
        return g_payment_qr.status;
    }

    max_scan_size = payment_qr_get_scan_size(frame_width, frame_height);
    if (max_scan_size < PAYMENT_QR_SCAN_MIN_SIZE)
    {
        g_payment_qr.status = PAYMENT_QR_ERROR;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR process rejected: frame too small for ROI\r\n");
        return g_payment_qr.status;
    }

    scan_count = payment_qr_build_scan_sizes(max_scan_size, scan_sizes);
    if (scan_count == 0U)
    {
        g_payment_qr.status = PAYMENT_QR_ERROR;
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_ERROR, "[PAY_QR] QR process rejected: no valid ROI size\r\n");
        return g_payment_qr.status;
    }

    for (scan_idx = 0U; scan_idx < scan_count; scan_idx++)
    {
        uint16_t try_size = scan_sizes[scan_idx];
        uint16_t gray_width = 0U;
        uint16_t gray_height = 0U;
        uint8_t mirror_pass;

        for (mirror_pass = 0U; mirror_pass <= (uint8_t)PAYMENT_QR_SCAN_TRY_HMIRROR; mirror_pass++)
        {
            if (payment_qr_rgb565_to_gray_roi((const uint16_t *)camera_date_buf,
                                              frame_width,
                                              frame_height,
                                              try_size,
                                              mirror_pass,
                                              &gray_width,
                                              &gray_height) == 0U)
            {
#if PAYMENT_QR_VERBOSE_SCAN_LOG
                PAYMENT_QR_LOG(PAYMENT_QR_LOG_DEBUG,
                               "[PAY_QR] QR ROI %ux%u, mode=%s, gray @ %p\r\n",
                               gray_width,
                               gray_height,
                               (mirror_pass != 0U) ? "hmirror" : "normal",
                               payment_qr_get_gray_buffer());
#endif
                last_status = payment_qr_process_frame(payment_qr_get_gray_buffer(),
                                                       gray_width,
                                                       gray_height);
                if ((last_status == PAYMENT_QR_VALID) ||
                    (last_status == PAYMENT_QR_INVALID) ||
                    (last_status == PAYMENT_QR_DETECTED))
                {
                    g_payment_qr.last_success_scan_size = try_size;
                    PAYMENT_QR_LOG(PAYMENT_QR_LOG_INFO,
                                   "[PAY_QR] QR detected roi=%u mode=%s\r\n",
                                   (unsigned)try_size,
                                   (mirror_pass != 0U) ? "hmirror" : "normal");
                    return last_status;
                }
            }
        }
    }

    if (last_status == PAYMENT_QR_ERROR)
    {
        g_payment_qr.status = PAYMENT_QR_ERROR;
    }
    else if (last_status == PAYMENT_QR_RECO_ERR)
    {
        g_payment_qr.status = PAYMENT_QR_RECO_ERR;
    }
    else
    {
        g_payment_qr.status = PAYMENT_QR_SCANNING;
    }
    return g_payment_qr.status;
}

const char *payment_qr_get_last_text(void)
{
    return g_payment_qr.last_text;
}

payment_qr_status_t payment_qr_get_status(void)
{
    return g_payment_qr.status;
}

uint8_t payment_qr_get_last_decode_result(void)
{
    return g_payment_qr.last_decode_result;
}

uint8_t payment_qr_is_valid(void)
{
    return (g_payment_qr.status == PAYMENT_QR_VALID) ? 1U : 0U;
}

void payment_qr_deinit(void)
{
    if (g_payment_qr.initialized != 0U)
    {
        atk_qr_destroy();
        PAYMENT_QR_LOG(PAYMENT_QR_LOG_DEBUG, "[PAY_QR] QR deinit\r\n");
    }

    memset(&g_payment_qr, 0, sizeof(g_payment_qr));
    g_payment_qr.status = PAYMENT_QR_IDLE;
}
