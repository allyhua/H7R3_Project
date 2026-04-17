#ifndef __CAMERA_WORKFLOW_H
#define __CAMERA_WORKFLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "app_x-cube-ai.h"

typedef struct
{
    uint16_t frame_width;
    uint16_t frame_height;
    uint16_t display_yoffset;
    uint16_t roi_x;
    uint16_t roi_y;
    uint16_t roi_size;
    app_ai_result_t last_result;
    uint8_t last_result_valid;
    uint32_t inference_counter;
    uint32_t weight_gram;
    uint8_t weight_valid;
    uint32_t recognition_time_ms_x10;
    uint8_t recognition_time_valid;
} camera_workflow_state_t;

int camera_workflow_init(camera_workflow_state_t *state);
void camera_workflow_handle_capture(camera_workflow_state_t *state);

#ifdef __cplusplus
}
#endif

#endif
