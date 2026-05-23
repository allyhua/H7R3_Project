/**
 ****************************************************************************************************
 * @file        dcmipp.h
 * @brief       DCMIPP driver
 ****************************************************************************************************
 */
#ifndef __DCMIPP_H
#define __DCMIPP_H
#include "./SYSTEM/sys/sys.h"

#define DCMIPP_CAPTURE_WIDTH          (480UL)
#define DCMIPP_CAPTURE_HEIGHT         (360UL)
#define DCMIPP_CAPTURE_BYTES_PER_PIXEL (2UL)
#define DCMIPP_CAPTURE_CACHE_BYTES    (DCMIPP_CAPTURE_WIDTH * DCMIPP_CAPTURE_HEIGHT * DCMIPP_CAPTURE_BYTES_PER_PIXEL)
#define DCMIPP_CAMERA_BUFFER_BYTES    (1024UL * 1024UL * 2UL)

extern DCMIPP_HandleTypeDef g_dcmipp_handle;
extern __ALIGNED(16) uint32_t camera_date_buf[DCMIPP_CAMERA_BUFFER_BYTES / 4UL];
void dcmipp_init(void);
int dcmipp_start(void);
int dcmipp_start_continuous(void);
void dcmipp_stop(void);
uint8_t dcmipp_wait_frame(uint8_t frame_mark, uint32_t timeout_ms);
void dcmipp_invalidate_frame_cache(void);
#endif
