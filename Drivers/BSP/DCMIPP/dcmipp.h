/**
 ****************************************************************************************************
 * @file        dcmipp.h
 * @brief       DCMIPP driver
 ****************************************************************************************************
 */
#ifndef __DCMIPP_H
#define __DCMIPP_H
#include "./SYSTEM/sys/sys.h"
extern DCMIPP_HandleTypeDef g_dcmipp_handle;
extern __ALIGNED(16) uint32_t camera_date_buf[1024 * 1024 * 2 / 4];
void dcmipp_init(void);
void dcmipp_start(void);
void dcmipp_stop(void);
uint8_t dcmipp_wait_frame(uint8_t frame_mark, uint32_t timeout_ms);
void dcmipp_invalidate_frame_cache(void);
#endif
