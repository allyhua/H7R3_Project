#ifndef __UI_CHECKOUT_MAIN_H
#define __UI_CHECKOUT_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui/checkout_ui.h"
#include "app/camera_workflow.h"

void ui_checkout_main_init(camera_workflow_state_t *state);
void ui_checkout_main_refresh(camera_workflow_state_t *state);
void ui_checkout_refresh_status(camera_workflow_state_t *state);
void ui_checkout_refresh_result(camera_workflow_state_t *state);
void ui_checkout_refresh_cart(camera_workflow_state_t *state);
void ui_checkout_refresh_total(camera_workflow_state_t *state);
void ui_checkout_show_recognize_fail(const char *message);
void ui_checkout_show_checkout_done(uint32_t total_price_cent);

#ifdef __cplusplus
}
#endif

#endif
