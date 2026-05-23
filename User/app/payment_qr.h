#ifndef __PAYMENT_QR_H
#define __PAYMENT_QR_H

#include <stdint.h>

#define PAYMENT_QR_TARGET_TEXT       "PAY_OK_SELF_CHECKOUT_001"
#define PAYMENT_QR_RESULT_MAX_LEN    (256U)
#define PAYMENT_QR_GRAY_OFFSET_BYTES (1024U * 1024U)
#define PAYMENT_QR_GRAY_MAX_BYTES    (512U * 1024U)

#ifndef PAYMENT_QR_TEST_MODE
#define PAYMENT_QR_TEST_MODE         (0U)
#endif

typedef enum
{
    PAYMENT_QR_IDLE = 0,
    PAYMENT_QR_INIT_OK,
    PAYMENT_QR_SCANNING,
    PAYMENT_QR_DETECTED,
    PAYMENT_QR_VALID,
    PAYMENT_QR_INVALID,
    PAYMENT_QR_RECO_ERR,
    PAYMENT_QR_ERROR
} payment_qr_status_t;

payment_qr_status_t payment_qr_init(void);
void payment_qr_reset(void);
payment_qr_status_t payment_qr_process_current_frame(uint16_t frame_width,
                                                     uint16_t frame_height);

/*
 * The ATKQR library expects an 8-bit grayscale image buffer, one byte per pixel.
 * Do not pass the current RGB565 camera frame directly; convert RGB565 to
 * grayscale before calling this function.
 */
payment_qr_status_t payment_qr_process_frame(const uint8_t *gray_image,
                                             uint16_t width,
                                             uint16_t height);

const char *payment_qr_get_last_text(void);
uint8_t payment_qr_is_valid(void);
payment_qr_status_t payment_qr_get_status(void);
uint8_t payment_qr_get_last_decode_result(void);
void payment_qr_deinit(void);

#endif
