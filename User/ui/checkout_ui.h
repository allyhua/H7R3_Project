#ifndef __CHECKOUT_UI_H
#define __CHECKOUT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app/camera_workflow.h"

void checkout_ui_init(camera_workflow_state_t *state);
void checkout_ui_set_status(const char *status_text);
void checkout_ui_set_weight_info(uint32_t weight_gram,
                                 uint8_t latency_valid,
                                 uint32_t recognition_time_ms_x10);
void checkout_ui_set_recognition_result(const cart_add_result_t *result);
void checkout_ui_clear_recognition_result(void);
void checkout_ui_refresh_cart(const cart_context_t *cart, int32_t selected_index);
void checkout_ui_set_total_price(uint32_t total_price_cent);
void checkout_ui_show_error(const char *title, const char *message);
void checkout_ui_show_checkout_done(uint32_t total_price_cent);

#ifdef __cplusplus
}
#endif

#endif
