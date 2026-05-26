
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_AI_H
#define __APP_AI_H
#ifdef __cplusplus
extern "C" {
#endif
/**
  ******************************************************************************
  * @file    app_x-cube-ai.h
  * @author  X-CUBE-AI C code generator
  * @brief   AI entry function definitions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ai_platform.h"
#include <stdint.h>

typedef struct
{
    uint32_t top1_index;
    float top1_probability;
    float top1_logit;
} app_ai_result_t;

void MX_X_CUBE_AI_Init(void);
void MX_X_CUBE_AI_Process(void);
int APP_AI_RunInferenceOnRgb565Frame(const uint16_t *rgb565_frame,
                                     uint16_t frame_width,
                                     uint16_t frame_height,
                                     const char *sample_name);
int APP_AI_ClassifyRgb565Roi(const uint16_t *rgb565_frame,
                             uint16_t frame_width,
                             uint16_t frame_height,
                             uint16_t roi_x,
                             uint16_t roi_y,
                             uint16_t roi_width,
                             uint16_t roi_height,
                             const char *sample_name,
                             app_ai_result_t *out_result);
/* USER CODE BEGIN includes */
/* USER CODE END includes */
#ifdef __cplusplus
}
#endif
#endif /*__STMicroelectronics_X-CUBE-AI_10_2_0_H */
