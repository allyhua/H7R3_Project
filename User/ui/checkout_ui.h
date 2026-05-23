#ifndef __CHECKOUT_UI_H
#define __CHECKOUT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app/cart_service.h"
#include "lvgl.h"

#define CHECKOUT_UI_SCREEN_W        480
#define CHECKOUT_UI_SCREEN_H        800
#define CHECKOUT_UI_PAGE_MARGIN     15
#define CHECKOUT_UI_CARD_GAP        10
#define CHECKOUT_UI_HEADER_H        60
#define CHECKOUT_UI_BUTTON_H        70
#define CHECKOUT_UI_BUTTON_RADIUS   10
#define CHECKOUT_UI_CARD_RADIUS     8

#define CHECKOUT_UI_COLOR_PAGE_BG       0xF3F3F3
#define CHECKOUT_UI_COLOR_HEADER_BG     0x144B5C
#define CHECKOUT_UI_COLOR_CARD_BG       0xFFFFFF
#define CHECKOUT_UI_COLOR_TEXT_MAIN     0x222222
#define CHECKOUT_UI_COLOR_TEXT_SUB      0x666666
#define CHECKOUT_UI_COLOR_LINE          0xE0E0E0
#define CHECKOUT_UI_COLOR_AMOUNT_RED    0xF44336
#define CHECKOUT_UI_COLOR_BTN_BLUE      0x2D8CFF
#define CHECKOUT_UI_COLOR_BTN_GREEN     0x4CAF50
#define CHECKOUT_UI_COLOR_BTN_RED       0xF44336
#define CHECKOUT_UI_COLOR_BTN_ORANGE    0xF59E0B
#define CHECKOUT_UI_COLOR_POPUP_MASK    0x000000

typedef enum
{
    CHECKOUT_UI_BUTTON_BLUE = 0,
    CHECKOUT_UI_BUTTON_GREEN,
    CHECKOUT_UI_BUTTON_RED,
    CHECKOUT_UI_BUTTON_ORANGE
} checkout_ui_button_color_t;

typedef void (*checkout_ui_cart_action_cb_t)(void *user_data);
typedef const cart_context_t *(*checkout_ui_cart_get_cb_t)(void *user_data);
typedef void (*checkout_ui_payment_action_cb_t)(void *user_data);

typedef struct
{
    lv_obj_t *root;
    lv_obj_t *main_page;
    lv_obj_t *cart_page;
    lv_obj_t *payment_page;
    lv_obj_t *payment_success_page;
    lv_obj_t *popup_mask;
    lv_obj_t *recog_fail_popup;
    lv_obj_t *payment_invalid_popup;
    lv_obj_t *payment_timeout_popup;
    lv_obj_t *delete_confirm_popup;
    lv_obj_t *clear_confirm_popup;
    lv_obj_t *checkout_done_popup;
} checkout_ui_page_handles_t;

typedef struct
{
    lv_obj_t *header;
    lv_obj_t *header_title_label;
    lv_obj_t *header_icon_slot;
    lv_obj_t *weight_card;
    lv_obj_t *weight_value_label;
    lv_obj_t *time_value_label;
    lv_obj_t *image_card;
    lv_obj_t *image_box;
    lv_obj_t *preview_image;
    lv_obj_t *image_placeholder_icon;
    lv_obj_t *image_placeholder_label;
    lv_obj_t *result_card;
    lv_obj_t *result_state_label;
    lv_obj_t *result_product_value_label;
    lv_obj_t *result_price_value_label;
    lv_obj_t *result_amount_value_label;
    lv_obj_t *result_subtotal_value_label;
    lv_obj_t *summary_card;
    lv_obj_t *cart_count_label;
    lv_obj_t *total_value_label;
    lv_obj_t *total_unit_label;
    lv_obj_t *cart_button;
    lv_obj_t *checkout_button;
    lv_obj_t *clear_button;
    lv_obj_t *continue_button;
} checkout_ui_main_page_handles_t;

void checkout_ui_style_init(void);
void checkout_ui_framework_init(void);
checkout_ui_page_handles_t *checkout_ui_get_pages(void);
checkout_ui_main_page_handles_t *checkout_ui_get_main_page(void);

lv_obj_t *checkout_ui_create_header(lv_obj_t *parent, const char *title);
lv_obj_t *checkout_ui_create_card(lv_obj_t *parent,
                                  lv_coord_t x,
                                  lv_coord_t y,
                                  lv_coord_t w,
                                  lv_coord_t h);
lv_obj_t *checkout_ui_create_button(lv_obj_t *parent,
                                    const char *text,
                                    checkout_ui_button_color_t color);
lv_obj_t *checkout_ui_create_popup_panel(lv_obj_t *parent,
                                         lv_coord_t w,
                                         lv_coord_t h);
lv_obj_t *checkout_ui_create_title_label(lv_obj_t *parent, const char *text);
lv_obj_t *checkout_ui_create_body_label(lv_obj_t *parent, const char *text);
lv_obj_t *checkout_ui_create_amount_label(lv_obj_t *parent, const char *text);

void checkout_ui_show_main_page(void);
void checkout_ui_show_cart_page(void);
void checkout_ui_show_payment_page(uint32_t total_price_cent);
void checkout_ui_show_payment_success_page(const char *order_no,
                                           uint32_t total_price_cent,
                                           const char *pay_time);
void checkout_ui_set_payment_scanning(void);
void checkout_ui_set_payment_recognizing(void);
void checkout_ui_set_payment_success(void);
void checkout_ui_set_payment_invalid(void);
void checkout_ui_reset_payment_camera_preview(void);
void checkout_ui_payment_set_camera_source(uint16_t width, uint16_t height, const uint16_t *pixels);
void checkout_ui_update_payment_amount(uint32_t total_price_cent);
void checkout_ui_update_order_no(const char *order_no);
const char *checkout_ui_get_payment_order_no(void);
void checkout_ui_show_payment_invalid_popup(void);
void checkout_ui_show_payment_timeout_popup(void);
void checkout_ui_show_recog_fail_popup(void);
void checkout_ui_show_delete_confirm_popup(void);
void checkout_ui_show_clear_confirm_popup(void);
void checkout_ui_show_checkout_done_popup(uint32_t total_price_cent);
void checkout_ui_hide_popup(void);
void checkout_ui_set_cart_action_callbacks(checkout_ui_cart_action_cb_t delete_selected_cb,
                                           checkout_ui_cart_action_cb_t clear_cart_cb,
                                           checkout_ui_cart_action_cb_t checkout_done_cb,
                                           checkout_ui_cart_get_cb_t get_cart_cb,
                                           void *user_data);
void checkout_ui_set_payment_action_callbacks(checkout_ui_payment_action_cb_t start_cb,
                                              checkout_ui_payment_action_cb_t cancel_cb,
                                              checkout_ui_payment_action_cb_t rescan_cb,
                                              void *user_data);
void checkout_ui_refresh_main_summary(const cart_context_t *cart);
void checkout_ui_refresh_cart_page(const cart_context_t *cart);
void checkout_ui_refresh_all(const cart_context_t *cart);

void checkout_ui_main_set_status(const char *status_text);
void checkout_ui_main_set_weight_gram(uint32_t weight_gram);
void checkout_ui_main_set_recognition_time(uint8_t valid, uint32_t time_ms_x10);
void checkout_ui_main_set_result_state(const char *state_text);
void checkout_ui_main_set_result(const char *product,
                                 const char *price,
                                 const char *amount,
                                 const char *subtotal);
void checkout_ui_main_clear_result(void);
void checkout_ui_main_set_cart_summary(uint32_t item_type_count);
void checkout_ui_main_set_total_price(uint32_t total_price_cent);
void checkout_ui_main_set_image_source(uint16_t width, uint16_t height, const uint16_t *pixels);
void checkout_ui_main_show_image(uint8_t visible);
void checkout_ui_cart_refresh(const cart_context_t *cart);
int32_t checkout_ui_cart_get_selected_index(void);

#ifdef __cplusplus
}
#endif

#endif
