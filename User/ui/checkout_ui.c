#include "ui/checkout_ui.h"
#include "ui/checkout_icons.h"

#include <stdio.h>
#include <stdint.h>

LV_FONT_DECLARE(Font20);
LV_FONT_DECLARE(lv_font_montserrat_32);

static checkout_ui_page_handles_t g_checkout_ui_pages;
static checkout_ui_main_page_handles_t g_checkout_ui_main;
static lv_img_dsc_t g_checkout_ui_preview_dsc;
static lv_img_dsc_t g_checkout_ui_payment_preview_dsc;
static uint8_t g_style_ready;

typedef enum
{
    CHECKOUT_UI_PAYMENT_PREV_MAIN = 0,
    CHECKOUT_UI_PAYMENT_PREV_CART
} checkout_ui_payment_prev_page_t;

typedef enum
{
    CHECKOUT_UI_PAYMENT_SCANNING = 0,
    CHECKOUT_UI_PAYMENT_RECOGNIZING,
    CHECKOUT_UI_PAYMENT_SUCCESS,
    CHECKOUT_UI_PAYMENT_INVALID
} checkout_ui_payment_state_t;

typedef struct
{
    lv_obj_t *header;
    lv_obj_t *back_button;
    lv_obj_t *clear_top_button;
    lv_obj_t *summary_card;
    lv_obj_t *summary_count_label;
    lv_obj_t *summary_total_label;
    lv_obj_t *summary_total_unit_label;
    lv_obj_t *list_card;
    lv_obj_t *list_container;
    lv_obj_t *empty_label;
    lv_obj_t *hint_label;
    lv_obj_t *delete_button;
    lv_obj_t *clear_button;
    lv_obj_t *checkout_button;
    lv_obj_t *item_row[PRODUCT_ID_COUNT];
    lv_obj_t *item_check[PRODUCT_ID_COUNT];
    lv_obj_t *item_name_label[PRODUCT_ID_COUNT];
    lv_obj_t *item_price_label[PRODUCT_ID_COUNT];
    lv_obj_t *item_amount_label[PRODUCT_ID_COUNT];
    lv_obj_t *item_subtotal_label[PRODUCT_ID_COUNT];
} checkout_ui_cart_page_handles_t;

typedef struct
{
    lv_obj_t *header;
    lv_obj_t *back_button;
    lv_obj_t *cancel_top_button;
    lv_obj_t *amount_card;
    lv_obj_t *amount_prefix_label;
    lv_obj_t *amount_value_label;
    lv_obj_t *order_no_label;
    lv_obj_t *scan_card;
    lv_obj_t *scan_title_label;
    lv_obj_t *scan_frame;
    lv_obj_t *scan_preview_image;
    lv_obj_t *scan_qr_panel;
    lv_obj_t *scan_line;
    lv_obj_t *scan_corner[4];
    lv_obj_t *success_badge;
    lv_obj_t *success_check_label;
    lv_obj_t *hint_card;
    lv_obj_t *status_card;
    lv_obj_t *status_icon_bg;
    lv_obj_t *status_icon_label;
    lv_obj_t *status_title_label;
    lv_obj_t *status_body_label;
    lv_obj_t *cancel_button;
    lv_obj_t *rescan_button;
} checkout_ui_payment_page_handles_t;

typedef struct
{
    lv_obj_t *success_icon_bg;
    lv_obj_t *success_icon_label;
    lv_obj_t *title_label;
    lv_obj_t *order_no_value_label;
    lv_obj_t *amount_value_label;
    lv_obj_t *method_value_label;
    lv_obj_t *time_value_label;
    lv_obj_t *thanks_label;
    lv_obj_t *done_label;
    lv_obj_t *home_button;
} checkout_ui_payment_success_page_handles_t;

typedef struct
{
    lv_obj_t *panel;
    lv_obj_t *icon_bg;
    lv_obj_t *icon_label;
    lv_obj_t *title_label;
    lv_obj_t *body_label;
    lv_obj_t *cancel_button;
    lv_obj_t *confirm_button;
} checkout_ui_confirm_popup_handles_t;

typedef struct
{
    lv_obj_t *panel;
    lv_obj_t *icon_bg;
    lv_obj_t *icon_label;
    lv_obj_t *title_label;
    lv_obj_t *body_label;
    lv_obj_t *amount_prefix_label;
    lv_obj_t *amount_value_label;
    lv_obj_t *amount_unit_label;
    lv_obj_t *ok_button;
} checkout_ui_checkout_done_popup_handles_t;

typedef struct
{
    lv_obj_t *panel;
    lv_obj_t *icon_bg;
    lv_obj_t *icon_label;
    lv_obj_t *title_label;
    lv_obj_t *body_label;
    lv_obj_t *ok_button;
} checkout_ui_recog_fail_popup_handles_t;

typedef struct
{
    lv_obj_t *panel;
    lv_obj_t *icon_bg;
    lv_obj_t *icon_label;
    lv_obj_t *title_label;
    lv_obj_t *body_label;
    lv_obj_t *ok_button;
} checkout_ui_payment_invalid_popup_handles_t;

typedef struct
{
    lv_obj_t *panel;
    lv_obj_t *icon_bg;
    lv_obj_t *icon_label;
    lv_obj_t *title_label;
    lv_obj_t *body_label;
    lv_obj_t *cancel_button;
    lv_obj_t *rescan_button;
} checkout_ui_payment_timeout_popup_handles_t;

static checkout_ui_cart_page_handles_t g_checkout_ui_cart;
static checkout_ui_payment_page_handles_t g_checkout_ui_payment;
static checkout_ui_payment_success_page_handles_t g_checkout_ui_payment_success;
static checkout_ui_recog_fail_popup_handles_t g_checkout_ui_recog_fail;
static checkout_ui_payment_invalid_popup_handles_t g_checkout_ui_payment_invalid;
static checkout_ui_payment_timeout_popup_handles_t g_checkout_ui_payment_timeout;
static checkout_ui_confirm_popup_handles_t g_checkout_ui_delete_confirm;
static checkout_ui_confirm_popup_handles_t g_checkout_ui_clear_confirm;
static checkout_ui_checkout_done_popup_handles_t g_checkout_ui_checkout_done;
static int32_t g_checkout_ui_cart_selected_index = -1;
static checkout_ui_cart_action_cb_t g_checkout_ui_delete_selected_cb;
static checkout_ui_cart_action_cb_t g_checkout_ui_clear_cart_cb;
static checkout_ui_cart_action_cb_t g_checkout_ui_checkout_done_cb;
static checkout_ui_cart_get_cb_t g_checkout_ui_get_cart_cb;
static void *g_checkout_ui_cart_action_user_data;
static checkout_ui_payment_action_cb_t g_checkout_ui_payment_start_cb;
static checkout_ui_payment_action_cb_t g_checkout_ui_payment_cancel_cb;
static checkout_ui_payment_action_cb_t g_checkout_ui_payment_rescan_cb;
static void *g_checkout_ui_payment_action_user_data;
static checkout_ui_payment_prev_page_t g_checkout_ui_payment_prev_page;
static checkout_ui_payment_state_t g_checkout_ui_payment_state = CHECKOUT_UI_PAYMENT_SCANNING;
static uint32_t g_checkout_ui_payment_order_seq;
static char g_checkout_ui_payment_order_no[20] = "20260513-001";
static uint8_t g_checkout_ui_main_built;
static uint8_t g_checkout_ui_cart_built;
static uint8_t g_checkout_ui_payment_built;
static uint8_t g_checkout_ui_payment_success_built;

static lv_style_t g_style_screen;
static lv_style_t g_style_header;
static lv_style_t g_style_card;
static lv_style_t g_style_button_blue;
static lv_style_t g_style_button_green;
static lv_style_t g_style_button_red;
static lv_style_t g_style_button_orange;
static lv_style_t g_style_popup_mask;
static lv_style_t g_style_popup_panel;
static lv_style_t g_style_title_text;
static lv_style_t g_style_body_text;
static lv_style_t g_style_amount_text;
static lv_style_t g_style_hint_text;
static lv_style_t g_style_state_badge;

static lv_obj_t *checkout_ui_create_page(lv_obj_t *parent);
static void checkout_ui_ensure_cart_page(void);
static void checkout_ui_ensure_payment_page(void);
static void checkout_ui_ensure_payment_success_page(void);
static void checkout_ui_ensure_recog_fail_popup(void);
static void checkout_ui_ensure_payment_invalid_popup(void);
static void checkout_ui_ensure_payment_timeout_popup(void);
static void checkout_ui_ensure_delete_confirm_popup(void);
static void checkout_ui_ensure_clear_confirm_popup(void);
static void checkout_ui_ensure_checkout_done_popup(void);
static void checkout_ui_build_main_page(void);
static lv_obj_t *checkout_ui_create_icon(lv_obj_t *parent, const lv_img_dsc_t *icon);
static lv_obj_t *checkout_ui_create_icon_button(lv_obj_t *parent,
                                                const char *text,
                                                const lv_img_dsc_t *icon,
                                                checkout_ui_button_color_t color);
static lv_obj_t *checkout_ui_create_cart_action_button(lv_obj_t *parent,
                                                       const char *text,
                                                       checkout_ui_button_color_t color);
static void checkout_ui_create_info_item(lv_obj_t *parent,
                                         lv_coord_t x,
                                         const lv_img_dsc_t *icon,
                                         const char *title,
                                         const char *value,
                                         lv_obj_t **value_label);
static void checkout_ui_build_cart_page(void);
static void checkout_ui_build_payment_page(void);
static void checkout_ui_build_payment_success_page(void);
static void checkout_ui_cart_item_event_cb(lv_event_t *e);
static void checkout_ui_cart_back_event_cb(lv_event_t *e);
static void checkout_ui_cart_delete_event_cb(lv_event_t *e);
static void checkout_ui_cart_clear_event_cb(lv_event_t *e);
static void checkout_ui_cart_checkout_event_cb(lv_event_t *e);
static void checkout_ui_main_cart_event_cb(lv_event_t *e);
static void checkout_ui_popup_cancel_event_cb(lv_event_t *e);
static void checkout_ui_delete_confirm_event_cb(lv_event_t *e);
static void checkout_ui_clear_confirm_event_cb(lv_event_t *e);
static void checkout_ui_main_continue_event_cb(lv_event_t *e);
static void checkout_ui_main_checkout_event_cb(lv_event_t *e);
static void checkout_ui_payment_back_event_cb(lv_event_t *e);
static void checkout_ui_payment_rescan_event_cb(lv_event_t *e);
static void checkout_ui_payment_invalid_ok_event_cb(lv_event_t *e);
static void checkout_ui_payment_timeout_cancel_event_cb(lv_event_t *e);
static void checkout_ui_payment_timeout_rescan_event_cb(lv_event_t *e);
static void checkout_ui_payment_success_home_event_cb(lv_event_t *e);
static void checkout_ui_checkout_done_ok_event_cb(lv_event_t *e);
static void checkout_ui_build_recog_fail_popup(lv_obj_t *panel);
static void checkout_ui_build_payment_invalid_popup(lv_obj_t *panel);
static void checkout_ui_build_payment_timeout_popup(lv_obj_t *panel);
static void checkout_ui_build_confirm_popup(checkout_ui_confirm_popup_handles_t *handles,
                                            lv_obj_t *panel,
                                            const char *title,
                                            const char *body,
                                            lv_event_cb_t confirm_event_cb);
static void checkout_ui_build_checkout_done_popup(lv_obj_t *panel);
static void checkout_ui_request_checkout(void);
static void checkout_ui_show_empty_checkout_popup(void);
static void checkout_ui_cart_set_selected_index(int32_t selected_index);
static void checkout_ui_cart_update_selection(void);
static lv_obj_t *checkout_ui_create_value_row(lv_obj_t *parent,
                                              lv_coord_t y,
                                              const char *name,
                                              const char *value);
static void checkout_ui_hide_all_pages(void);
static void checkout_ui_return_from_payment_page(void);
static void checkout_ui_payment_set_state(checkout_ui_payment_state_t state);
static void checkout_ui_payment_set_button_disabled(lv_obj_t *button, uint8_t disabled);
static lv_obj_t *checkout_ui_create_payment_text_button(lv_obj_t *parent,
                                                        const char *text,
                                                        uint32_t color);
static void checkout_ui_create_payment_corner(lv_obj_t *parent,
                                              lv_obj_t **corner,
                                              lv_coord_t x,
                                              lv_coord_t y,
                                              uint8_t right,
                                              uint8_t bottom);
static void checkout_ui_create_payment_qr_mock(lv_obj_t *parent);
static void checkout_ui_hide_all_popups(void);
static void checkout_ui_show_popup(lv_obj_t *popup);
static lv_style_t *checkout_ui_button_style(checkout_ui_button_color_t color);
static void checkout_ui_empty_button_event_cb(lv_event_t *e);
static void checkout_ui_format_money(uint32_t cent, char *buffer, uint32_t size);

void checkout_ui_style_init(void)
{
    if (g_style_ready != 0U)
    {
        return;
    }

    lv_style_init(&g_style_screen);
    lv_style_set_bg_color(&g_style_screen, lv_color_hex(CHECKOUT_UI_COLOR_PAGE_BG));
    lv_style_set_bg_opa(&g_style_screen, LV_OPA_COVER);
    lv_style_set_pad_all(&g_style_screen, 0);

    lv_style_init(&g_style_header);
    lv_style_set_bg_color(&g_style_header, lv_color_hex(CHECKOUT_UI_COLOR_HEADER_BG));
    lv_style_set_bg_opa(&g_style_header, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_header, 0);
    lv_style_set_radius(&g_style_header, 0);
    lv_style_set_pad_all(&g_style_header, 0);
    lv_style_set_text_color(&g_style_header, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_text_font(&g_style_header, &Font20);

    lv_style_init(&g_style_card);
    lv_style_set_bg_color(&g_style_card, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_bg_opa(&g_style_card, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_card, 1);
    lv_style_set_border_color(&g_style_card, lv_color_hex(CHECKOUT_UI_COLOR_LINE));
    lv_style_set_radius(&g_style_card, CHECKOUT_UI_CARD_RADIUS);
    lv_style_set_pad_all(&g_style_card, 0);

    lv_style_init(&g_style_button_blue);
    lv_style_set_bg_color(&g_style_button_blue, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE));
    lv_style_set_bg_opa(&g_style_button_blue, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_button_blue, 0);
    lv_style_set_radius(&g_style_button_blue, CHECKOUT_UI_BUTTON_RADIUS);
    lv_style_set_text_color(&g_style_button_blue, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_text_font(&g_style_button_blue, &Font20);

    lv_style_init(&g_style_button_green);
    lv_style_set_bg_color(&g_style_button_green, lv_color_hex(CHECKOUT_UI_COLOR_BTN_GREEN));
    lv_style_set_bg_opa(&g_style_button_green, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_button_green, 0);
    lv_style_set_radius(&g_style_button_green, CHECKOUT_UI_BUTTON_RADIUS);
    lv_style_set_text_color(&g_style_button_green, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_text_font(&g_style_button_green, &Font20);

    lv_style_init(&g_style_button_red);
    lv_style_set_bg_color(&g_style_button_red, lv_color_hex(CHECKOUT_UI_COLOR_BTN_RED));
    lv_style_set_bg_opa(&g_style_button_red, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_button_red, 0);
    lv_style_set_radius(&g_style_button_red, CHECKOUT_UI_BUTTON_RADIUS);
    lv_style_set_text_color(&g_style_button_red, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_text_font(&g_style_button_red, &Font20);

    lv_style_init(&g_style_button_orange);
    lv_style_set_bg_color(&g_style_button_orange, lv_color_hex(CHECKOUT_UI_COLOR_BTN_ORANGE));
    lv_style_set_bg_opa(&g_style_button_orange, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_button_orange, 0);
    lv_style_set_radius(&g_style_button_orange, CHECKOUT_UI_BUTTON_RADIUS);
    lv_style_set_text_color(&g_style_button_orange, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_text_font(&g_style_button_orange, &Font20);

    lv_style_init(&g_style_popup_mask);
    lv_style_set_bg_color(&g_style_popup_mask, lv_color_hex(CHECKOUT_UI_COLOR_POPUP_MASK));
    lv_style_set_bg_opa(&g_style_popup_mask, LV_OPA_50);
    lv_style_set_border_width(&g_style_popup_mask, 0);
    lv_style_set_radius(&g_style_popup_mask, 0);
    lv_style_set_pad_all(&g_style_popup_mask, 0);

    lv_style_init(&g_style_popup_panel);
    lv_style_set_bg_color(&g_style_popup_panel, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_bg_opa(&g_style_popup_panel, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_popup_panel, 0);
    lv_style_set_radius(&g_style_popup_panel, CHECKOUT_UI_CARD_RADIUS);
    lv_style_set_pad_all(&g_style_popup_panel, CHECKOUT_UI_CARD_GAP);

    lv_style_init(&g_style_title_text);
    lv_style_set_text_color(&g_style_title_text, lv_color_hex(CHECKOUT_UI_COLOR_TEXT_MAIN));
    lv_style_set_text_font(&g_style_title_text, &Font20);

    lv_style_init(&g_style_body_text);
    lv_style_set_text_color(&g_style_body_text, lv_color_hex(CHECKOUT_UI_COLOR_TEXT_SUB));
    lv_style_set_text_font(&g_style_body_text, &Font20);

    lv_style_init(&g_style_amount_text);
    lv_style_set_text_color(&g_style_amount_text, lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED));
    lv_style_set_text_font(&g_style_amount_text, &lv_font_montserrat_32);

    lv_style_init(&g_style_hint_text);
    lv_style_set_text_color(&g_style_hint_text, lv_color_hex(0x999999));
    lv_style_set_text_font(&g_style_hint_text, &Font20);

    lv_style_init(&g_style_state_badge);
    lv_style_set_bg_color(&g_style_state_badge, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG));
    lv_style_set_bg_opa(&g_style_state_badge, LV_OPA_COVER);
    lv_style_set_border_width(&g_style_state_badge, 1);
    lv_style_set_border_color(&g_style_state_badge, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE));
    lv_style_set_radius(&g_style_state_badge, 6);
    lv_style_set_pad_all(&g_style_state_badge, 0);
    lv_style_set_text_color(&g_style_state_badge, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE));
    lv_style_set_text_font(&g_style_state_badge, &Font20);

    g_style_ready = 1U;
}

void checkout_ui_framework_init(void)
{
    checkout_ui_style_init();

    if (g_checkout_ui_pages.root != NULL)
    {
        return;
    }

    g_checkout_ui_pages.root = lv_obj_create(NULL);
    lv_obj_set_size(g_checkout_ui_pages.root, CHECKOUT_UI_SCREEN_W, CHECKOUT_UI_SCREEN_H);
    lv_obj_add_style(g_checkout_ui_pages.root, &g_style_screen, 0);
    lv_obj_clear_flag(g_checkout_ui_pages.root, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_pages.main_page = checkout_ui_create_page(g_checkout_ui_pages.root);
    g_checkout_ui_pages.cart_page = checkout_ui_create_page(g_checkout_ui_pages.root);
    g_checkout_ui_pages.payment_page = checkout_ui_create_page(g_checkout_ui_pages.root);
    g_checkout_ui_pages.payment_success_page = checkout_ui_create_page(g_checkout_ui_pages.root);
    checkout_ui_build_main_page();
    g_checkout_ui_main_built = 1U;

    g_checkout_ui_pages.popup_mask = lv_obj_create(g_checkout_ui_pages.root);
    lv_obj_set_size(g_checkout_ui_pages.popup_mask, CHECKOUT_UI_SCREEN_W, CHECKOUT_UI_SCREEN_H);
    lv_obj_add_style(g_checkout_ui_pages.popup_mask, &g_style_popup_mask, 0);
    lv_obj_add_flag(g_checkout_ui_pages.popup_mask, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(g_checkout_ui_pages.popup_mask, LV_OBJ_FLAG_SCROLLABLE);


    checkout_ui_show_main_page();
}

checkout_ui_page_handles_t *checkout_ui_get_pages(void)
{
    return &g_checkout_ui_pages;
}

checkout_ui_main_page_handles_t *checkout_ui_get_main_page(void)
{
    return &g_checkout_ui_main;
}

lv_obj_t *checkout_ui_create_header(lv_obj_t *parent, const char *title)
{
    lv_obj_t *header;
    lv_obj_t *label;

    checkout_ui_style_init();

    header = lv_obj_create(parent);
    lv_obj_set_size(header, CHECKOUT_UI_SCREEN_W, CHECKOUT_UI_HEADER_H);
    lv_obj_set_pos(header, 0, 0);
    lv_obj_add_style(header, &g_style_header, 0);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(header);
    lv_label_set_text(label, (title != NULL) ? title : "");
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_center(label);

    return header;
}

lv_obj_t *checkout_ui_create_card(lv_obj_t *parent,
                                  lv_coord_t x,
                                  lv_coord_t y,
                                  lv_coord_t w,
                                  lv_coord_t h)
{
    lv_obj_t *card;

    checkout_ui_style_init();

    card = lv_obj_create(parent);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_size(card, w, h);
    lv_obj_add_style(card, &g_style_card, 0);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    return card;
}

lv_obj_t *checkout_ui_create_button(lv_obj_t *parent,
                                    const char *text,
                                    checkout_ui_button_color_t color)
{
    lv_obj_t *button;
    lv_obj_t *label;

    checkout_ui_style_init();

    button = lv_btn_create(parent);
    lv_obj_set_height(button, CHECKOUT_UI_BUTTON_H);
    lv_obj_add_style(button, checkout_ui_button_style(color), 0);

    label = lv_label_create(button);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);

    return button;
}

static lv_obj_t *checkout_ui_create_icon(lv_obj_t *parent, const lv_img_dsc_t *icon)
{
    lv_obj_t *image;

    image = lv_img_create(parent);
    lv_img_set_src(image, icon);
    lv_obj_clear_flag(image, LV_OBJ_FLAG_SCROLLABLE);

    return image;
}

static lv_obj_t *checkout_ui_create_icon_button(lv_obj_t *parent,
                                                const char *text,
                                                const lv_img_dsc_t *icon,
                                                checkout_ui_button_color_t color)
{
    lv_obj_t *button;
    lv_obj_t *image;
    lv_obj_t *label;

    checkout_ui_style_init();

    button = lv_btn_create(parent);
    lv_obj_set_height(button, CHECKOUT_UI_BUTTON_H);
    lv_obj_add_style(button, checkout_ui_button_style(color), 0);
    lv_obj_clear_flag(button, LV_OBJ_FLAG_SCROLLABLE);

    image = checkout_ui_create_icon(button, icon);
    lv_obj_align(image, LV_ALIGN_LEFT_MID, 24, 0);

    label = lv_label_create(button);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_width(label, 126);
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 76, 0);

    return button;
}

static lv_obj_t *checkout_ui_create_cart_action_button(lv_obj_t *parent,
                                                       const char *text,
                                                       checkout_ui_button_color_t color)
{
    lv_obj_t *button;
    lv_obj_t *label;

    checkout_ui_style_init();

    button = lv_btn_create(parent);
    lv_obj_add_style(button, checkout_ui_button_style(color), 0);
    lv_obj_clear_flag(button, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(button);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);

    return button;
}

lv_obj_t *checkout_ui_create_popup_panel(lv_obj_t *parent,
                                         lv_coord_t w,
                                         lv_coord_t h)
{
    lv_obj_t *panel;

    checkout_ui_style_init();

    panel = lv_obj_create(parent);
    lv_obj_set_size(panel, w, h);
    lv_obj_center(panel);
    lv_obj_add_style(panel, &g_style_popup_panel, 0);
    lv_obj_set_style_radius(panel, 12, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    return panel;
}

lv_obj_t *checkout_ui_create_title_label(lv_obj_t *parent, const char *text)
{
    lv_obj_t *label;

    checkout_ui_style_init();

    label = lv_label_create(parent);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_add_style(label, &g_style_title_text, 0);

    return label;
}

lv_obj_t *checkout_ui_create_body_label(lv_obj_t *parent, const char *text)
{
    lv_obj_t *label;

    checkout_ui_style_init();

    label = lv_label_create(parent);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_add_style(label, &g_style_body_text, 0);

    return label;
}

lv_obj_t *checkout_ui_create_amount_label(lv_obj_t *parent, const char *text)
{
    lv_obj_t *label;

    checkout_ui_style_init();

    label = lv_label_create(parent);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_add_style(label, &g_style_amount_text, 0);

    return label;
}

void checkout_ui_main_set_status(const char *status_text)
{
    checkout_ui_framework_init();

    if (g_checkout_ui_main.header_title_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.header_title_label, (status_text != NULL) ? status_text : "");
    }
}

void checkout_ui_main_set_weight_gram(uint32_t weight_gram)
{
    char text[24];

    checkout_ui_framework_init();
    (void)snprintf(text, sizeof(text), "%lu.%03lu kg",
                   (unsigned long)(weight_gram / 1000U),
                   (unsigned long)(weight_gram % 1000U));

    if (g_checkout_ui_main.weight_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.weight_value_label, text);
    }
}

void checkout_ui_main_set_recognition_time(uint8_t valid, uint32_t time_ms_x10)
{
    char text[24];
    uint32_t centisecond;

    checkout_ui_framework_init();

    if (valid == 0U)
    {
        (void)snprintf(text, sizeof(text), "--.-- s");
    }
    else
    {
        centisecond = time_ms_x10 / 100U;
        (void)snprintf(text, sizeof(text), "%lu.%02lu s",
                       (unsigned long)(centisecond / 100U),
                       (unsigned long)(centisecond % 100U));
    }

    if (g_checkout_ui_main.time_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.time_value_label, text);
    }
}

void checkout_ui_main_set_result_state(const char *state_text)
{
    checkout_ui_framework_init();

    if (g_checkout_ui_main.result_state_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.result_state_label, (state_text != NULL) ? state_text : "");
    }
}

void checkout_ui_main_set_result(const char *product,
                                 const char *price,
                                 const char *amount,
                                 const char *subtotal)
{
    checkout_ui_framework_init();

    if (g_checkout_ui_main.result_product_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.result_product_value_label, (product != NULL) ? product : "--");
    }

    if (g_checkout_ui_main.result_price_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.result_price_value_label, (price != NULL) ? price : "--");
    }

    if (g_checkout_ui_main.result_amount_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.result_amount_value_label, (amount != NULL) ? amount : "--");
    }

    if (g_checkout_ui_main.result_subtotal_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.result_subtotal_value_label, (subtotal != NULL) ? subtotal : "--");
    }
}

void checkout_ui_main_clear_result(void)
{
    checkout_ui_main_set_result("--", "--", "--", "--");
    checkout_ui_main_set_result_state("等待识别");
}

void checkout_ui_main_set_cart_summary(uint32_t item_type_count)
{
    char text[32];

    checkout_ui_framework_init();
    (void)snprintf(text, sizeof(text), "共 %lu 类商品", (unsigned long)item_type_count);

    if (g_checkout_ui_main.cart_count_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.cart_count_label, text);
    }
}

void checkout_ui_main_set_total_price(uint32_t total_price_cent)
{
    char text[24];

    checkout_ui_framework_init();
    checkout_ui_format_money(total_price_cent, text, sizeof(text));

    if (g_checkout_ui_main.total_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_main.total_value_label, text);
    }
}

void checkout_ui_main_set_image_source(uint16_t width, uint16_t height, const uint16_t *pixels)
{
    lv_coord_t box_w;
    lv_coord_t box_h;
    uint32_t zoom_x;
    uint32_t zoom_y;
    uint32_t zoom;

    checkout_ui_framework_init();

    if ((g_checkout_ui_main.preview_image == NULL) ||
        (g_checkout_ui_main.image_box == NULL) ||
        (pixels == NULL) ||
        (width == 0U) ||
        (height == 0U))
    {
        return;
    }

    g_checkout_ui_preview_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    g_checkout_ui_preview_dsc.header.w = width;
    g_checkout_ui_preview_dsc.header.h = height;
    g_checkout_ui_preview_dsc.data_size = (uint32_t)width * (uint32_t)height * 2U;
    g_checkout_ui_preview_dsc.data = (const uint8_t *)pixels;

    box_w = lv_obj_get_width(g_checkout_ui_main.image_box) - 8;
    box_h = lv_obj_get_height(g_checkout_ui_main.image_box) - 8;
    zoom_x = (uint32_t)box_w * 256U / width;
    zoom_y = (uint32_t)box_h * 256U / height;
    zoom = (zoom_x < zoom_y) ? zoom_x : zoom_y;

    if (zoom > 256U)
    {
        zoom = 256U;
    }

    if (zoom < 32U)
    {
        zoom = 32U;
    }

    lv_img_set_src(g_checkout_ui_main.preview_image, &g_checkout_ui_preview_dsc);
    lv_img_set_zoom(g_checkout_ui_main.preview_image, (uint16_t)zoom);
    lv_obj_center(g_checkout_ui_main.preview_image);
    checkout_ui_main_show_image(1U);
    lv_obj_move_foreground(g_checkout_ui_main.preview_image);
    lv_obj_invalidate(g_checkout_ui_main.image_box);
}

void checkout_ui_main_show_image(uint8_t visible)
{
    checkout_ui_framework_init();

    if (g_checkout_ui_main.preview_image == NULL)
    {
        return;
    }

    if (visible != 0U)
    {
        lv_obj_clear_flag(g_checkout_ui_main.preview_image, LV_OBJ_FLAG_HIDDEN);
        if (g_checkout_ui_main.image_placeholder_label != NULL)
        {
            lv_obj_add_flag(g_checkout_ui_main.image_placeholder_label, LV_OBJ_FLAG_HIDDEN);
        }
        if (g_checkout_ui_main.image_placeholder_icon != NULL)
        {
            lv_obj_add_flag(g_checkout_ui_main.image_placeholder_icon, LV_OBJ_FLAG_HIDDEN);
        }
    }
    else
    {
        lv_obj_add_flag(g_checkout_ui_main.preview_image, LV_OBJ_FLAG_HIDDEN);
        if (g_checkout_ui_main.image_placeholder_icon != NULL)
        {
            lv_obj_clear_flag(g_checkout_ui_main.image_placeholder_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_align(g_checkout_ui_main.image_placeholder_icon, LV_ALIGN_CENTER, 0, -24);
        }
        if (g_checkout_ui_main.image_placeholder_label != NULL)
        {
            lv_obj_clear_flag(g_checkout_ui_main.image_placeholder_label, LV_OBJ_FLAG_HIDDEN);
            lv_obj_align(g_checkout_ui_main.image_placeholder_label, LV_ALIGN_CENTER, 0, 42);
        }
    }
}

void checkout_ui_cart_refresh(const cart_context_t *cart)
{
    uint32_t idx;
    uint32_t y = 0U;
    char text[40];

    checkout_ui_framework_init();

    if ((cart == NULL) || (g_checkout_ui_cart.list_container == NULL))
    {
        return;
    }

    (void)snprintf(text, sizeof(text), "共 %lu 类商品", (unsigned long)cart->item_count);
    if (g_checkout_ui_cart.summary_count_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_cart.summary_count_label, text);
    }

    checkout_ui_format_money(cart->total_price_cent, text, sizeof(text));
    if (g_checkout_ui_cart.summary_total_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_cart.summary_total_label, text);
    }
    if (g_checkout_ui_cart.summary_total_unit_label != NULL)
    {
        lv_obj_align_to(g_checkout_ui_cart.summary_total_unit_label,
                        g_checkout_ui_cart.summary_total_label,
                        LV_ALIGN_OUT_RIGHT_MID,
                        6,
                        2);
    }

    if (cart->item_count == 0U)
    {
        g_checkout_ui_cart_selected_index = -1;
    }
    else if ((g_checkout_ui_cart_selected_index >= 0) &&
             (((uint32_t)g_checkout_ui_cart_selected_index >= PRODUCT_ID_COUNT) ||
              (cart->items[g_checkout_ui_cart_selected_index].line_valid == 0U)))
    {
        g_checkout_ui_cart_selected_index = -1;
    }

    for (idx = 0U; idx < PRODUCT_ID_COUNT; idx++)
    {
        lv_obj_t *row = g_checkout_ui_cart.item_row[idx];
        lv_obj_t *check = g_checkout_ui_cart.item_check[idx];
        const cart_item_t *item = &cart->items[idx];
        const product_info_t *product = product_catalog_get(item->product_id);

        if (row == NULL)
        {
            continue;
        }

        if (item->line_valid == 0U)
        {
            lv_obj_add_flag(row, LV_OBJ_FLAG_HIDDEN);
            continue;
        }

        lv_obj_clear_flag(row, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_pos(row, 0, (lv_coord_t)y);
        y += 108U;

        if (g_checkout_ui_cart.item_name_label[idx] != NULL)
        {
            lv_label_set_text(g_checkout_ui_cart.item_name_label[idx], item->product_name);
        }

        if (g_checkout_ui_cart.item_price_label[idx] != NULL)
        {
            (void)snprintf(text,
                           sizeof(text),
                           "单价：%lu.%02lu %s",
                           (unsigned long)(item->unit_price_cent / 100U),
                           (unsigned long)(item->unit_price_cent % 100U),
                           (product != NULL) ? product->price_unit_text : "元");
            lv_label_set_text(g_checkout_ui_cart.item_price_label[idx], text);
        }

        if (g_checkout_ui_cart.item_amount_label[idx] != NULL)
        {
            if (item->pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
            {
                (void)snprintf(text,
                               sizeof(text),
                               "重量：%lu.%03lu kg",
                               (unsigned long)(item->total_weight_g / 1000U),
                               (unsigned long)(item->total_weight_g % 1000U));
            }
            else
            {
                (void)snprintf(text,
                               sizeof(text),
                               "数量：%lu %s",
                               (unsigned long)item->total_count,
                               (product != NULL) ? product->display_unit_text : "件");
            }
            lv_label_set_text(g_checkout_ui_cart.item_amount_label[idx], text);
        }

        if (g_checkout_ui_cart.item_subtotal_label[idx] != NULL)
        {
            (void)snprintf(text,
                           sizeof(text),
                           "%lu.%02lu 元",
                           (unsigned long)(item->subtotal_cent / 100U),
                           (unsigned long)(item->subtotal_cent % 100U));
            lv_label_set_text(g_checkout_ui_cart.item_subtotal_label[idx], text);
        }

        if (check != NULL)
        {
            lv_obj_set_style_bg_color(check,
                                      lv_color_hex((g_checkout_ui_cart_selected_index == (int32_t)idx) ?
                                                   CHECKOUT_UI_COLOR_BTN_BLUE : CHECKOUT_UI_COLOR_CARD_BG),
                                      0);
            lv_obj_set_style_border_color(check,
                                          lv_color_hex((g_checkout_ui_cart_selected_index == (int32_t)idx) ?
                                                       CHECKOUT_UI_COLOR_BTN_BLUE : 0xB0B0B0),
                                          0);
        }
    }

    if (g_checkout_ui_cart.empty_label != NULL)
    {
        if (cart->item_count == 0U)
        {
            lv_obj_clear_flag(g_checkout_ui_cart.empty_label, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_add_flag(g_checkout_ui_cart.empty_label, LV_OBJ_FLAG_HIDDEN);
        }
    }

    if (g_checkout_ui_cart.hint_label != NULL)
    {
        lv_obj_set_pos(g_checkout_ui_cart.hint_label, 18, (lv_coord_t)(y + 8U));
    }

    checkout_ui_cart_update_selection();
}

void checkout_ui_refresh_main_summary(const cart_context_t *cart)
{
    if (cart == NULL)
    {
        checkout_ui_main_set_cart_summary(0U);
        checkout_ui_main_set_total_price(0U);
        return;
    }

    checkout_ui_main_set_cart_summary(cart->item_count);
    checkout_ui_main_set_total_price(cart->total_price_cent);
}

void checkout_ui_refresh_cart_page(const cart_context_t *cart)
{
    checkout_ui_cart_refresh(cart);
}

void checkout_ui_refresh_all(const cart_context_t *cart)
{
    checkout_ui_refresh_main_summary(cart);
    checkout_ui_refresh_cart_page(cart);
}

int32_t checkout_ui_cart_get_selected_index(void)
{
    return g_checkout_ui_cart_selected_index;
}

void checkout_ui_show_main_page(void)
{
    checkout_ui_framework_init();
    checkout_ui_hide_all_pages();
    lv_obj_clear_flag(g_checkout_ui_pages.main_page, LV_OBJ_FLAG_HIDDEN);
    checkout_ui_hide_popup();
    lv_scr_load(g_checkout_ui_pages.root);
}

void checkout_ui_show_cart_page(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_cart_page();
    checkout_ui_hide_all_pages();
    lv_obj_clear_flag(g_checkout_ui_pages.cart_page, LV_OBJ_FLAG_HIDDEN);
    checkout_ui_hide_popup();
    checkout_ui_cart_refresh((g_checkout_ui_get_cart_cb != NULL) ?
                             g_checkout_ui_get_cart_cb(g_checkout_ui_cart_action_user_data) : NULL);
    lv_scr_load(g_checkout_ui_pages.root);
}

void checkout_ui_show_payment_page(uint32_t total_price_cent)
{
    char order_no[20];

    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();

    if ((g_checkout_ui_pages.cart_page != NULL) &&
        !lv_obj_has_flag(g_checkout_ui_pages.cart_page, LV_OBJ_FLAG_HIDDEN))
    {
        g_checkout_ui_payment_prev_page = CHECKOUT_UI_PAYMENT_PREV_CART;
    }
    else
    {
        g_checkout_ui_payment_prev_page = CHECKOUT_UI_PAYMENT_PREV_MAIN;
    }

    g_checkout_ui_payment_order_seq++;
    if (g_checkout_ui_payment_order_seq == 0U)
    {
        g_checkout_ui_payment_order_seq = 1U;
    }

    (void)snprintf(order_no,
                   sizeof(order_no),
                   "20260513-%03lu",
                   (unsigned long)((g_checkout_ui_payment_order_seq - 1U) % 1000U + 1U));
    (void)snprintf(g_checkout_ui_payment_order_no, sizeof(g_checkout_ui_payment_order_no), "%s", order_no);
    checkout_ui_update_payment_amount(total_price_cent);
    checkout_ui_update_order_no(order_no);
    checkout_ui_set_payment_scanning();
    checkout_ui_reset_payment_camera_preview();
    checkout_ui_hide_all_pages();
    lv_obj_clear_flag(g_checkout_ui_pages.payment_page, LV_OBJ_FLAG_HIDDEN);
    checkout_ui_hide_popup();
    lv_scr_load(g_checkout_ui_pages.root);

    if (g_checkout_ui_payment_start_cb != NULL)
    {
        g_checkout_ui_payment_start_cb(g_checkout_ui_payment_action_user_data);
    }
}

void checkout_ui_show_payment_success_page(const char *order_no,
                                           uint32_t total_price_cent,
                                           const char *pay_time)
{
    char text[24];
    char amount_text[32];

    checkout_ui_framework_init();
    checkout_ui_ensure_payment_success_page();
    checkout_ui_format_money(total_price_cent, text, sizeof(text));
    (void)snprintf(amount_text, sizeof(amount_text), "￥%s", text);

    if (g_checkout_ui_payment_success.order_no_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment_success.order_no_value_label,
                          (order_no != NULL) ? order_no : g_checkout_ui_payment_order_no);
    }
    if (g_checkout_ui_payment_success.amount_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment_success.amount_value_label, amount_text);
    }
    if (g_checkout_ui_payment_success.method_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment_success.method_value_label, "扫码支付（模拟）");
    }
    if (g_checkout_ui_payment_success.time_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment_success.time_value_label,
                          (pay_time != NULL) ? pay_time : "--");
    }

    checkout_ui_hide_all_pages();
    checkout_ui_hide_popup();
    lv_obj_clear_flag(g_checkout_ui_pages.payment_success_page, LV_OBJ_FLAG_HIDDEN);
    lv_scr_load(g_checkout_ui_pages.root);
}

void checkout_ui_set_payment_scanning(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();
    g_checkout_ui_payment_state = CHECKOUT_UI_PAYMENT_INVALID;
    checkout_ui_payment_set_state(CHECKOUT_UI_PAYMENT_SCANNING);
}

void checkout_ui_set_payment_recognizing(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();
    checkout_ui_payment_set_state(CHECKOUT_UI_PAYMENT_RECOGNIZING);
}

void checkout_ui_set_payment_success(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();
    checkout_ui_payment_set_state(CHECKOUT_UI_PAYMENT_SUCCESS);
}

void checkout_ui_set_payment_invalid(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();
    checkout_ui_payment_set_state(CHECKOUT_UI_PAYMENT_INVALID);
}

void checkout_ui_reset_payment_camera_preview(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();

    if (g_checkout_ui_payment.scan_preview_image != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_payment.scan_preview_image, LV_OBJ_FLAG_HIDDEN);
    }
    if (g_checkout_ui_payment.scan_qr_panel != NULL)
    {
        lv_obj_clear_flag(g_checkout_ui_payment.scan_qr_panel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(g_checkout_ui_payment.scan_qr_panel);
    }
    if (g_checkout_ui_payment.scan_frame != NULL)
    {
        lv_obj_invalidate(g_checkout_ui_payment.scan_frame);
    }
}

void checkout_ui_payment_set_camera_source(uint16_t width, uint16_t height, const uint16_t *pixels)
{
    lv_coord_t box_w;
    lv_coord_t box_h;
    uint32_t zoom_x;
    uint32_t zoom_y;
    uint32_t zoom;

    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();

    if ((g_checkout_ui_payment.scan_preview_image == NULL) ||
        (g_checkout_ui_payment.scan_frame == NULL) ||
        (pixels == NULL) ||
        (width == 0U) ||
        (height == 0U))
    {
        return;
    }

    g_checkout_ui_payment_preview_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    g_checkout_ui_payment_preview_dsc.header.w = width;
    g_checkout_ui_payment_preview_dsc.header.h = height;
    g_checkout_ui_payment_preview_dsc.data_size = (uint32_t)width * (uint32_t)height * 2U;
    g_checkout_ui_payment_preview_dsc.data = (const uint8_t *)pixels;

    box_w = lv_obj_get_width(g_checkout_ui_payment.scan_frame);
    box_h = lv_obj_get_height(g_checkout_ui_payment.scan_frame);
    zoom_x = (uint32_t)box_w * 256U / width;
    zoom_y = (uint32_t)box_h * 256U / height;
    zoom = (zoom_x < zoom_y) ? zoom_x : zoom_y;
    if (zoom > 256U)
    {
        zoom = 256U;
    }
    if (zoom < 32U)
    {
        zoom = 32U;
    }

    lv_img_cache_invalidate_src(&g_checkout_ui_payment_preview_dsc);
    lv_img_set_src(g_checkout_ui_payment.scan_preview_image, &g_checkout_ui_payment_preview_dsc);
    lv_img_set_zoom(g_checkout_ui_payment.scan_preview_image, (uint16_t)zoom);
    lv_obj_center(g_checkout_ui_payment.scan_preview_image);
    lv_obj_clear_flag(g_checkout_ui_payment.scan_preview_image, LV_OBJ_FLAG_HIDDEN);
    if (g_checkout_ui_payment.scan_qr_panel != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_payment.scan_qr_panel, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_move_background(g_checkout_ui_payment.scan_preview_image);
    lv_obj_invalidate(g_checkout_ui_payment.scan_frame);
}

void checkout_ui_update_payment_amount(uint32_t total_price_cent)
{
    char text[24];

    checkout_ui_framework_init();
    checkout_ui_ensure_payment_page();
    checkout_ui_format_money(total_price_cent, text, sizeof(text));
    if (g_checkout_ui_payment.amount_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment.amount_value_label, text);
    }
}

void checkout_ui_update_order_no(const char *order_no)
{
    checkout_ui_framework_init();
    (void)snprintf(g_checkout_ui_payment_order_no,
                   sizeof(g_checkout_ui_payment_order_no),
                   "%s",
                   (order_no != NULL) ? order_no : "");
    if (g_checkout_ui_payment.order_no_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment.order_no_label,
                          (order_no != NULL) ? order_no : "");
    }
}

const char *checkout_ui_get_payment_order_no(void)
{
    return g_checkout_ui_payment_order_no;
}

void checkout_ui_show_payment_invalid_popup(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_invalid_popup();
    checkout_ui_show_popup(g_checkout_ui_pages.payment_invalid_popup);
}

void checkout_ui_show_payment_timeout_popup(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_payment_timeout_popup();
    checkout_ui_show_popup(g_checkout_ui_pages.payment_timeout_popup);
}

void checkout_ui_show_recog_fail_popup(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_recog_fail_popup();
    if (g_checkout_ui_recog_fail.title_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_recog_fail.title_label, "识别失败");
    }
    if (g_checkout_ui_recog_fail.body_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_recog_fail.body_label, "请取走商品后重新放置");
    }
    checkout_ui_show_popup(g_checkout_ui_pages.recog_fail_popup);
}

void checkout_ui_show_delete_confirm_popup(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_delete_confirm_popup();
    checkout_ui_show_popup(g_checkout_ui_pages.delete_confirm_popup);
}

void checkout_ui_show_clear_confirm_popup(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_clear_confirm_popup();
    checkout_ui_show_popup(g_checkout_ui_pages.clear_confirm_popup);
}

void checkout_ui_show_checkout_done_popup(uint32_t total_price_cent)
{
    char text[24];

    checkout_ui_framework_init();
    checkout_ui_ensure_checkout_done_popup();
    checkout_ui_format_money(total_price_cent, text, sizeof(text));
    if (g_checkout_ui_checkout_done.amount_value_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_checkout_done.amount_value_label, text);
    }
    if (g_checkout_ui_checkout_done.amount_unit_label != NULL)
    {
        lv_obj_align_to(g_checkout_ui_checkout_done.amount_unit_label,
                        g_checkout_ui_checkout_done.amount_value_label,
                        LV_ALIGN_OUT_RIGHT_MID,
                        6,
                        0);
    }
    checkout_ui_show_popup(g_checkout_ui_pages.checkout_done_popup);
}

void checkout_ui_hide_popup(void)
{
    if (g_checkout_ui_pages.popup_mask == NULL)
    {
        return;
    }

    lv_obj_add_flag(g_checkout_ui_pages.popup_mask, LV_OBJ_FLAG_HIDDEN);
    checkout_ui_hide_all_popups();
}

void checkout_ui_set_cart_action_callbacks(checkout_ui_cart_action_cb_t delete_selected_cb,
                                           checkout_ui_cart_action_cb_t clear_cart_cb,
                                           checkout_ui_cart_action_cb_t checkout_done_cb,
                                           checkout_ui_cart_get_cb_t get_cart_cb,
                                           void *user_data)
{
    g_checkout_ui_delete_selected_cb = delete_selected_cb;
    g_checkout_ui_clear_cart_cb = clear_cart_cb;
    g_checkout_ui_checkout_done_cb = checkout_done_cb;
    g_checkout_ui_get_cart_cb = get_cart_cb;
    g_checkout_ui_cart_action_user_data = user_data;
}

void checkout_ui_set_payment_action_callbacks(checkout_ui_payment_action_cb_t start_cb,
                                              checkout_ui_payment_action_cb_t cancel_cb,
                                              checkout_ui_payment_action_cb_t rescan_cb,
                                              void *user_data)
{
    g_checkout_ui_payment_start_cb = start_cb;
    g_checkout_ui_payment_cancel_cb = cancel_cb;
    g_checkout_ui_payment_rescan_cb = rescan_cb;
    g_checkout_ui_payment_action_user_data = user_data;
}

static lv_obj_t *checkout_ui_create_page(lv_obj_t *parent)
{
    lv_obj_t *page;

    page = lv_obj_create(parent);
    lv_obj_set_size(page, CHECKOUT_UI_SCREEN_W, CHECKOUT_UI_SCREEN_H);
    lv_obj_set_pos(page, 0, 0);
    lv_obj_add_style(page, &g_style_screen, 0);
    lv_obj_add_flag(page, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);

    return page;
}

static void checkout_ui_ensure_cart_page(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_cart_built == 0U)
    {
        checkout_ui_build_cart_page();
        g_checkout_ui_cart_built = 1U;
    }
}

static void checkout_ui_ensure_payment_page(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_payment_built == 0U)
    {
        checkout_ui_build_payment_page();
        g_checkout_ui_payment_built = 1U;
    }
}

static void checkout_ui_ensure_payment_success_page(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_payment_success_built == 0U)
    {
        checkout_ui_build_payment_success_page();
        g_checkout_ui_payment_success_built = 1U;
    }
}

static void checkout_ui_ensure_recog_fail_popup(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_pages.recog_fail_popup == NULL)
    {
        g_checkout_ui_pages.recog_fail_popup = checkout_ui_create_popup_panel(g_checkout_ui_pages.popup_mask, 310, 190);
        checkout_ui_build_recog_fail_popup(g_checkout_ui_pages.recog_fail_popup);
    }
}

static void checkout_ui_ensure_payment_invalid_popup(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_pages.payment_invalid_popup == NULL)
    {
        g_checkout_ui_pages.payment_invalid_popup = checkout_ui_create_popup_panel(g_checkout_ui_pages.popup_mask, 320, 230);
        checkout_ui_build_payment_invalid_popup(g_checkout_ui_pages.payment_invalid_popup);
    }
}

static void checkout_ui_ensure_payment_timeout_popup(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_pages.payment_timeout_popup == NULL)
    {
        g_checkout_ui_pages.payment_timeout_popup = checkout_ui_create_popup_panel(g_checkout_ui_pages.popup_mask, 320, 230);
        checkout_ui_build_payment_timeout_popup(g_checkout_ui_pages.payment_timeout_popup);
    }
}

static void checkout_ui_ensure_delete_confirm_popup(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_pages.delete_confirm_popup == NULL)
    {
        g_checkout_ui_pages.delete_confirm_popup = checkout_ui_create_popup_panel(g_checkout_ui_pages.popup_mask, 320, 200);
        checkout_ui_build_confirm_popup(&g_checkout_ui_delete_confirm,
                                        g_checkout_ui_pages.delete_confirm_popup,
                                        "删除商品",
                                        "确认删除当前选中商品？",
                                        checkout_ui_delete_confirm_event_cb);
    }
}

static void checkout_ui_ensure_clear_confirm_popup(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_pages.clear_confirm_popup == NULL)
    {
        g_checkout_ui_pages.clear_confirm_popup = checkout_ui_create_popup_panel(g_checkout_ui_pages.popup_mask, 320, 200);
        checkout_ui_build_confirm_popup(&g_checkout_ui_clear_confirm,
                                        g_checkout_ui_pages.clear_confirm_popup,
                                        "清空购物车",
                                        "确认清空全部商品？",
                                        checkout_ui_clear_confirm_event_cb);
    }
}

static void checkout_ui_ensure_checkout_done_popup(void)
{
    checkout_ui_framework_init();
    if (g_checkout_ui_pages.checkout_done_popup == NULL)
    {
        g_checkout_ui_pages.checkout_done_popup = checkout_ui_create_popup_panel(g_checkout_ui_pages.popup_mask, 320, 210);
        checkout_ui_build_checkout_done_popup(g_checkout_ui_pages.checkout_done_popup);
    }
}

static void checkout_ui_build_cart_page(void)
{
    lv_obj_t *label;
    lv_obj_t *line;
    lv_obj_t *icon_bg;
    lv_obj_t *thumb;
    uint32_t idx;

    g_checkout_ui_cart.header = lv_obj_create(g_checkout_ui_pages.cart_page);
    lv_obj_set_size(g_checkout_ui_cart.header, 480, 60);
    lv_obj_set_pos(g_checkout_ui_cart.header, 0, 0);
    lv_obj_add_style(g_checkout_ui_cart.header, &g_style_header, 0);
    lv_obj_clear_flag(g_checkout_ui_cart.header, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_cart.back_button = lv_btn_create(g_checkout_ui_cart.header);
    lv_obj_set_size(g_checkout_ui_cart.back_button, 40, 40);
    lv_obj_set_pos(g_checkout_ui_cart.back_button, 12, 10);
    lv_obj_set_style_bg_opa(g_checkout_ui_cart.back_button, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_checkout_ui_cart.back_button, 0, 0);
    lv_obj_clear_flag(g_checkout_ui_cart.back_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_cart.back_button, checkout_ui_cart_back_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(g_checkout_ui_cart.back_button);
    lv_label_set_text(label, "<");
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    lv_obj_center(label);

    label = lv_label_create(g_checkout_ui_cart.header);
    lv_label_set_text(label, "购物车");
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_center(label);

    g_checkout_ui_cart.clear_top_button = lv_btn_create(g_checkout_ui_cart.header);
    lv_obj_set_size(g_checkout_ui_cart.clear_top_button, 68, 40);
    lv_obj_align(g_checkout_ui_cart.clear_top_button, LV_ALIGN_RIGHT_MID, -12, 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_cart.clear_top_button, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_checkout_ui_cart.clear_top_button, 0, 0);
    lv_obj_clear_flag(g_checkout_ui_cart.clear_top_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_cart.clear_top_button, checkout_ui_cart_clear_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(g_checkout_ui_cart.clear_top_button);
    lv_label_set_text(label, "娓呯┖");
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_center(label);

    g_checkout_ui_cart.summary_card = checkout_ui_create_card(g_checkout_ui_pages.cart_page, 15, 72, 450, 70);
    icon_bg = lv_obj_create(g_checkout_ui_cart.summary_card);
    lv_obj_set_size(icon_bg, 38, 38);
    lv_obj_set_pos(icon_bg, 26, 16);
    lv_obj_set_style_bg_color(icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(icon_bg, 0, 0);
    lv_obj_set_style_radius(icon_bg, 19, 0);
    lv_obj_clear_flag(icon_bg, LV_OBJ_FLAG_SCROLLABLE);
    label = checkout_ui_create_icon(icon_bg, &checkout_icon_cart_summary_36);
    lv_obj_center(label);

    g_checkout_ui_cart.summary_count_label = checkout_ui_create_title_label(g_checkout_ui_cart.summary_card, "共 0 类商品");
    lv_obj_set_pos(g_checkout_ui_cart.summary_count_label, 84, 25);

    line = lv_obj_create(g_checkout_ui_cart.summary_card);
    lv_obj_set_size(line, 1, 46);
    lv_obj_set_pos(line, 272, 12);
    lv_obj_set_style_bg_color(line, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    label = checkout_ui_create_title_label(g_checkout_ui_cart.summary_card, "总价：");
    lv_obj_set_pos(label, 298, 12);
    g_checkout_ui_cart.summary_total_label = checkout_ui_create_amount_label(g_checkout_ui_cart.summary_card, "0.00");
    lv_obj_set_pos(g_checkout_ui_cart.summary_total_label, 298, 34);
    g_checkout_ui_cart.summary_total_unit_label = checkout_ui_create_title_label(g_checkout_ui_cart.summary_card, "元");
    lv_obj_set_style_text_color(g_checkout_ui_cart.summary_total_unit_label, lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
    lv_obj_align_to(g_checkout_ui_cart.summary_total_unit_label, g_checkout_ui_cart.summary_total_label, LV_ALIGN_OUT_RIGHT_MID, 6, 2);

    g_checkout_ui_cart.list_card = checkout_ui_create_card(g_checkout_ui_pages.cart_page, 15, 154, 450, 500);
    lv_obj_set_scroll_dir(g_checkout_ui_cart.list_card, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(g_checkout_ui_cart.list_card, LV_SCROLLBAR_MODE_AUTO);
    g_checkout_ui_cart.list_container = g_checkout_ui_cart.list_card;

    g_checkout_ui_cart.empty_label = checkout_ui_create_title_label(g_checkout_ui_cart.list_card, "购物车为空");
    lv_obj_center(g_checkout_ui_cart.empty_label);

    for (idx = 0U; idx < PRODUCT_ID_COUNT; idx++)
    {
        lv_obj_t *row = lv_obj_create(g_checkout_ui_cart.list_card);
        lv_obj_set_size(row, 430, 104);
        lv_obj_set_pos(row, 0, (lv_coord_t)(idx * 108U));
        lv_obj_set_style_bg_color(row, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
        lv_obj_set_style_bg_opa(row, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_radius(row, 0, 0);
        lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(row, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(row, checkout_ui_cart_item_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)idx);
        g_checkout_ui_cart.item_row[idx] = row;

        g_checkout_ui_cart.item_check[idx] = lv_obj_create(row);
        lv_obj_set_size(g_checkout_ui_cart.item_check[idx], 28, 28);
        lv_obj_set_pos(g_checkout_ui_cart.item_check[idx], 16, 38);
        lv_obj_set_style_bg_color(g_checkout_ui_cart.item_check[idx], lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
        lv_obj_set_style_bg_opa(g_checkout_ui_cart.item_check[idx], LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(g_checkout_ui_cart.item_check[idx], 2, 0);
        lv_obj_set_style_border_color(g_checkout_ui_cart.item_check[idx], lv_color_hex(0xB0B0B0), 0);
        lv_obj_set_style_radius(g_checkout_ui_cart.item_check[idx], 14, 0);
        lv_obj_clear_flag(g_checkout_ui_cart.item_check[idx], LV_OBJ_FLAG_SCROLLABLE);

        thumb = lv_obj_create(row);
        lv_obj_set_size(thumb, 76, 76);
        lv_obj_set_pos(thumb, 58, 14);
        lv_obj_set_style_bg_color(thumb, lv_color_hex(0xEFEFEF), 0);
        lv_obj_set_style_bg_opa(thumb, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(thumb, 1, 0);
        lv_obj_set_style_border_color(thumb, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
        lv_obj_set_style_radius(thumb, 8, 0);
        lv_obj_clear_flag(thumb, LV_OBJ_FLAG_SCROLLABLE);
        label = checkout_ui_create_icon(thumb, &checkout_icon_basket_placeholder_72);
        lv_obj_center(label);

        g_checkout_ui_cart.item_name_label[idx] = checkout_ui_create_title_label(row, "--");
        lv_obj_set_pos(g_checkout_ui_cart.item_name_label[idx], 154, 16);
        g_checkout_ui_cart.item_price_label[idx] = checkout_ui_create_body_label(row, "单价：-");
        lv_obj_set_pos(g_checkout_ui_cart.item_price_label[idx], 154, 45);
        g_checkout_ui_cart.item_amount_label[idx] = checkout_ui_create_body_label(row, "重量：-");
        lv_obj_set_pos(g_checkout_ui_cart.item_amount_label[idx], 154, 72);

        g_checkout_ui_cart.item_subtotal_label[idx] = checkout_ui_create_title_label(row, "0.00 元");
        lv_obj_set_style_text_color(g_checkout_ui_cart.item_subtotal_label[idx], lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
        lv_obj_set_width(g_checkout_ui_cart.item_subtotal_label[idx], 96);
        lv_label_set_long_mode(g_checkout_ui_cart.item_subtotal_label[idx], LV_LABEL_LONG_CLIP);
        lv_obj_set_pos(g_checkout_ui_cart.item_subtotal_label[idx], 326, 22);

        line = lv_obj_create(row);
        lv_obj_set_size(line, 406, 1);
        lv_obj_set_pos(line, 12, 103);
        lv_obj_set_style_bg_color(line, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
        lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(line, 0, 0);
        lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
    }

    g_checkout_ui_cart.hint_label = checkout_ui_create_body_label(g_checkout_ui_cart.list_card, "点击商品可选中，再进行删除操作");
    lv_obj_set_pos(g_checkout_ui_cart.hint_label, 18, 444);

    g_checkout_ui_cart.delete_button = checkout_ui_create_cart_action_button(g_checkout_ui_pages.cart_page,
                                                                            "删除选中项",
                                                                            CHECKOUT_UI_BUTTON_RED);
    lv_obj_set_pos(g_checkout_ui_cart.delete_button, 15, 682);
    lv_obj_set_size(g_checkout_ui_cart.delete_button, 140, 92);
    lv_obj_add_event_cb(g_checkout_ui_cart.delete_button, checkout_ui_cart_delete_event_cb, LV_EVENT_CLICKED, NULL);

    g_checkout_ui_cart.clear_button = checkout_ui_create_cart_action_button(g_checkout_ui_pages.cart_page,
                                                                           "清空购物车",
                                                                           CHECKOUT_UI_BUTTON_ORANGE);
    lv_obj_set_pos(g_checkout_ui_cart.clear_button, 170, 682);
    lv_obj_set_size(g_checkout_ui_cart.clear_button, 140, 92);
    lv_obj_add_event_cb(g_checkout_ui_cart.clear_button, checkout_ui_cart_clear_event_cb, LV_EVENT_CLICKED, NULL);

    g_checkout_ui_cart.checkout_button = checkout_ui_create_cart_action_button(g_checkout_ui_pages.cart_page,
                                                                              "去结账",
                                                                              CHECKOUT_UI_BUTTON_BLUE);
    lv_obj_set_pos(g_checkout_ui_cart.checkout_button, 325, 682);
    lv_obj_set_size(g_checkout_ui_cart.checkout_button, 140, 92);
    lv_obj_add_event_cb(g_checkout_ui_cart.checkout_button, checkout_ui_cart_checkout_event_cb, LV_EVENT_CLICKED, NULL);
}

static lv_obj_t *checkout_ui_create_payment_text_button(lv_obj_t *parent,
                                                        const char *text,
                                                        uint32_t color)
{
    lv_obj_t *button;
    lv_obj_t *label;

    button = lv_btn_create(parent);
    lv_obj_set_style_bg_color(button, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(button, 0, 0);
    lv_obj_set_style_radius(button, 8, 0);
    lv_obj_clear_flag(button, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(button);
    lv_label_set_text(label, (text != NULL) ? text : "");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);

    return button;
}

static void checkout_ui_create_payment_corner(lv_obj_t *parent,
                                              lv_obj_t **corner,
                                              lv_coord_t x,
                                              lv_coord_t y,
                                              uint8_t right,
                                              uint8_t bottom)
{
    lv_obj_t *container;
    lv_obj_t *line_a;
    lv_obj_t *line_b;

    if (corner == NULL)
    {
        return;
    }

    container = lv_obj_create(parent);
    lv_obj_set_pos(container, x, y);
    lv_obj_set_size(container, 46, 46);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container, 0, 0);
    lv_obj_set_style_pad_all(container, 0, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    line_a = lv_obj_create(container);
    lv_obj_set_size(line_a, 46, 5);
    lv_obj_set_pos(line_a, 0, (bottom != 0U) ? 41 : 0);
    lv_obj_set_style_bg_color(line_a, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(line_a, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line_a, 0, 0);
    lv_obj_clear_flag(line_a, LV_OBJ_FLAG_SCROLLABLE);

    line_b = lv_obj_create(container);
    lv_obj_set_size(line_b, 5, 46);
    lv_obj_set_pos(line_b, (right != 0U) ? 41 : 0, 0);
    lv_obj_set_style_bg_color(line_b, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(line_b, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line_b, 0, 0);
    lv_obj_clear_flag(line_b, LV_OBJ_FLAG_SCROLLABLE);

    *corner = container;
}

static void checkout_ui_create_payment_qr_mock(lv_obj_t *parent)
{
    static const uint8_t cells[9][9] = {
        {1,1,1,0,1,0,1,1,1},
        {1,0,1,1,0,1,1,0,1},
        {1,1,1,0,1,1,1,1,1},
        {0,1,0,1,1,0,0,1,0},
        {1,0,1,1,0,1,1,0,1},
        {0,1,1,0,1,0,1,1,0},
        {1,1,1,1,0,1,1,1,1},
        {1,0,1,0,1,1,1,0,1},
        {1,1,1,1,0,0,1,1,1}
    };
    uint8_t y;

    for (y = 0U; y < 9U; y++)
    {
        uint8_t x;

        for (x = 0U; x < 9U; x++)
        {
            if (cells[y][x] != 0U)
            {
                lv_obj_t *cell = lv_obj_create(parent);
                lv_obj_set_size(cell, 12, 12);
                lv_obj_set_pos(cell, (lv_coord_t)(14 + (x * 15U)), (lv_coord_t)(14 + (y * 15U)));
                lv_obj_set_style_bg_color(cell, lv_color_hex(0x111111), 0);
                lv_obj_set_style_bg_opa(cell, LV_OPA_COVER, 0);
                lv_obj_set_style_border_width(cell, 0, 0);
                lv_obj_clear_flag(cell, LV_OBJ_FLAG_SCROLLABLE);
            }
        }
    }
}

static void checkout_ui_build_payment_page(void)
{
    lv_obj_t *label;
    lv_obj_t *line;
    lv_obj_t *header_title;

    g_checkout_ui_payment.header = checkout_ui_create_header(g_checkout_ui_pages.payment_page, "扫码支付");
    header_title = lv_obj_get_child(g_checkout_ui_payment.header, 0);
    lv_obj_set_style_text_font(header_title, &Font20, 0);

    g_checkout_ui_payment.back_button = lv_btn_create(g_checkout_ui_payment.header);
    lv_obj_set_pos(g_checkout_ui_payment.back_button, 10, 8);
    lv_obj_set_size(g_checkout_ui_payment.back_button, 96, 44);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.back_button, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.back_button, 0, 0);
    lv_obj_set_style_shadow_width(g_checkout_ui_payment.back_button, 0, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.back_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_payment.back_button, checkout_ui_payment_back_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(g_checkout_ui_payment.back_button);
    lv_label_set_text(label, "< 返回");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);

    g_checkout_ui_payment.cancel_top_button = lv_btn_create(g_checkout_ui_payment.header);
    lv_obj_set_pos(g_checkout_ui_payment.cancel_top_button, 374, 8);
    lv_obj_set_size(g_checkout_ui_payment.cancel_top_button, 96, 44);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.cancel_top_button, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.cancel_top_button, 0, 0);
    lv_obj_set_style_shadow_width(g_checkout_ui_payment.cancel_top_button, 0, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.cancel_top_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_payment.cancel_top_button, checkout_ui_payment_back_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(g_checkout_ui_payment.cancel_top_button);
    lv_label_set_text(label, "取消");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);

    g_checkout_ui_payment.amount_card = checkout_ui_create_card(g_checkout_ui_pages.payment_page, 15, 75, 450, 95);
    label = checkout_ui_create_title_label(g_checkout_ui_payment.amount_card, "应付金额");
    lv_obj_set_pos(label, 22, 14);
    g_checkout_ui_payment.amount_prefix_label = checkout_ui_create_title_label(g_checkout_ui_payment.amount_card, "￥");
    lv_obj_set_pos(g_checkout_ui_payment.amount_prefix_label, 22, 46);
    lv_obj_set_style_text_color(g_checkout_ui_payment.amount_prefix_label, lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
    g_checkout_ui_payment.amount_value_label = checkout_ui_create_amount_label(g_checkout_ui_payment.amount_card, "0.00");
    lv_obj_set_pos(g_checkout_ui_payment.amount_value_label, 50, 36);

    line = lv_obj_create(g_checkout_ui_payment.amount_card);
    lv_obj_set_pos(line, 225, 18);
    lv_obj_set_size(line, 1, 59);
    lv_obj_set_style_bg_color(line, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    label = checkout_ui_create_title_label(g_checkout_ui_payment.amount_card, "订单号");
    lv_obj_set_pos(label, 272, 22);
    g_checkout_ui_payment.order_no_label = checkout_ui_create_body_label(g_checkout_ui_payment.amount_card, "20260506-001");
    lv_obj_set_pos(g_checkout_ui_payment.order_no_label, 272, 54);

    g_checkout_ui_payment.scan_card = checkout_ui_create_card(g_checkout_ui_pages.payment_page, 15, 185, 450, 330);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.scan_card, lv_color_hex(0x222222), 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.scan_card, 0, 0);
    g_checkout_ui_payment.scan_title_label = lv_label_create(g_checkout_ui_payment.scan_card);
    lv_label_set_text(g_checkout_ui_payment.scan_title_label, "请将付款码对准框内");
    lv_obj_set_style_text_font(g_checkout_ui_payment.scan_title_label, &Font20, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment.scan_title_label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_width(g_checkout_ui_payment.scan_title_label, 420);
    lv_obj_set_style_text_align(g_checkout_ui_payment.scan_title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_pos(g_checkout_ui_payment.scan_title_label, 15, 18);

    g_checkout_ui_payment.scan_frame = lv_obj_create(g_checkout_ui_payment.scan_card);
    lv_obj_set_pos(g_checkout_ui_payment.scan_frame, 95, 50);
    lv_obj_set_size(g_checkout_ui_payment.scan_frame, 260, 260);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.scan_frame, lv_color_hex(0x111111), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.scan_frame, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.scan_frame, 0, 0);
    lv_obj_set_style_pad_all(g_checkout_ui_payment.scan_frame, 0, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.scan_frame, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_payment.scan_preview_image = lv_img_create(g_checkout_ui_payment.scan_frame);
    lv_obj_add_flag(g_checkout_ui_payment.scan_preview_image, LV_OBJ_FLAG_HIDDEN);

    g_checkout_ui_payment.scan_qr_panel = lv_obj_create(g_checkout_ui_payment.scan_card);
    lv_obj_set_size(g_checkout_ui_payment.scan_qr_panel, 160, 160);
    lv_obj_set_pos(g_checkout_ui_payment.scan_qr_panel, 145, 95);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.scan_qr_panel, lv_color_hex(0xF3F3F3), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.scan_qr_panel, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.scan_qr_panel, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment.scan_qr_panel, 2, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.scan_qr_panel, LV_OBJ_FLAG_SCROLLABLE);
    checkout_ui_create_payment_qr_mock(g_checkout_ui_payment.scan_qr_panel);

    checkout_ui_create_payment_corner(g_checkout_ui_payment.scan_frame, &g_checkout_ui_payment.scan_corner[0], 0, 0, 0U, 0U);
    checkout_ui_create_payment_corner(g_checkout_ui_payment.scan_frame, &g_checkout_ui_payment.scan_corner[1], 214, 0, 1U, 0U);
    checkout_ui_create_payment_corner(g_checkout_ui_payment.scan_frame, &g_checkout_ui_payment.scan_corner[2], 0, 214, 0U, 1U);
    checkout_ui_create_payment_corner(g_checkout_ui_payment.scan_frame, &g_checkout_ui_payment.scan_corner[3], 214, 214, 1U, 1U);

    g_checkout_ui_payment.scan_line = lv_obj_create(g_checkout_ui_payment.scan_card);
    lv_obj_set_pos(g_checkout_ui_payment.scan_line, 45, 160);
    lv_obj_set_size(g_checkout_ui_payment.scan_line, 360, 6);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.scan_line, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.scan_line, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.scan_line, 0, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.scan_line, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_payment.success_badge = lv_obj_create(g_checkout_ui_payment.scan_card);
    lv_obj_set_size(g_checkout_ui_payment.success_badge, 76, 76);
    lv_obj_set_pos(g_checkout_ui_payment.success_badge, 187, 137);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.success_badge, lv_color_hex(CHECKOUT_UI_COLOR_BTN_GREEN), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.success_badge, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.success_badge, 4, 0);
    lv_obj_set_style_border_color(g_checkout_ui_payment.success_badge, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_set_style_radius(g_checkout_ui_payment.success_badge, 38, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.success_badge, LV_OBJ_FLAG_SCROLLABLE);
    g_checkout_ui_payment.success_check_label = lv_label_create(g_checkout_ui_payment.success_badge);
    lv_label_set_text(g_checkout_ui_payment.success_check_label, "V");
    lv_obj_set_style_text_font(g_checkout_ui_payment.success_check_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment.success_check_label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(g_checkout_ui_payment.success_check_label);

    g_checkout_ui_payment.hint_card = checkout_ui_create_card(g_checkout_ui_pages.payment_page, 15, 530, 450, 70);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.hint_card, lv_color_hex(0xEAF4FF), 0);
    lv_obj_set_style_border_color(g_checkout_ui_payment.hint_card, lv_color_hex(0x90CAF9), 0);
    label = checkout_ui_create_title_label(g_checkout_ui_payment.hint_card, "提示：");
    lv_obj_set_pos(label, 28, 10);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    label = checkout_ui_create_body_label(g_checkout_ui_payment.hint_card, "请使用摄像头识别付款码完成支付");
    lv_obj_set_pos(label, 28, 38);

    g_checkout_ui_payment.status_card = checkout_ui_create_card(g_checkout_ui_pages.payment_page, 15, 615, 450, 80);
    g_checkout_ui_payment.status_icon_bg = lv_obj_create(g_checkout_ui_payment.status_card);
    lv_obj_set_size(g_checkout_ui_payment.status_icon_bg, 42, 42);
    lv_obj_set_pos(g_checkout_ui_payment.status_icon_bg, 24, 19);
    lv_obj_set_style_bg_color(g_checkout_ui_payment.status_icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment.status_icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment.status_icon_bg, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment.status_icon_bg, 21, 0);
    lv_obj_clear_flag(g_checkout_ui_payment.status_icon_bg, LV_OBJ_FLAG_SCROLLABLE);
    g_checkout_ui_payment.status_icon_label = lv_label_create(g_checkout_ui_payment.status_icon_bg);
    lv_label_set_text(g_checkout_ui_payment.status_icon_label, "i");
    lv_obj_set_style_text_font(g_checkout_ui_payment.status_icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment.status_icon_label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(g_checkout_ui_payment.status_icon_label);

    g_checkout_ui_payment.status_title_label = checkout_ui_create_title_label(g_checkout_ui_payment.status_card, "等待扫码...");
    lv_obj_set_pos(g_checkout_ui_payment.status_title_label, 82, 16);
    g_checkout_ui_payment.status_body_label = checkout_ui_create_body_label(g_checkout_ui_payment.status_card, "将付款码对准摄像头");
    lv_obj_set_pos(g_checkout_ui_payment.status_body_label, 82, 46);

    g_checkout_ui_payment.cancel_button = checkout_ui_create_payment_text_button(g_checkout_ui_pages.payment_page,
                                                                                "取消支付",
                                                                                CHECKOUT_UI_COLOR_BTN_RED);
    lv_obj_set_pos(g_checkout_ui_payment.cancel_button, 15, 715);
    lv_obj_set_size(g_checkout_ui_payment.cancel_button, 215, 60);
    lv_obj_add_event_cb(g_checkout_ui_payment.cancel_button, checkout_ui_payment_back_event_cb, LV_EVENT_CLICKED, NULL);

    g_checkout_ui_payment.rescan_button = checkout_ui_create_payment_text_button(g_checkout_ui_pages.payment_page,
                                                                                "重新扫码",
                                                                                CHECKOUT_UI_COLOR_BTN_BLUE);
    lv_obj_set_pos(g_checkout_ui_payment.rescan_button, 250, 715);
    lv_obj_set_size(g_checkout_ui_payment.rescan_button, 215, 60);
    lv_obj_add_event_cb(g_checkout_ui_payment.rescan_button, checkout_ui_payment_rescan_event_cb, LV_EVENT_CLICKED, NULL);

    checkout_ui_payment_set_state(CHECKOUT_UI_PAYMENT_SCANNING);
}

static void checkout_ui_build_payment_success_page(void)
{
    lv_obj_t *card;
    lv_obj_t *label;
    lv_obj_t *line;
    lv_obj_t *icon_label;
    lv_obj_t *name_label;
    uint32_t idx;
    static const char *names[4] = {
        "订单号",
        "支付金额",
        "支付方式",
        "支付时间"
    };
    lv_obj_t **values[4] = {
        &g_checkout_ui_payment_success.order_no_value_label,
        &g_checkout_ui_payment_success.amount_value_label,
        &g_checkout_ui_payment_success.method_value_label,
        &g_checkout_ui_payment_success.time_value_label
    };

    lv_obj_set_style_bg_color(g_checkout_ui_pages.payment_success_page,
                              lv_color_hex(CHECKOUT_UI_COLOR_HEADER_BG),
                              0);
    lv_obj_set_style_bg_opa(g_checkout_ui_pages.payment_success_page, LV_OPA_COVER, 0);

    g_checkout_ui_payment_success.success_icon_bg = lv_obj_create(g_checkout_ui_pages.payment_success_page);
    lv_obj_set_size(g_checkout_ui_payment_success.success_icon_bg, 72, 72);
    lv_obj_set_pos(g_checkout_ui_payment_success.success_icon_bg, 204, 60);
    lv_obj_set_style_bg_color(g_checkout_ui_payment_success.success_icon_bg,
                              lv_color_hex(CHECKOUT_UI_COLOR_BTN_GREEN),
                              0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_success.success_icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_success.success_icon_bg, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_success.success_icon_bg, 36, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_success.success_icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_payment_success.success_icon_label =
        lv_label_create(g_checkout_ui_payment_success.success_icon_bg);
    lv_label_set_text(g_checkout_ui_payment_success.success_icon_label, "V");
    lv_obj_set_style_text_font(g_checkout_ui_payment_success.success_icon_label,
                               &lv_font_montserrat_32,
                               0);
    lv_obj_set_style_text_color(g_checkout_ui_payment_success.success_icon_label,
                                lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG),
                                0);
    lv_obj_center(g_checkout_ui_payment_success.success_icon_label);

    g_checkout_ui_payment_success.title_label =
        lv_label_create(g_checkout_ui_pages.payment_success_page);
    lv_label_set_text(g_checkout_ui_payment_success.title_label, "支付成功");
    lv_obj_set_style_text_font(g_checkout_ui_payment_success.title_label, &Font20, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment_success.title_label,
                                lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG),
                                0);
    lv_obj_set_width(g_checkout_ui_payment_success.title_label, 240);
    lv_obj_set_style_text_align(g_checkout_ui_payment_success.title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_pos(g_checkout_ui_payment_success.title_label, 120, 150);

    card = checkout_ui_create_card(g_checkout_ui_pages.payment_success_page, 25, 250, 430, 260);
    lv_obj_set_style_radius(card, 10, 0);
    lv_obj_set_style_border_width(card, 0, 0);

    for (idx = 0U; idx < 4U; idx++)
    {
        lv_coord_t y = (lv_coord_t)(22U + (idx * 58U));

        name_label = checkout_ui_create_title_label(card, names[idx]);
        lv_obj_set_pos(name_label, 18, y);

        *values[idx] = checkout_ui_create_title_label(card, (idx == 2U) ? "扫码支付（模拟）" : "--");
        lv_obj_set_width(*values[idx], 230);
        lv_obj_set_style_text_align(*values[idx], LV_TEXT_ALIGN_RIGHT, 0);
        lv_obj_set_pos(*values[idx], 180, y);
        if (idx == 1U)
        {
            lv_obj_set_style_text_color(*values[idx], lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
        }

        if (idx < 3U)
        {
            line = lv_obj_create(card);
            lv_obj_set_size(line, 390, 1);
            lv_obj_set_pos(line, 20, (lv_coord_t)(58U + (idx * 58U)));
            lv_obj_set_style_bg_color(line, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
            lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
            lv_obj_set_style_border_width(line, 0, 0);
            lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
        }
    }

    icon_label = lv_label_create(g_checkout_ui_pages.payment_success_page);
    lv_label_set_text(icon_label, ":)");
    lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(icon_label, lv_color_hex(CHECKOUT_UI_COLOR_BTN_GREEN), 0);
    lv_obj_set_pos(icon_label, 86, 565);

    g_checkout_ui_payment_success.thanks_label =
        lv_label_create(g_checkout_ui_pages.payment_success_page);
    lv_label_set_text(g_checkout_ui_payment_success.thanks_label, "感谢您的购买！");
    lv_obj_set_style_text_font(g_checkout_ui_payment_success.thanks_label, &Font20, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment_success.thanks_label,
                                lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG),
                                0);
    lv_obj_set_pos(g_checkout_ui_payment_success.thanks_label, 130, 558);

    g_checkout_ui_payment_success.done_label =
        lv_label_create(g_checkout_ui_pages.payment_success_page);
    lv_label_set_text(g_checkout_ui_payment_success.done_label, "订单已完成。");
    lv_obj_set_style_text_font(g_checkout_ui_payment_success.done_label, &Font20, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment_success.done_label,
                                lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG),
                                0);
    lv_obj_set_pos(g_checkout_ui_payment_success.done_label, 130, 590);

    g_checkout_ui_payment_success.home_button = lv_btn_create(g_checkout_ui_pages.payment_success_page);
    lv_obj_set_pos(g_checkout_ui_payment_success.home_button, 35, 700);
    lv_obj_set_size(g_checkout_ui_payment_success.home_button, 410, 60);
    lv_obj_set_style_bg_color(g_checkout_ui_payment_success.home_button,
                              lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE),
                              0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_success.home_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_success.home_button, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_success.home_button, 8, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_success.home_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_payment_success.home_button,
                        checkout_ui_payment_success_home_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);
    label = lv_label_create(g_checkout_ui_payment_success.home_button);
    lv_label_set_text(label, "返回首页");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);
}

static void checkout_ui_build_main_page(void)
{
    lv_obj_t *line;
    lv_obj_t *label;
    lv_obj_t *badge;
    lv_obj_t *cart_icon;

    g_checkout_ui_main.header = checkout_ui_create_header(g_checkout_ui_pages.main_page, "请放置商品");

    g_checkout_ui_main.header_title_label = lv_obj_get_child(g_checkout_ui_main.header, 0);
    g_checkout_ui_main.header_icon_slot = checkout_ui_create_icon(g_checkout_ui_main.header, &checkout_icon_scale_header_32);
    lv_obj_align(g_checkout_ui_main.header_icon_slot, LV_ALIGN_RIGHT_MID, -24, 0);

    g_checkout_ui_main.weight_card = checkout_ui_create_card(g_checkout_ui_pages.main_page, 15, 72, 450, 56);
    checkout_ui_create_info_item(g_checkout_ui_main.weight_card,
                                 26,
                                 &checkout_icon_scale_info_36,
                                 "当前重量",
                                 "0.000 kg",
                                 &g_checkout_ui_main.weight_value_label);
    checkout_ui_create_info_item(g_checkout_ui_main.weight_card,
                                 260,
                                 &checkout_icon_time_info_36,
                                 "识别耗时",
                                 "--.-- s",
                                 &g_checkout_ui_main.time_value_label);

    line = lv_obj_create(g_checkout_ui_main.weight_card);
    lv_obj_set_size(line, 1, 40);
    lv_obj_set_pos(line, 225, 8);
    lv_obj_set_style_bg_color(line, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_main.image_card = checkout_ui_create_card(g_checkout_ui_pages.main_page, 15, 140, 450, 220);
    g_checkout_ui_main.image_box = lv_obj_create(g_checkout_ui_main.image_card);
    lv_obj_set_size(g_checkout_ui_main.image_box, 420, 188);
    lv_obj_center(g_checkout_ui_main.image_box);
    lv_obj_set_style_bg_color(g_checkout_ui_main.image_box, lv_color_hex(0xEFEFEF), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_main.image_box, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_main.image_box, 1, 0);
    lv_obj_set_style_border_color(g_checkout_ui_main.image_box, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
    lv_obj_set_style_radius(g_checkout_ui_main.image_box, CHECKOUT_UI_CARD_RADIUS, 0);
    lv_obj_clear_flag(g_checkout_ui_main.image_box, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_main.preview_image = lv_img_create(g_checkout_ui_main.image_box);
    lv_obj_center(g_checkout_ui_main.preview_image);
    lv_obj_add_flag(g_checkout_ui_main.preview_image, LV_OBJ_FLAG_HIDDEN);

    g_checkout_ui_main.image_placeholder_icon = checkout_ui_create_icon(g_checkout_ui_main.image_box,
                                                                        &checkout_icon_basket_placeholder_72);
    lv_obj_align(g_checkout_ui_main.image_placeholder_icon, LV_ALIGN_CENTER, 0, -24);

    g_checkout_ui_main.image_placeholder_label = checkout_ui_create_body_label(g_checkout_ui_main.image_box, "图像显示区域");
    lv_obj_remove_style(g_checkout_ui_main.image_placeholder_label, &g_style_body_text, 0);
    lv_obj_add_style(g_checkout_ui_main.image_placeholder_label, &g_style_hint_text, 0);
    lv_obj_align(g_checkout_ui_main.image_placeholder_label, LV_ALIGN_CENTER, 0, 42);
    lv_obj_move_foreground(g_checkout_ui_main.image_placeholder_icon);
    lv_obj_move_foreground(g_checkout_ui_main.image_placeholder_label);

    g_checkout_ui_main.result_card = checkout_ui_create_card(g_checkout_ui_pages.main_page, 15, 372, 450, 168);
    label = checkout_ui_create_title_label(g_checkout_ui_main.result_card, "本次识别结果");
    lv_obj_set_pos(label, 16, 12);

    badge = lv_obj_create(g_checkout_ui_main.result_card);
    lv_obj_set_size(badge, 90, 34);
    lv_obj_set_pos(badge, 344, 10);
    lv_obj_add_style(badge, &g_style_state_badge, 0);
    lv_obj_clear_flag(badge, LV_OBJ_FLAG_SCROLLABLE);
    g_checkout_ui_main.result_state_label = lv_label_create(badge);
    lv_label_set_text(g_checkout_ui_main.result_state_label, "等待识别");
    lv_obj_set_style_text_color(g_checkout_ui_main.result_state_label, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_text_font(g_checkout_ui_main.result_state_label, &Font20, 0);
    lv_obj_center(g_checkout_ui_main.result_state_label);

    g_checkout_ui_main.result_product_value_label = checkout_ui_create_value_row(g_checkout_ui_main.result_card, 56, "商品：", "--");
    g_checkout_ui_main.result_price_value_label = checkout_ui_create_value_row(g_checkout_ui_main.result_card, 84, "单价：", "--");
    g_checkout_ui_main.result_amount_value_label = checkout_ui_create_value_row(g_checkout_ui_main.result_card, 112, "重量/数量：", "--");
    g_checkout_ui_main.result_subtotal_value_label = checkout_ui_create_value_row(g_checkout_ui_main.result_card, 140, "小计：", "--");

    g_checkout_ui_main.summary_card = checkout_ui_create_card(g_checkout_ui_pages.main_page, 15, 552, 450, 86);
    cart_icon = lv_obj_create(g_checkout_ui_main.summary_card);
    lv_obj_set_size(cart_icon, 48, 48);
    lv_obj_set_pos(cart_icon, 26, 19);
    lv_obj_set_style_bg_color(cart_icon, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(cart_icon, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(cart_icon, 0, 0);
    lv_obj_set_style_radius(cart_icon, 24, 0);
    lv_obj_clear_flag(cart_icon, LV_OBJ_FLAG_SCROLLABLE);
    label = checkout_ui_create_icon(cart_icon, &checkout_icon_cart_summary_36);
    lv_obj_center(label);

    label = checkout_ui_create_title_label(g_checkout_ui_main.summary_card, "购物车");
    lv_obj_set_pos(label, 86, 20);
    g_checkout_ui_main.cart_count_label = checkout_ui_create_body_label(g_checkout_ui_main.summary_card, "共 0 类商品");
    lv_obj_set_pos(g_checkout_ui_main.cart_count_label, 86, 50);

    line = lv_obj_create(g_checkout_ui_main.summary_card);
    lv_obj_set_size(line, 1, 62);
    lv_obj_set_pos(line, 225, 12);
    lv_obj_set_style_bg_color(line, lv_color_hex(CHECKOUT_UI_COLOR_LINE), 0);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    label = checkout_ui_create_title_label(g_checkout_ui_main.summary_card, "总价：");
    lv_obj_set_pos(label, 268, 18);
    g_checkout_ui_main.total_value_label = checkout_ui_create_amount_label(g_checkout_ui_main.summary_card, "0.00");
    lv_obj_set_pos(g_checkout_ui_main.total_value_label, 268, 42);
    g_checkout_ui_main.total_unit_label = checkout_ui_create_title_label(g_checkout_ui_main.summary_card, "元");
    lv_obj_set_style_text_color(g_checkout_ui_main.total_unit_label, lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
    lv_obj_align_to(g_checkout_ui_main.total_unit_label, g_checkout_ui_main.total_value_label, LV_ALIGN_OUT_RIGHT_MID, 8, 2);

    g_checkout_ui_main.cart_button = checkout_ui_create_icon_button(g_checkout_ui_pages.main_page,
                                                                    "查看购物车",
                                                                    &checkout_icon_cart_button_34,
                                                                    CHECKOUT_UI_BUTTON_BLUE);
    lv_obj_set_pos(g_checkout_ui_main.cart_button, 15, 654);
    lv_obj_set_size(g_checkout_ui_main.cart_button, 215, 60);
    lv_obj_add_event_cb(g_checkout_ui_main.cart_button, checkout_ui_main_cart_event_cb, LV_EVENT_CLICKED, NULL);

    g_checkout_ui_main.checkout_button = checkout_ui_create_icon_button(g_checkout_ui_pages.main_page,
                                                                        "结 账",
                                                                        &checkout_icon_money_button_34,
                                                                        CHECKOUT_UI_BUTTON_GREEN);
    lv_obj_set_pos(g_checkout_ui_main.checkout_button, 250, 654);
    lv_obj_set_size(g_checkout_ui_main.checkout_button, 215, 60);
    lv_obj_add_event_cb(g_checkout_ui_main.checkout_button, checkout_ui_main_checkout_event_cb, LV_EVENT_CLICKED, NULL);

    g_checkout_ui_main.clear_button = checkout_ui_create_icon_button(g_checkout_ui_pages.main_page,
                                                                     "清空购物车",
                                                                     &checkout_icon_trash_button_34,
                                                                     CHECKOUT_UI_BUTTON_RED);
    lv_obj_set_pos(g_checkout_ui_main.clear_button, 15, 726);
    lv_obj_set_size(g_checkout_ui_main.clear_button, 215, 60);
    lv_obj_add_event_cb(g_checkout_ui_main.clear_button, checkout_ui_cart_clear_event_cb, LV_EVENT_CLICKED, NULL);

    g_checkout_ui_main.continue_button = checkout_ui_create_icon_button(g_checkout_ui_pages.main_page,
                                                                        "继续购物",
                                                                        &checkout_icon_refresh_button_34,
                                                                        CHECKOUT_UI_BUTTON_ORANGE);
    lv_obj_set_pos(g_checkout_ui_main.continue_button, 250, 726);
    lv_obj_set_size(g_checkout_ui_main.continue_button, 215, 60);
    lv_obj_add_event_cb(g_checkout_ui_main.continue_button, checkout_ui_main_continue_event_cb, LV_EVENT_CLICKED, NULL);
}

static void checkout_ui_create_info_item(lv_obj_t *parent,
                                         lv_coord_t x,
                                         const lv_img_dsc_t *icon,
                                         const char *title,
                                         const char *value,
                                         lv_obj_t **value_label)
{
    lv_obj_t *image;
    lv_obj_t *title_label;
    const lv_coord_t text_x = x + 48;

    image = checkout_ui_create_icon(parent, icon);
    lv_obj_set_pos(image, x, 10);

    title_label = checkout_ui_create_body_label(parent, title);
    lv_obj_set_pos(title_label, text_x, 8);

    *value_label = checkout_ui_create_title_label(parent, value);
    lv_obj_set_pos(*value_label, text_x, 30);
}

static lv_obj_t *checkout_ui_create_value_row(lv_obj_t *parent,
                                              lv_coord_t y,
                                              const char *name,
                                              const char *value)
{
    lv_obj_t *name_label;
    lv_obj_t *value_label;

    name_label = checkout_ui_create_title_label(parent, name);
    lv_obj_set_pos(name_label, 16, y);

    value_label = checkout_ui_create_body_label(parent, value);
    lv_obj_set_pos(value_label, 150, y);

    return value_label;
}

static void checkout_ui_hide_all_pages(void)
{
    if (g_checkout_ui_pages.main_page != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.main_page, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.cart_page != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.cart_page, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.payment_page != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.payment_page, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.payment_success_page != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.payment_success_page, LV_OBJ_FLAG_HIDDEN);
    }
}

static void checkout_ui_return_from_payment_page(void)
{
    if (g_checkout_ui_payment_prev_page == CHECKOUT_UI_PAYMENT_PREV_CART)
    {
        checkout_ui_show_cart_page();
    }
    else
    {
        checkout_ui_show_main_page();
    }
}

static void checkout_ui_payment_set_button_disabled(lv_obj_t *button, uint8_t disabled)
{
    if (button == NULL)
    {
        return;
    }

    if (disabled != 0U)
    {
        lv_obj_add_state(button, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(button, lv_color_hex(0xE0E0E0), 0);
        lv_obj_set_style_text_color(button, lv_color_hex(0x9E9E9E), 0);
    }
    else
    {
        lv_obj_clear_state(button, LV_STATE_DISABLED);
    }
}

static void checkout_ui_payment_set_state(checkout_ui_payment_state_t state)
{
    uint32_t idx;
    uint32_t corner_color = CHECKOUT_UI_COLOR_BTN_BLUE;
    uint32_t line_color = CHECKOUT_UI_COLOR_BTN_BLUE;
    uint32_t status_color = CHECKOUT_UI_COLOR_BTN_BLUE;
    const char *scan_title = "请将付款码对准框内";
    const char *status_title = "等待扫码...";
    const char *status_body = "将付款码对准摄像头";
    const char *status_icon = "i";
    uint8_t show_line = 1U;
    uint8_t show_success = 0U;
    uint8_t disable_buttons = 0U;

    if (g_checkout_ui_payment_state == state)
    {
        return;
    }

    g_checkout_ui_payment_state = state;

    switch (state)
    {
        case CHECKOUT_UI_PAYMENT_RECOGNIZING:
            line_color = CHECKOUT_UI_COLOR_BTN_GREEN;
            status_color = CHECKOUT_UI_COLOR_BTN_GREEN;
            status_title = "正在识别二维码...";
            status_body = "请保持二维码稳定";
            status_icon = "...";
            break;

        case CHECKOUT_UI_PAYMENT_SUCCESS:
            corner_color = CHECKOUT_UI_COLOR_BTN_GREEN;
            line_color = CHECKOUT_UI_COLOR_BTN_GREEN;
            status_color = CHECKOUT_UI_COLOR_BTN_GREEN;
            scan_title = "识别成功";
            status_title = "二维码识别成功";
            status_body = "正在确认支付...";
            status_icon = "V";
            show_line = 0U;
            show_success = 1U;
            disable_buttons = 1U;
            break;

        case CHECKOUT_UI_PAYMENT_INVALID:
            corner_color = CHECKOUT_UI_COLOR_BTN_RED;
            line_color = CHECKOUT_UI_COLOR_BTN_RED;
            status_color = CHECKOUT_UI_COLOR_BTN_RED;
            status_title = "未识别到有效付款码";
            status_body = "请调整后重新扫码";
            status_icon = "!";
            break;

        case CHECKOUT_UI_PAYMENT_SCANNING:
        default:
            break;
    }

    if (g_checkout_ui_payment.scan_title_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment.scan_title_label, scan_title);
    }

    if (g_checkout_ui_payment.scan_line != NULL)
    {
        lv_obj_set_style_bg_color(g_checkout_ui_payment.scan_line, lv_color_hex(line_color), 0);
        if (show_line != 0U)
        {
            lv_obj_clear_flag(g_checkout_ui_payment.scan_line, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_add_flag(g_checkout_ui_payment.scan_line, LV_OBJ_FLAG_HIDDEN);
        }
    }

    for (idx = 0U; idx < 4U; idx++)
    {
        lv_obj_t *corner = g_checkout_ui_payment.scan_corner[idx];
        uint32_t child_count;
        uint32_t child_idx;

        if (corner == NULL)
        {
            continue;
        }

        child_count = lv_obj_get_child_cnt(corner);
        for (child_idx = 0U; child_idx < child_count; child_idx++)
        {
            lv_obj_t *child = lv_obj_get_child(corner, child_idx);
            lv_obj_set_style_bg_color(child, lv_color_hex(corner_color), 0);
        }
    }

    if (g_checkout_ui_payment.success_badge != NULL)
    {
        if (show_success != 0U)
        {
            lv_obj_clear_flag(g_checkout_ui_payment.success_badge, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(g_checkout_ui_payment.success_badge);
        }
        else
        {
            lv_obj_add_flag(g_checkout_ui_payment.success_badge, LV_OBJ_FLAG_HIDDEN);
        }
    }

    if (g_checkout_ui_payment.status_icon_bg != NULL)
    {
        lv_obj_set_style_bg_color(g_checkout_ui_payment.status_icon_bg, lv_color_hex(status_color), 0);
    }
    if (g_checkout_ui_payment.status_icon_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment.status_icon_label, status_icon);
    }
    if (g_checkout_ui_payment.status_title_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment.status_title_label, status_title);
    }
    if (g_checkout_ui_payment.status_body_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_payment.status_body_label, status_body);
    }

    checkout_ui_payment_set_button_disabled(g_checkout_ui_payment.cancel_button, disable_buttons);
    checkout_ui_payment_set_button_disabled(g_checkout_ui_payment.rescan_button, disable_buttons);
    if (disable_buttons == 0U)
    {
        if (g_checkout_ui_payment.cancel_button != NULL)
        {
            lv_obj_set_style_bg_color(g_checkout_ui_payment.cancel_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_RED), 0);
        }
        if (g_checkout_ui_payment.rescan_button != NULL)
        {
            lv_obj_set_style_bg_color(g_checkout_ui_payment.rescan_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
        }
    }
}

static void checkout_ui_hide_all_popups(void)
{
    if (g_checkout_ui_pages.recog_fail_popup != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.recog_fail_popup, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.payment_invalid_popup != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.payment_invalid_popup, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.payment_timeout_popup != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.payment_timeout_popup, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.delete_confirm_popup != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.delete_confirm_popup, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.clear_confirm_popup != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.clear_confirm_popup, LV_OBJ_FLAG_HIDDEN);
    }

    if (g_checkout_ui_pages.checkout_done_popup != NULL)
    {
        lv_obj_add_flag(g_checkout_ui_pages.checkout_done_popup, LV_OBJ_FLAG_HIDDEN);
    }
}

static void checkout_ui_show_popup(lv_obj_t *popup)
{
    if ((g_checkout_ui_pages.popup_mask == NULL) || (popup == NULL))
    {
        return;
    }

    checkout_ui_hide_all_popups();
    lv_obj_clear_flag(g_checkout_ui_pages.popup_mask, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(popup, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(g_checkout_ui_pages.popup_mask);
}

static lv_style_t *checkout_ui_button_style(checkout_ui_button_color_t color)
{
    switch (color)
    {
        case CHECKOUT_UI_BUTTON_GREEN:
            return &g_style_button_green;

        case CHECKOUT_UI_BUTTON_RED:
            return &g_style_button_red;

        case CHECKOUT_UI_BUTTON_ORANGE:
            return &g_style_button_orange;

        case CHECKOUT_UI_BUTTON_BLUE:
        default:
            return &g_style_button_blue;
    }
}

static void checkout_ui_cart_set_selected_index(int32_t selected_index)
{
    if ((selected_index < 0) || ((uint32_t)selected_index >= PRODUCT_ID_COUNT))
    {
        g_checkout_ui_cart_selected_index = -1;
    }
    else if (g_checkout_ui_cart_selected_index == selected_index)
    {
        g_checkout_ui_cart_selected_index = -1;
    }
    else
    {
        g_checkout_ui_cart_selected_index = selected_index;
    }

    checkout_ui_cart_update_selection();
}

static void checkout_ui_cart_update_selection(void)
{
    uint32_t idx;

    for (idx = 0U; idx < PRODUCT_ID_COUNT; idx++)
    {
        lv_obj_t *row = g_checkout_ui_cart.item_row[idx];
        lv_obj_t *check = g_checkout_ui_cart.item_check[idx];
        uint8_t selected = (g_checkout_ui_cart_selected_index == (int32_t)idx) ? 1U : 0U;

        if (row != NULL)
        {
            lv_obj_set_style_bg_color(row,
                                      lv_color_hex((selected != 0U) ? 0xEAF3FF : CHECKOUT_UI_COLOR_CARD_BG),
                                      0);
        }

        if (check != NULL)
        {
            lv_obj_set_style_bg_color(check,
                                      lv_color_hex((selected != 0U) ? CHECKOUT_UI_COLOR_BTN_BLUE : CHECKOUT_UI_COLOR_CARD_BG),
                                      0);
            lv_obj_set_style_border_color(check,
                                          lv_color_hex((selected != 0U) ? CHECKOUT_UI_COLOR_BTN_BLUE : 0xB0B0B0),
                                          0);
        }
    }
}

static void checkout_ui_cart_item_event_cb(lv_event_t *e)
{
    checkout_ui_cart_set_selected_index((int32_t)(uintptr_t)lv_event_get_user_data(e));
}

static void checkout_ui_cart_back_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_show_main_page();
}

static void checkout_ui_cart_delete_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_show_delete_confirm_popup();
}

static void checkout_ui_cart_clear_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_show_clear_confirm_popup();
}

static void checkout_ui_cart_checkout_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_request_checkout();
}

static void checkout_ui_main_cart_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_show_cart_page();
}

static void checkout_ui_main_continue_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_main_show_image(0U);
}

static void checkout_ui_empty_button_event_cb(lv_event_t *e)
{
    (void)e;
}

static void checkout_ui_payment_back_event_cb(lv_event_t *e)
{
    (void)e;
    if (g_checkout_ui_payment_cancel_cb != NULL)
    {
        g_checkout_ui_payment_cancel_cb(g_checkout_ui_payment_action_user_data);
    }
    checkout_ui_return_from_payment_page();
}

static void checkout_ui_payment_rescan_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_set_payment_scanning();
    if (g_checkout_ui_payment_rescan_cb != NULL)
    {
        g_checkout_ui_payment_rescan_cb(g_checkout_ui_payment_action_user_data);
    }
}

static void checkout_ui_payment_invalid_ok_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_hide_popup();
    checkout_ui_set_payment_scanning();
    if (g_checkout_ui_payment_rescan_cb != NULL)
    {
        g_checkout_ui_payment_rescan_cb(g_checkout_ui_payment_action_user_data);
    }
}

static void checkout_ui_payment_timeout_cancel_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_hide_popup();
    if (g_checkout_ui_payment_cancel_cb != NULL)
    {
        g_checkout_ui_payment_cancel_cb(g_checkout_ui_payment_action_user_data);
    }
    checkout_ui_return_from_payment_page();
}

static void checkout_ui_payment_timeout_rescan_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_hide_popup();
    checkout_ui_set_payment_scanning();
    if (g_checkout_ui_payment_rescan_cb != NULL)
    {
        g_checkout_ui_payment_rescan_cb(g_checkout_ui_payment_action_user_data);
    }
}

static void checkout_ui_payment_success_home_event_cb(lv_event_t *e)
{
    (void)e;

    checkout_ui_main_clear_result();
    checkout_ui_main_show_image(0U);
    checkout_ui_main_set_status("请放置商品");
    checkout_ui_refresh_all((g_checkout_ui_get_cart_cb != NULL) ?
                            g_checkout_ui_get_cart_cb(g_checkout_ui_cart_action_user_data) : NULL);
    checkout_ui_show_main_page();
}

static void checkout_ui_main_checkout_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_request_checkout();
}

static void checkout_ui_popup_cancel_event_cb(lv_event_t *e)
{
    (void)e;
    checkout_ui_hide_popup();
}

static void checkout_ui_build_recog_fail_popup(lv_obj_t *panel)
{
    lv_obj_t *label;

    if (panel == NULL)
    {
        return;
    }

    g_checkout_ui_recog_fail.panel = panel;

    g_checkout_ui_recog_fail.icon_bg = lv_obj_create(panel);
    lv_obj_set_size(g_checkout_ui_recog_fail.icon_bg, 56, 56);
    lv_obj_align(g_checkout_ui_recog_fail.icon_bg, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(g_checkout_ui_recog_fail.icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_RED), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_recog_fail.icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_recog_fail.icon_bg, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_recog_fail.icon_bg, 28, 0);
    lv_obj_clear_flag(g_checkout_ui_recog_fail.icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_recog_fail.icon_label = lv_label_create(g_checkout_ui_recog_fail.icon_bg);
    lv_label_set_text(g_checkout_ui_recog_fail.icon_label, "X");
    lv_obj_set_style_text_font(g_checkout_ui_recog_fail.icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_checkout_ui_recog_fail.icon_label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(g_checkout_ui_recog_fail.icon_label);

    g_checkout_ui_recog_fail.title_label = checkout_ui_create_title_label(panel, "识别失败");
    lv_obj_set_width(g_checkout_ui_recog_fail.title_label, 270);
    lv_obj_set_style_text_align(g_checkout_ui_recog_fail.title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_recog_fail.title_label, LV_ALIGN_TOP_MID, 0, 64);

    g_checkout_ui_recog_fail.body_label = checkout_ui_create_body_label(panel, "请取走商品后重新放置");
    lv_obj_set_width(g_checkout_ui_recog_fail.body_label, 270);
    lv_obj_set_style_text_align(g_checkout_ui_recog_fail.body_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_recog_fail.body_label, LV_ALIGN_TOP_MID, 0, 96);

    g_checkout_ui_recog_fail.ok_button = lv_btn_create(panel);
    lv_obj_set_size(g_checkout_ui_recog_fail.ok_button, 220, 44);
    lv_obj_align(g_checkout_ui_recog_fail.ok_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(g_checkout_ui_recog_fail.ok_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_recog_fail.ok_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_recog_fail.ok_button, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_recog_fail.ok_button, 8, 0);
    lv_obj_clear_flag(g_checkout_ui_recog_fail.ok_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_recog_fail.ok_button, checkout_ui_popup_cancel_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(g_checkout_ui_recog_fail.ok_button);
    lv_label_set_text(label, "确定");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);
}

static void checkout_ui_build_payment_invalid_popup(lv_obj_t *panel)
{
    lv_obj_t *label;

    if (panel == NULL)
    {
        return;
    }

    g_checkout_ui_payment_invalid.panel = panel;
    lv_obj_set_style_pad_all(panel, 0, 0);

    g_checkout_ui_payment_invalid.icon_bg = lv_obj_create(panel);
    lv_obj_set_size(g_checkout_ui_payment_invalid.icon_bg, 60, 60);
    lv_obj_align(g_checkout_ui_payment_invalid.icon_bg, LV_ALIGN_TOP_MID, 0, 28);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_invalid.icon_bg, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_invalid.icon_bg, 3, 0);
    lv_obj_set_style_border_color(g_checkout_ui_payment_invalid.icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_RED), 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_invalid.icon_bg, 30, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_invalid.icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_payment_invalid.icon_label = lv_label_create(g_checkout_ui_payment_invalid.icon_bg);
    lv_label_set_text(g_checkout_ui_payment_invalid.icon_label, "X");
    lv_obj_set_style_text_font(g_checkout_ui_payment_invalid.icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment_invalid.icon_label, lv_color_hex(CHECKOUT_UI_COLOR_BTN_RED), 0);
    lv_obj_center(g_checkout_ui_payment_invalid.icon_label);

    g_checkout_ui_payment_invalid.title_label = checkout_ui_create_title_label(panel, "无效的付款码");
    lv_obj_set_width(g_checkout_ui_payment_invalid.title_label, 280);
    lv_obj_set_style_text_align(g_checkout_ui_payment_invalid.title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_payment_invalid.title_label, LV_ALIGN_TOP_MID, 0, 102);

    g_checkout_ui_payment_invalid.body_label = checkout_ui_create_body_label(panel,
                                                                            "未识别到有效的付款码\n请检查二维码是否正确");
    lv_obj_set_width(g_checkout_ui_payment_invalid.body_label, 280);
    lv_obj_set_style_text_align(g_checkout_ui_payment_invalid.body_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_payment_invalid.body_label, LV_ALIGN_TOP_MID, 0, 134);

    g_checkout_ui_payment_invalid.ok_button = lv_btn_create(panel);
    lv_obj_set_size(g_checkout_ui_payment_invalid.ok_button, 230, 48);
    lv_obj_align(g_checkout_ui_payment_invalid.ok_button, LV_ALIGN_BOTTOM_MID, 0, -24);
    lv_obj_set_style_bg_color(g_checkout_ui_payment_invalid.ok_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_invalid.ok_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_invalid.ok_button, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_invalid.ok_button, 7, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_invalid.ok_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_payment_invalid.ok_button,
                        checkout_ui_payment_invalid_ok_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);
    label = lv_label_create(g_checkout_ui_payment_invalid.ok_button);
    lv_label_set_text(label, "鎴戠煡閬撲簡");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);
}

static void checkout_ui_build_payment_timeout_popup(lv_obj_t *panel)
{
    lv_obj_t *label;

    if (panel == NULL)
    {
        return;
    }

    g_checkout_ui_payment_timeout.panel = panel;
    lv_obj_set_style_pad_all(panel, 0, 0);

    g_checkout_ui_payment_timeout.icon_bg = lv_obj_create(panel);
    lv_obj_set_size(g_checkout_ui_payment_timeout.icon_bg, 60, 60);
    lv_obj_align(g_checkout_ui_payment_timeout.icon_bg, LV_ALIGN_TOP_MID, 0, 28);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_timeout.icon_bg, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_timeout.icon_bg, 3, 0);
    lv_obj_set_style_border_color(g_checkout_ui_payment_timeout.icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_ORANGE), 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_timeout.icon_bg, 30, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_timeout.icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_payment_timeout.icon_label = lv_label_create(g_checkout_ui_payment_timeout.icon_bg);
    lv_label_set_text(g_checkout_ui_payment_timeout.icon_label, "L");
    lv_obj_set_style_text_font(g_checkout_ui_payment_timeout.icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_checkout_ui_payment_timeout.icon_label, lv_color_hex(CHECKOUT_UI_COLOR_BTN_ORANGE), 0);
    lv_obj_center(g_checkout_ui_payment_timeout.icon_label);

    g_checkout_ui_payment_timeout.title_label = checkout_ui_create_title_label(panel, "支付超时");
    lv_obj_set_width(g_checkout_ui_payment_timeout.title_label, 280);
    lv_obj_set_style_text_align(g_checkout_ui_payment_timeout.title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_payment_timeout.title_label, LV_ALIGN_TOP_MID, 0, 102);

    g_checkout_ui_payment_timeout.body_label = checkout_ui_create_body_label(panel,
                                                                             "支付时间已超时\n请重新扫码支付");
    lv_obj_set_width(g_checkout_ui_payment_timeout.body_label, 280);
    lv_obj_set_style_text_align(g_checkout_ui_payment_timeout.body_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_payment_timeout.body_label, LV_ALIGN_TOP_MID, 0, 134);

    g_checkout_ui_payment_timeout.cancel_button = lv_btn_create(panel);
    lv_obj_set_size(g_checkout_ui_payment_timeout.cancel_button, 130, 48);
    lv_obj_align(g_checkout_ui_payment_timeout.cancel_button, LV_ALIGN_BOTTOM_MID, -74, -24);
    lv_obj_set_style_bg_color(g_checkout_ui_payment_timeout.cancel_button, lv_color_hex(0xF0F0F0), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_timeout.cancel_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_timeout.cancel_button, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_timeout.cancel_button, 7, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_timeout.cancel_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_payment_timeout.cancel_button,
                        checkout_ui_payment_timeout_cancel_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);
    label = lv_label_create(g_checkout_ui_payment_timeout.cancel_button);
    lv_label_set_text(label, "取消支付");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_TEXT_MAIN), 0);
    lv_obj_center(label);

    g_checkout_ui_payment_timeout.rescan_button = lv_btn_create(panel);
    lv_obj_set_size(g_checkout_ui_payment_timeout.rescan_button, 130, 48);
    lv_obj_align(g_checkout_ui_payment_timeout.rescan_button, LV_ALIGN_BOTTOM_MID, 74, -24);
    lv_obj_set_style_bg_color(g_checkout_ui_payment_timeout.rescan_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_payment_timeout.rescan_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_payment_timeout.rescan_button, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_payment_timeout.rescan_button, 7, 0);
    lv_obj_clear_flag(g_checkout_ui_payment_timeout.rescan_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_payment_timeout.rescan_button,
                        checkout_ui_payment_timeout_rescan_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);
    label = lv_label_create(g_checkout_ui_payment_timeout.rescan_button);
    lv_label_set_text(label, "重新扫码");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);
}

static void checkout_ui_delete_confirm_event_cb(lv_event_t *e)
{
    (void)e;

    if (g_checkout_ui_delete_selected_cb != NULL)
    {
        g_checkout_ui_delete_selected_cb(g_checkout_ui_cart_action_user_data);
    }

    checkout_ui_hide_popup();
}

static void checkout_ui_clear_confirm_event_cb(lv_event_t *e)
{
    (void)e;

    if (g_checkout_ui_clear_cart_cb != NULL)
    {
        g_checkout_ui_clear_cart_cb(g_checkout_ui_cart_action_user_data);
    }

    checkout_ui_hide_popup();
}

static void checkout_ui_checkout_done_ok_event_cb(lv_event_t *e)
{
    (void)e;

    checkout_ui_hide_popup();

    if (g_checkout_ui_checkout_done_cb != NULL)
    {
        g_checkout_ui_checkout_done_cb(g_checkout_ui_cart_action_user_data);
    }

    checkout_ui_main_clear_result();
    checkout_ui_main_set_status("请放置商品");
    checkout_ui_refresh_all((g_checkout_ui_get_cart_cb != NULL) ?
                            g_checkout_ui_get_cart_cb(g_checkout_ui_cart_action_user_data) : NULL);
    checkout_ui_show_main_page();
}

static void checkout_ui_request_checkout(void)
{
    const cart_context_t *cart = NULL;

    if (g_checkout_ui_get_cart_cb != NULL)
    {
        cart = g_checkout_ui_get_cart_cb(g_checkout_ui_cart_action_user_data);
    }

    if ((cart == NULL) || (cart->item_count == 0U))
    {
        checkout_ui_show_empty_checkout_popup();
        return;
    }

    checkout_ui_show_payment_page(cart->total_price_cent);
}

static void checkout_ui_show_empty_checkout_popup(void)
{
    checkout_ui_framework_init();
    checkout_ui_ensure_recog_fail_popup();

    if (g_checkout_ui_recog_fail.title_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_recog_fail.title_label, "购物车为空");
    }
    if (g_checkout_ui_recog_fail.body_label != NULL)
    {
        lv_label_set_text(g_checkout_ui_recog_fail.body_label, "购物车为空，无法结账");
    }

    checkout_ui_show_popup(g_checkout_ui_pages.recog_fail_popup);
}

static void checkout_ui_build_confirm_popup(checkout_ui_confirm_popup_handles_t *handles,
                                            lv_obj_t *panel,
                                            const char *title,
                                            const char *body,
                                            lv_event_cb_t confirm_event_cb)
{
    lv_obj_t *label;

    if ((handles == NULL) || (panel == NULL))
    {
        return;
    }

    handles->panel = panel;

    handles->icon_bg = lv_obj_create(panel);
    lv_obj_set_size(handles->icon_bg, 46, 46);
    lv_obj_align(handles->icon_bg, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(handles->icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_ORANGE), 0);
    lv_obj_set_style_bg_opa(handles->icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(handles->icon_bg, 0, 0);
    lv_obj_set_style_radius(handles->icon_bg, 23, 0);
    lv_obj_clear_flag(handles->icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    handles->icon_label = lv_label_create(handles->icon_bg);
    lv_label_set_text(handles->icon_label, "?");
    lv_obj_set_style_text_font(handles->icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(handles->icon_label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(handles->icon_label);

    handles->title_label = checkout_ui_create_title_label(panel, title);
    lv_obj_set_width(handles->title_label, 280);
    lv_obj_set_style_text_align(handles->title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(handles->title_label, LV_ALIGN_TOP_MID, 0, 58);

    handles->body_label = checkout_ui_create_body_label(panel, body);
    lv_obj_set_width(handles->body_label, 280);
    lv_obj_set_style_text_align(handles->body_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(handles->body_label, LV_ALIGN_TOP_MID, 0, 94);

    handles->cancel_button = lv_btn_create(panel);
    lv_obj_set_size(handles->cancel_button, 110, 44);
    lv_obj_align(handles->cancel_button, LV_ALIGN_BOTTOM_MID, -64, 0);
    lv_obj_set_style_bg_color(handles->cancel_button, lv_color_hex(0xBDBDBD), 0);
    lv_obj_set_style_bg_opa(handles->cancel_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(handles->cancel_button, 0, 0);
    lv_obj_set_style_radius(handles->cancel_button, 8, 0);
    lv_obj_clear_flag(handles->cancel_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(handles->cancel_button, checkout_ui_popup_cancel_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(handles->cancel_button);
    lv_label_set_text(label, "取消");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);

    handles->confirm_button = lv_btn_create(panel);
    lv_obj_set_size(handles->confirm_button, 110, 44);
    lv_obj_align(handles->confirm_button, LV_ALIGN_BOTTOM_MID, 64, 0);
    lv_obj_set_style_bg_color(handles->confirm_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_RED), 0);
    lv_obj_set_style_bg_opa(handles->confirm_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(handles->confirm_button, 0, 0);
    lv_obj_set_style_radius(handles->confirm_button, 8, 0);
    lv_obj_clear_flag(handles->confirm_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(handles->confirm_button, confirm_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(handles->confirm_button);
    lv_label_set_text(label, "确定");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);
}

static void checkout_ui_build_checkout_done_popup(lv_obj_t *panel)
{
    lv_obj_t *label;

    if (panel == NULL)
    {
        return;
    }

    g_checkout_ui_checkout_done.panel = panel;

    g_checkout_ui_checkout_done.icon_bg = lv_obj_create(panel);
    lv_obj_set_size(g_checkout_ui_checkout_done.icon_bg, 56, 56);
    lv_obj_align(g_checkout_ui_checkout_done.icon_bg, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(g_checkout_ui_checkout_done.icon_bg, lv_color_hex(CHECKOUT_UI_COLOR_BTN_GREEN), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_checkout_done.icon_bg, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_checkout_done.icon_bg, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_checkout_done.icon_bg, 28, 0);
    lv_obj_clear_flag(g_checkout_ui_checkout_done.icon_bg, LV_OBJ_FLAG_SCROLLABLE);

    g_checkout_ui_checkout_done.icon_label = lv_label_create(g_checkout_ui_checkout_done.icon_bg);
    lv_label_set_text(g_checkout_ui_checkout_done.icon_label, "V");
    lv_obj_set_style_text_font(g_checkout_ui_checkout_done.icon_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_checkout_ui_checkout_done.icon_label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(g_checkout_ui_checkout_done.icon_label);

    g_checkout_ui_checkout_done.title_label = checkout_ui_create_title_label(panel, "结账完成");
    lv_obj_set_width(g_checkout_ui_checkout_done.title_label, 280);
    lv_obj_set_style_text_align(g_checkout_ui_checkout_done.title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_checkout_done.title_label, LV_ALIGN_TOP_MID, 0, 68);

    g_checkout_ui_checkout_done.body_label = checkout_ui_create_body_label(panel, "感谢使用本系统");
    lv_obj_set_width(g_checkout_ui_checkout_done.body_label, 280);
    lv_obj_set_style_text_align(g_checkout_ui_checkout_done.body_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(g_checkout_ui_checkout_done.body_label, LV_ALIGN_TOP_MID, 0, 100);

    g_checkout_ui_checkout_done.amount_prefix_label = checkout_ui_create_body_label(panel, "本次总金额：");
    lv_obj_align(g_checkout_ui_checkout_done.amount_prefix_label, LV_ALIGN_TOP_MID, -44, 130);

    g_checkout_ui_checkout_done.amount_value_label = checkout_ui_create_title_label(panel, "0.00");
    lv_obj_set_style_text_color(g_checkout_ui_checkout_done.amount_value_label, lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
    lv_obj_align_to(g_checkout_ui_checkout_done.amount_value_label,
                    g_checkout_ui_checkout_done.amount_prefix_label,
                    LV_ALIGN_OUT_RIGHT_MID,
                    4,
                    0);

    g_checkout_ui_checkout_done.amount_unit_label = checkout_ui_create_title_label(panel, "元");
    lv_obj_set_style_text_color(g_checkout_ui_checkout_done.amount_unit_label, lv_color_hex(CHECKOUT_UI_COLOR_AMOUNT_RED), 0);
    lv_obj_align_to(g_checkout_ui_checkout_done.amount_unit_label,
                    g_checkout_ui_checkout_done.amount_value_label,
                    LV_ALIGN_OUT_RIGHT_MID,
                    6,
                    0);

    g_checkout_ui_checkout_done.ok_button = lv_btn_create(panel);
    lv_obj_set_size(g_checkout_ui_checkout_done.ok_button, 220, 44);
    lv_obj_align(g_checkout_ui_checkout_done.ok_button, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(g_checkout_ui_checkout_done.ok_button, lv_color_hex(CHECKOUT_UI_COLOR_BTN_BLUE), 0);
    lv_obj_set_style_bg_opa(g_checkout_ui_checkout_done.ok_button, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_checkout_ui_checkout_done.ok_button, 0, 0);
    lv_obj_set_style_radius(g_checkout_ui_checkout_done.ok_button, 8, 0);
    lv_obj_clear_flag(g_checkout_ui_checkout_done.ok_button, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(g_checkout_ui_checkout_done.ok_button, checkout_ui_checkout_done_ok_event_cb, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(g_checkout_ui_checkout_done.ok_button);
    lv_label_set_text(label, "确定");
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(CHECKOUT_UI_COLOR_CARD_BG), 0);
    lv_obj_center(label);
}

static void checkout_ui_format_money(uint32_t cent, char *buffer, uint32_t size)
{
    if ((buffer == NULL) || (size == 0U))
    {
        return;
    }

    (void)snprintf(buffer, size, "%lu.%02lu",
                   (unsigned long)(cent / 100U),
                   (unsigned long)(cent % 100U));
}
