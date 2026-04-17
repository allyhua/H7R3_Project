#ifndef __CAMERA_WORKFLOW_H
#define __CAMERA_WORKFLOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "app_x-cube-ai.h"

typedef enum
{
    CAMERA_TRIGGER_EMPTY = 0,
    CAMERA_TRIGGER_DETECT_PLACEMENT,
    CAMERA_TRIGGER_WAIT_STABLE,
    CAMERA_TRIGGER_RECOGNIZING,
    CAMERA_TRIGGER_WAIT_REMOVE
} camera_trigger_state_t;

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
    uint32_t total_flow_time_ms_x10;
    uint8_t total_flow_time_valid;
    uint32_t stable_weight_gram;
    uint32_t last_sample_weight_gram;
    uint32_t stable_start_tick_ms;
    uint32_t last_weight_sample_tick_ms;
    uint32_t flow_start_cycles;
    uint32_t pending_image_refresh_tick_ms;
    uint8_t pending_image_refresh;
    camera_trigger_state_t trigger_state;
    char status_text[48];
} camera_workflow_state_t;

int camera_workflow_init(camera_workflow_state_t *state);
void camera_workflow_handle_capture(camera_workflow_state_t *state);
void camera_workflow_process(camera_workflow_state_t *state);

#ifdef __cplusplus
}
#endif

#endif
