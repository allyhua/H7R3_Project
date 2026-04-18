#ifndef __UI_CHECKOUT_MAIN_H
#define __UI_CHECKOUT_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app/camera_workflow.h"

void ui_checkout_main_init(camera_workflow_state_t *state);
void ui_checkout_main_refresh(camera_workflow_state_t *state);

#ifdef __cplusplus
}
#endif

#endif
