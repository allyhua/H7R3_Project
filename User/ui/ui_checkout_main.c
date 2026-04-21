#include "ui/ui_checkout_main.h"

#include "ui/ui_dialog.h"
#include "lvgl.h"
#include "app/product_catalog.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

LV_FONT_DECLARE(lv_font_montserrat_20);
LV_FONT_DECLARE(lv_font_montserrat_32);

#define UI_BASE_W                480
#define UI_BASE_H                800

#define UI_COLOR_BG              0xF2F3F5
#define UI_COLOR_PRIMARY         0x174E5C
#define UI_COLOR_CARD            0xFFFFFF
#define UI_COLOR_DIVIDER         0xE0E0E0
#define UI_COLOR_TEXT            0x222222
#define UI_COLOR_TEXT_SECONDARY  0x666666
#define UI_COLOR_ACCENT_RED      0xE53935
#define UI_COLOR_BTN_BLUE        0x1E88E5
#define UI_COLOR_BTN_BLUE_PRESS  0x1565C0
#define UI_COLOR_BTN_RED         0xE53935
#define UI_COLOR_BTN_RED_PRESS   0xB71C1C
#define UI_COLOR_BTN_GREEN       0x43A047
#define UI_COLOR_BTN_GREEN_PRESS 0x2E7D32
#define UI_COLOR_SELECTED        0xE3F2FD
#define UI_COLOR_IMAGE_BG        0xDADDE1

#define UI_STATUS_BAR_H          56
#define UI_INFO_BAR_X            15
#define UI_INFO_BAR_Y            66
#define UI_INFO_BAR_W            450
#define UI_INFO_BAR_H            46

#define UI_IMAGE_CARD_X          15
#define UI_IMAGE_CARD_Y          122
#define UI_IMAGE_CARD_W          180
#define UI_IMAGE_CARD_H          178

#define UI_RESULT_CARD_X         205
#define UI_RESULT_CARD_Y         122
#define UI_RESULT_CARD_W         260
#define UI_RESULT_CARD_H         178

#define UI_CART_CARD_X           15
#define UI_CART_CARD_Y           310
#define UI_CART_CARD_W           450
#define UI_CART_CARD_H           270
#define UI_CART_HEADER_H         40

#define UI_TOTAL_CARD_X          15
#define UI_TOTAL_CARD_Y          590
#define UI_TOTAL_CARD_W          450
#define UI_TOTAL_CARD_H          64

#define UI_BUTTON_W              215
#define UI_BUTTON_H              50
#define UI_BUTTON_1_X            15
#define UI_BUTTON_1_Y            666
#define UI_BUTTON_2_X            250
#define UI_BUTTON_2_Y            666
#define UI_BUTTON_3_X            15
#define UI_BUTTON_3_Y            732
#define UI_BUTTON_4_X            250
#define UI_BUTTON_4_Y            732

typedef struct
{
    camera_workflow_state_t *workflow;
    lv_obj_t *screen;
    lv_obj_t *status_label;
    lv_obj_t *weight_info_label;
    lv_obj_t *preview_image;
    lv_obj_t *preview_placeholder;
    lv_obj_t *result_name_label;
    lv_obj_t *result_price_label;
    lv_obj_t *result_amount_label;
    lv_obj_t *result_subtotal_label;
    lv_obj_t *cart_scroll;
    lv_obj_t *cart_rows[PRODUCT_ID_COUNT];
    lv_obj_t *total_value_label;
    lv_obj_t *total_unit_label;
    lv_img_dsc_t preview_dsc;
    uint32_t screen_w;
    uint32_t screen_h;
    uint32_t last_capture_counter;
    uint32_t last_total_cent;
    uint32_t last_item_count;
    int32_t last_selected_index;
} checkout_ui_context_t;

static checkout_ui_context_t g_ui;

static lv_coord_t checkout_ui_scale_x(lv_coord_t value);
static lv_coord_t checkout_ui_scale_y(lv_coord_t value);
static lv_obj_t *checkout_ui_create_card(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h);
static void checkout_ui_apply_font(lv_obj_t *obj);
static void checkout_ui_set_button_style(lv_obj_t *button, lv_color_t bg_color, lv_color_t pressed_color);
static void checkout_ui_align_total_unit(void);
static void checkout_ui_format_money(uint32_t cent, char *buffer, uint32_t size);
static void checkout_ui_format_weight_info(uint32_t weight_gram,
                                           uint8_t latency_valid,
                                           uint32_t recognition_time_ms_x10,
                                           char *buffer,
                                           uint32_t size);
static const char *checkout_ui_status_text(const camera_workflow_state_t *state);
static void checkout_ui_update_preview(void);
static void checkout_ui_show_notice(camera_workflow_state_t *state);
static void checkout_ui_build_layout(void);
static void checkout_ui_update_row_selection(int32_t selected_index);
static void checkout_ui_row_event_cb(lv_event_t *e);
static void checkout_ui_finish_event_cb(lv_event_t *e);
static void checkout_ui_delete_event_cb(lv_event_t *e);
static void checkout_ui_clear_event_cb(lv_event_t *e);
static void checkout_ui_continue_event_cb(lv_event_t *e);
static void ui_checkout_refresh_all(camera_workflow_state_t *state);

static lv_coord_t checkout_ui_scale_x(lv_coord_t value)
{
    return (lv_coord_t)((int32_t)value * (int32_t)g_ui.screen_w / UI_BASE_W);
}

static lv_coord_t checkout_ui_scale_y(lv_coord_t value)
{
    return (lv_coord_t)((int32_t)value * (int32_t)g_ui.screen_h / UI_BASE_H);
}

static lv_obj_t *checkout_ui_create_card(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_size(card, w, h);
    lv_obj_set_style_radius(card, checkout_ui_scale_y(10), 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(UI_COLOR_CARD), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(UI_COLOR_DIVIDER), 0);
    lv_obj_set_style_pad_all(card, 0, 0);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
    return card;
}

static void checkout_ui_apply_font(lv_obj_t *obj)
{
    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, 0);
}

static void checkout_ui_set_button_style(lv_obj_t *button, lv_color_t bg_color, lv_color_t pressed_color)
{
    lv_obj_set_style_radius(button, checkout_ui_scale_y(10), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(button, checkout_ui_scale_y(10), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(button, bg_color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(button, pressed_color, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(button, 0, LV_PART_MAIN | LV_STATE_PRESSED);
}

static void checkout_ui_align_total_unit(void)
{
    lv_obj_align_to(g_ui.total_unit_label, g_ui.total_value_label, LV_ALIGN_OUT_RIGHT_MID, checkout_ui_scale_x(6), 0);
}

static void checkout_ui_format_money(uint32_t cent, char *buffer, uint32_t size)
{
    snprintf(buffer,
             size,
             "%lu.%02lu",
             (unsigned long)(cent / 100U),
             (unsigned long)(cent % 100U));
}

static void checkout_ui_format_weight_info(uint32_t weight_gram,
                                           uint8_t latency_valid,
                                           uint32_t recognition_time_ms_x10,
                                           char *buffer,
                                           uint32_t size)
{
    if (latency_valid != 0U)
    {
        uint32_t seconds_x10 = (recognition_time_ms_x10 + 500U) / 1000U;
        snprintf(buffer,
                 size,
                 "Weight: %lu.%03lu kg | Time: %lu.%01lu s",
                 (unsigned long)(weight_gram / 1000U),
                 (unsigned long)(weight_gram % 1000U),
                 (unsigned long)(seconds_x10 / 10U),
                 (unsigned long)(seconds_x10 % 10U));
    }
    else
    {
        snprintf(buffer,
                 size,
                 "Weight: %lu.%03lu kg | Time: --",
                 (unsigned long)(weight_gram / 1000U),
                 (unsigned long)(weight_gram % 1000U));
    }
}

static const char *checkout_ui_status_text(const camera_workflow_state_t *state)
{
    if (state == NULL)
    {
        return "Place item";
    }

    switch (state->trigger_state)
    {
        case CAMERA_TRIGGER_EMPTY:
            return "Place item";

        case CAMERA_TRIGGER_DETECT_PLACEMENT:
            return "Item detected";

        case CAMERA_TRIGGER_WAIT_STABLE:
            return "Waiting for stable weight";

        case CAMERA_TRIGGER_RECOGNIZING:
            return "Stable weight, recognizing";

        case CAMERA_TRIGGER_WAIT_REMOVE:
            if (state->last_cart_add.valid != 0U)
            {
                return "Remove item to continue";
            }
            return "Recognition failed";

        default:
            return "Place item";
    }
}

static void checkout_ui_update_preview(void)
{
    lv_coord_t box_w;
    lv_coord_t box_h;
    uint32_t zoom_x;
    uint32_t zoom_y;
    uint32_t zoom;

    if (g_ui.workflow == NULL)
    {
        return;
    }

    if (g_ui.workflow->capture_counter == 0U)
    {
        lv_obj_clear_flag(g_ui.preview_placeholder, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_add_flag(g_ui.preview_placeholder, LV_OBJ_FLAG_HIDDEN);

    if (g_ui.last_capture_counter == g_ui.workflow->capture_counter)
    {
        return;
    }

    box_w = lv_obj_get_width(lv_obj_get_parent(g_ui.preview_image)) - checkout_ui_scale_x(8);
    box_h = lv_obj_get_height(lv_obj_get_parent(g_ui.preview_image)) - checkout_ui_scale_y(8);

    zoom_x = (uint32_t)box_w * 256U / g_ui.workflow->frame_width;
    zoom_y = (uint32_t)box_h * 256U / g_ui.workflow->frame_height;
    zoom = (zoom_x < zoom_y) ? zoom_x : zoom_y;
    if (zoom > 256U)
    {
        zoom = 256U;
    }
    if (zoom < 64U)
    {
        zoom = 64U;
    }

    lv_img_set_src(g_ui.preview_image, &g_ui.preview_dsc);
    lv_img_set_zoom(g_ui.preview_image, (uint16_t)zoom);
    lv_obj_center(g_ui.preview_image);
    g_ui.last_capture_counter = g_ui.workflow->capture_counter;
}

static void checkout_ui_show_notice(camera_workflow_state_t *state)
{
    camera_notice_type_t notice_type;
    char notice_text[64];

    if (camera_workflow_pop_notice(state, &notice_type, notice_text, sizeof(notice_text)) == 0)
    {
        return;
    }

    if (notice_type == CAMERA_NOTICE_RECOGNIZE_FAIL)
    {
        checkout_ui_show_error("Recognition Failed", "Remove the item and try again.");
    }
    else
    {
        checkout_ui_show_error("Notice", "Please try again.");
    }
}

static void checkout_ui_update_row_selection(int32_t selected_index)
{
    uint32_t idx;

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        if (g_ui.cart_rows[idx] == NULL)
        {
            continue;
        }

        lv_obj_set_style_bg_color(g_ui.cart_rows[idx],
                                  ((int32_t)idx == selected_index) ? lv_color_hex(UI_COLOR_SELECTED)
                                                                   : lv_color_hex(UI_COLOR_CARD),
                                  0);
        lv_obj_set_style_bg_opa(g_ui.cart_rows[idx], LV_OPA_COVER, 0);
    }

    g_ui.last_selected_index = selected_index;
}

static void checkout_ui_build_layout(void)
{
    lv_obj_t *label;
    lv_obj_t *line;
    lv_obj_t *button;
    lv_obj_t *button_label;
    lv_obj_t *image_card;
    lv_obj_t *image_box;
    lv_obj_t *result_card;
    lv_obj_t *cart_card;
    lv_obj_t *total_card;
    lv_obj_t *status_bar;
    lv_obj_t *info_bar;

    g_ui.screen = lv_obj_create(NULL);
    lv_obj_set_size(g_ui.screen, (lv_coord_t)g_ui.screen_w, (lv_coord_t)g_ui.screen_h);
    lv_obj_set_style_radius(g_ui.screen, 0, 0);
    lv_obj_set_style_border_width(g_ui.screen, 0, 0);
    lv_obj_set_style_bg_color(g_ui.screen, lv_color_hex(UI_COLOR_BG), 0);
    lv_obj_set_style_bg_opa(g_ui.screen, LV_OPA_COVER, 0);
    lv_obj_clear_flag(g_ui.screen, LV_OBJ_FLAG_SCROLLABLE);

    status_bar = lv_obj_create(g_ui.screen);
    lv_obj_set_pos(status_bar, 0, 0);
    lv_obj_set_size(status_bar, (lv_coord_t)g_ui.screen_w, checkout_ui_scale_y(UI_STATUS_BAR_H));
    lv_obj_set_style_radius(status_bar, 0, 0);
    lv_obj_set_style_border_width(status_bar, 0, 0);
    lv_obj_set_style_bg_color(status_bar, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_bg_opa(status_bar, LV_OPA_COVER, 0);
    lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_SCROLLABLE);

    g_ui.status_label = lv_label_create(status_bar);
    checkout_ui_apply_font(g_ui.status_label);
    lv_obj_set_width(g_ui.status_label, (lv_coord_t)g_ui.screen_w - checkout_ui_scale_x(24));
    lv_obj_set_style_text_color(g_ui.status_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_align(g_ui.status_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(g_ui.status_label, LV_LABEL_LONG_CLIP);
    lv_obj_center(g_ui.status_label);

    info_bar = lv_obj_create(g_ui.screen);
    lv_obj_set_pos(info_bar, checkout_ui_scale_x(UI_INFO_BAR_X), checkout_ui_scale_y(UI_INFO_BAR_Y));
    lv_obj_set_size(info_bar, checkout_ui_scale_x(UI_INFO_BAR_W), checkout_ui_scale_y(UI_INFO_BAR_H));
    lv_obj_set_style_radius(info_bar, checkout_ui_scale_y(8), 0);
    lv_obj_set_style_border_width(info_bar, 0, 0);
    lv_obj_set_style_bg_color(info_bar, lv_color_hex(UI_COLOR_PRIMARY), 0);
    lv_obj_set_style_bg_opa(info_bar, LV_OPA_COVER, 0);
    lv_obj_clear_flag(info_bar, LV_OBJ_FLAG_SCROLLABLE);

    g_ui.weight_info_label = lv_label_create(info_bar);
    checkout_ui_apply_font(g_ui.weight_info_label);
    lv_obj_set_width(g_ui.weight_info_label, checkout_ui_scale_x(UI_INFO_BAR_W - 20));
    lv_obj_set_style_text_color(g_ui.weight_info_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_align(g_ui.weight_info_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(g_ui.weight_info_label);

    image_card = checkout_ui_create_card(g_ui.screen,
                                         checkout_ui_scale_x(UI_IMAGE_CARD_X),
                                         checkout_ui_scale_y(UI_IMAGE_CARD_Y),
                                         checkout_ui_scale_x(UI_IMAGE_CARD_W),
                                         checkout_ui_scale_y(UI_IMAGE_CARD_H));
    image_box = lv_obj_create(image_card);
    lv_obj_set_pos(image_box, checkout_ui_scale_x(6), checkout_ui_scale_y(6));
    lv_obj_set_size(image_box,
                    checkout_ui_scale_x(UI_IMAGE_CARD_W - 12),
                    checkout_ui_scale_y(UI_IMAGE_CARD_H - 12));
    lv_obj_set_style_radius(image_box, checkout_ui_scale_y(8), 0);
    lv_obj_set_style_border_width(image_box, 0, 0);
    lv_obj_set_style_bg_color(image_box, lv_color_hex(UI_COLOR_IMAGE_BG), 0);
    lv_obj_set_style_bg_opa(image_box, LV_OPA_COVER, 0);
    lv_obj_clear_flag(image_box, LV_OBJ_FLAG_SCROLLABLE);

    g_ui.preview_image = lv_img_create(image_box);
    lv_obj_center(g_ui.preview_image);

    g_ui.preview_placeholder = lv_label_create(image_box);
    checkout_ui_apply_font(g_ui.preview_placeholder);
    lv_obj_set_style_text_color(g_ui.preview_placeholder, lv_color_hex(UI_COLOR_TEXT_SECONDARY), 0);
    lv_label_set_text(g_ui.preview_placeholder, "Image Area");
    lv_obj_center(g_ui.preview_placeholder);

    result_card = checkout_ui_create_card(g_ui.screen,
                                          checkout_ui_scale_x(UI_RESULT_CARD_X),
                                          checkout_ui_scale_y(UI_RESULT_CARD_Y),
                                          checkout_ui_scale_x(UI_RESULT_CARD_W),
                                          checkout_ui_scale_y(UI_RESULT_CARD_H));

    g_ui.result_name_label = lv_label_create(result_card);
    checkout_ui_apply_font(g_ui.result_name_label);
    lv_obj_set_width(g_ui.result_name_label, checkout_ui_scale_x(UI_RESULT_CARD_W - 32));
    lv_obj_set_style_text_color(g_ui.result_name_label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_label_set_long_mode(g_ui.result_name_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_pos(g_ui.result_name_label, checkout_ui_scale_x(16), checkout_ui_scale_y(16));

    g_ui.result_price_label = lv_label_create(result_card);
    checkout_ui_apply_font(g_ui.result_price_label);
    lv_obj_set_width(g_ui.result_price_label, checkout_ui_scale_x(UI_RESULT_CARD_W - 32));
    lv_obj_set_style_text_color(g_ui.result_price_label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_label_set_long_mode(g_ui.result_price_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_pos(g_ui.result_price_label, checkout_ui_scale_x(16), checkout_ui_scale_y(56));

    g_ui.result_amount_label = lv_label_create(result_card);
    checkout_ui_apply_font(g_ui.result_amount_label);
    lv_obj_set_width(g_ui.result_amount_label, checkout_ui_scale_x(UI_RESULT_CARD_W - 32));
    lv_obj_set_style_text_color(g_ui.result_amount_label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_label_set_long_mode(g_ui.result_amount_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_pos(g_ui.result_amount_label, checkout_ui_scale_x(16), checkout_ui_scale_y(96));

    g_ui.result_subtotal_label = lv_label_create(result_card);
    checkout_ui_apply_font(g_ui.result_subtotal_label);
    lv_obj_set_width(g_ui.result_subtotal_label, checkout_ui_scale_x(UI_RESULT_CARD_W - 32));
    lv_obj_set_style_text_color(g_ui.result_subtotal_label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_label_set_long_mode(g_ui.result_subtotal_label, LV_LABEL_LONG_CLIP);
    lv_obj_set_pos(g_ui.result_subtotal_label, checkout_ui_scale_x(16), checkout_ui_scale_y(136));

    cart_card = checkout_ui_create_card(g_ui.screen,
                                        checkout_ui_scale_x(UI_CART_CARD_X),
                                        checkout_ui_scale_y(UI_CART_CARD_Y),
                                        checkout_ui_scale_x(UI_CART_CARD_W),
                                        checkout_ui_scale_y(UI_CART_CARD_H));

    label = lv_label_create(cart_card);
    checkout_ui_apply_font(label);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_label_set_text(label, "Cart");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, checkout_ui_scale_y(10));

    line = lv_obj_create(cart_card);
    lv_obj_set_pos(line, 0, checkout_ui_scale_y(UI_CART_HEADER_H - 1));
    lv_obj_set_size(line, checkout_ui_scale_x(UI_CART_CARD_W), 1);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_set_style_radius(line, 0, 0);
    lv_obj_set_style_bg_color(line, lv_color_hex(UI_COLOR_DIVIDER), 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

    g_ui.cart_scroll = lv_obj_create(cart_card);
    lv_obj_set_pos(g_ui.cart_scroll, checkout_ui_scale_x(8), checkout_ui_scale_y(UI_CART_HEADER_H + 4));
    lv_obj_set_size(g_ui.cart_scroll,
                    checkout_ui_scale_x(UI_CART_CARD_W - 16),
                    checkout_ui_scale_y(UI_CART_CARD_H - UI_CART_HEADER_H - 12));
    lv_obj_set_style_radius(g_ui.cart_scroll, checkout_ui_scale_y(8), 0);
    lv_obj_set_style_border_width(g_ui.cart_scroll, 0, 0);
    lv_obj_set_style_bg_color(g_ui.cart_scroll, lv_color_hex(UI_COLOR_CARD), 0);
    lv_obj_set_style_pad_all(g_ui.cart_scroll, checkout_ui_scale_x(8), 0);
    lv_obj_set_style_pad_row(g_ui.cart_scroll, checkout_ui_scale_y(8), 0);
    lv_obj_set_layout(g_ui.cart_scroll, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(g_ui.cart_scroll, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(g_ui.cart_scroll, LV_DIR_VER);
    lv_obj_set_style_bg_opa(g_ui.cart_scroll, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_top(g_ui.cart_scroll, checkout_ui_scale_y(4), 0);
    lv_obj_set_style_pad_bottom(g_ui.cart_scroll, checkout_ui_scale_y(4), 0);
    lv_obj_set_style_bg_color(g_ui.cart_scroll, lv_color_hex(UI_COLOR_CARD), LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_opa(g_ui.cart_scroll, LV_OPA_40, LV_PART_SCROLLBAR);
    lv_obj_set_style_radius(g_ui.cart_scroll, checkout_ui_scale_y(6), LV_PART_SCROLLBAR);
    lv_obj_set_style_width(g_ui.cart_scroll, checkout_ui_scale_x(6), LV_PART_SCROLLBAR);

    total_card = checkout_ui_create_card(g_ui.screen,
                                         checkout_ui_scale_x(UI_TOTAL_CARD_X),
                                         checkout_ui_scale_y(UI_TOTAL_CARD_Y),
                                         checkout_ui_scale_x(UI_TOTAL_CARD_W),
                                         checkout_ui_scale_y(UI_TOTAL_CARD_H));

    label = lv_label_create(total_card);
    checkout_ui_apply_font(label);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_label_set_text(label, "Total:");
    lv_obj_set_pos(label, checkout_ui_scale_x(20), checkout_ui_scale_y(18));

    g_ui.total_value_label = lv_label_create(total_card);
    lv_obj_set_style_text_font(g_ui.total_value_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_ui.total_value_label, lv_color_hex(UI_COLOR_ACCENT_RED), 0);
    lv_obj_set_pos(g_ui.total_value_label, checkout_ui_scale_x(130), checkout_ui_scale_y(8));

    g_ui.total_unit_label = lv_label_create(total_card);
    checkout_ui_apply_font(g_ui.total_unit_label);
    lv_obj_set_style_text_color(g_ui.total_unit_label, lv_color_hex(UI_COLOR_ACCENT_RED), 0);
    lv_label_set_text(g_ui.total_unit_label, "yuan");

    button = lv_btn_create(g_ui.screen);
    lv_obj_set_pos(button, checkout_ui_scale_x(UI_BUTTON_1_X), checkout_ui_scale_y(UI_BUTTON_1_Y));
    lv_obj_set_size(button, checkout_ui_scale_x(UI_BUTTON_W), checkout_ui_scale_y(UI_BUTTON_H));
    checkout_ui_set_button_style(button, lv_color_hex(UI_COLOR_BTN_BLUE), lv_color_hex(UI_COLOR_BTN_BLUE_PRESS));
    lv_obj_add_event_cb(button, checkout_ui_finish_event_cb, LV_EVENT_CLICKED, NULL);
    button_label = lv_label_create(button);
    checkout_ui_apply_font(button_label);
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(button_label, "Checkout");
    lv_obj_center(button_label);

    button = lv_btn_create(g_ui.screen);
    lv_obj_set_pos(button, checkout_ui_scale_x(UI_BUTTON_2_X), checkout_ui_scale_y(UI_BUTTON_2_Y));
    lv_obj_set_size(button, checkout_ui_scale_x(UI_BUTTON_W), checkout_ui_scale_y(UI_BUTTON_H));
    checkout_ui_set_button_style(button, lv_color_hex(UI_COLOR_BTN_RED), lv_color_hex(UI_COLOR_BTN_RED_PRESS));
    lv_obj_add_event_cb(button, checkout_ui_delete_event_cb, LV_EVENT_CLICKED, NULL);
    button_label = lv_label_create(button);
    checkout_ui_apply_font(button_label);
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(button_label, "Remove Selected");
    lv_obj_center(button_label);

    button = lv_btn_create(g_ui.screen);
    lv_obj_set_pos(button, checkout_ui_scale_x(UI_BUTTON_3_X), checkout_ui_scale_y(UI_BUTTON_3_Y));
    lv_obj_set_size(button, checkout_ui_scale_x(UI_BUTTON_W), checkout_ui_scale_y(UI_BUTTON_H));
    checkout_ui_set_button_style(button, lv_color_hex(UI_COLOR_BTN_RED), lv_color_hex(UI_COLOR_BTN_RED_PRESS));
    lv_obj_add_event_cb(button, checkout_ui_clear_event_cb, LV_EVENT_CLICKED, NULL);
    button_label = lv_label_create(button);
    checkout_ui_apply_font(button_label);
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(button_label, "Clear Cart");
    lv_obj_center(button_label);

    button = lv_btn_create(g_ui.screen);
    lv_obj_set_pos(button, checkout_ui_scale_x(UI_BUTTON_4_X), checkout_ui_scale_y(UI_BUTTON_4_Y));
    lv_obj_set_size(button, checkout_ui_scale_x(UI_BUTTON_W), checkout_ui_scale_y(UI_BUTTON_H));
    checkout_ui_set_button_style(button, lv_color_hex(UI_COLOR_BTN_GREEN), lv_color_hex(UI_COLOR_BTN_GREEN_PRESS));
    lv_obj_add_event_cb(button, checkout_ui_continue_event_cb, LV_EVENT_CLICKED, NULL);
    button_label = lv_label_create(button);
    checkout_ui_apply_font(button_label);
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(button_label, "Continue");
    lv_obj_center(button_label);
}

void checkout_ui_init(camera_workflow_state_t *state)
{
    memset(&g_ui, 0, sizeof(g_ui));
    g_ui.workflow = state;
    g_ui.screen_w = (uint32_t)lv_disp_get_hor_res(lv_disp_get_default());
    g_ui.screen_h = (uint32_t)lv_disp_get_ver_res(lv_disp_get_default());
    g_ui.last_selected_index = -2;
    g_ui.last_total_cent = 0xFFFFFFFFU;
    g_ui.last_item_count = 0xFFFFFFFFU;

    g_ui.preview_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    g_ui.preview_dsc.header.w = state->frame_width;
    g_ui.preview_dsc.header.h = state->frame_height;
    g_ui.preview_dsc.data_size = (uint32_t)state->frame_width * (uint32_t)state->frame_height * 2U;
    g_ui.preview_dsc.data = (const uint8_t *)camera_workflow_get_frame_buffer();

    checkout_ui_build_layout();
    lv_scr_load(g_ui.screen);
    ui_checkout_refresh_all(state);
}

void checkout_ui_set_status(const char *status_text)
{
    if (g_ui.status_label == NULL)
    {
        return;
    }

    lv_label_set_text(g_ui.status_label, (status_text != NULL) ? status_text : "--");
}

void checkout_ui_set_weight_info(uint32_t weight_gram,
                                 uint8_t latency_valid,
                                 uint32_t recognition_time_ms_x10)
{
    char info_text[96];

    if (g_ui.weight_info_label == NULL)
    {
        return;
    }

    checkout_ui_format_weight_info(weight_gram,
                                   latency_valid,
                                   recognition_time_ms_x10,
                                   info_text,
                                   sizeof(info_text));
    lv_label_set_text(g_ui.weight_info_label, info_text);
}

void checkout_ui_set_recognition_result(const cart_add_result_t *result)
{
    char text[96];
    const product_info_t *product;

    if ((result == NULL) || (result->valid == 0U))
    {
        checkout_ui_clear_recognition_result();
        return;
    }

    product = product_catalog_get(result->product_id);

    snprintf(text, sizeof(text), "Item: %s", result->product_name);
    lv_label_set_text(g_ui.result_name_label, text);

    if (product != NULL)
    {
        snprintf(text,
                 sizeof(text),
                 "Price: %lu.%02lu %s",
                 (unsigned long)(product->unit_price_cent / 100U),
                 (unsigned long)(product->unit_price_cent % 100U),
                 product->price_unit_text);
    }
    else
    {
        snprintf(text, sizeof(text), "Price: --");
    }
    lv_label_set_text(g_ui.result_price_label, text);

    if (result->pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
    {
        snprintf(text,
                 sizeof(text),
                 "Weight: %lu.%03lu kg",
                 (unsigned long)(result->measured_weight_g / 1000U),
                 (unsigned long)(result->measured_weight_g % 1000U));
    }
    else
    {
        snprintf(text, sizeof(text), "Qty: %lu pcs", (unsigned long)result->counted_units);
    }
    lv_label_set_text(g_ui.result_amount_label, text);

    snprintf(text,
             sizeof(text),
             "Subtotal: %lu.%02lu",
             (unsigned long)(result->subtotal_cent / 100U),
             (unsigned long)(result->subtotal_cent % 100U));
    lv_label_set_text(g_ui.result_subtotal_label, text);
}

void checkout_ui_clear_recognition_result(void)
{
    lv_label_set_text(g_ui.result_name_label, "Item: --");
    lv_label_set_text(g_ui.result_price_label, "Price: --");
    lv_label_set_text(g_ui.result_amount_label, "Weight/Qty: --");
    lv_label_set_text(g_ui.result_subtotal_label, "Subtotal: --");
}

void checkout_ui_refresh_cart(const cart_context_t *cart, int32_t selected_index)
{
    uint32_t idx;
    uint32_t valid_count = 0U;

    if ((cart == NULL) || (g_ui.cart_scroll == NULL))
    {
        return;
    }

    if ((g_ui.last_total_cent == cart->total_price_cent) &&
        (g_ui.last_item_count == cart->item_count))
    {
        if (g_ui.last_selected_index != selected_index)
        {
            checkout_ui_update_row_selection(selected_index);
        }
        return;
    }

    lv_obj_clean(g_ui.cart_scroll);
    memset(g_ui.cart_rows, 0, sizeof(g_ui.cart_rows));

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        const cart_item_t *item = &cart->items[idx];
        const product_info_t *product = product_catalog_get(item->product_id);
        lv_obj_t *row;
        lv_obj_t *top_name;
        lv_obj_t *top_total;
        lv_obj_t *bottom_amount;
        lv_obj_t *bottom_price;
        lv_obj_t *divider;
        lv_coord_t row_width;
        char amount_text[32];
        char price_text[32];
        char total_text[24];

        if (item->line_valid == 0U)
        {
            continue;
        }

        valid_count++;
        row = lv_obj_create(g_ui.cart_scroll);
        row_width = lv_obj_get_content_width(g_ui.cart_scroll);
        lv_obj_set_width(row, row_width);
        lv_obj_set_height(row, checkout_ui_scale_y(64));
        lv_obj_set_style_radius(row, checkout_ui_scale_y(8), 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_bg_color(row,
                                  ((int32_t)idx == selected_index) ? lv_color_hex(UI_COLOR_SELECTED)
                                                                   : lv_color_hex(UI_COLOR_CARD),
                                  0);
        lv_obj_set_style_bg_opa(row, LV_OPA_COVER, 0);
        lv_obj_set_style_pad_all(row, 0, 0);
        lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(row, checkout_ui_row_event_cb, LV_EVENT_PRESSED, (void *)(uintptr_t)idx);
        g_ui.cart_rows[idx] = row;

        top_name = lv_label_create(row);
        checkout_ui_apply_font(top_name);
        lv_obj_set_width(top_name, row_width - checkout_ui_scale_x(160));
        lv_obj_set_style_text_color(top_name, lv_color_hex(UI_COLOR_TEXT), 0);
        lv_label_set_long_mode(top_name, LV_LABEL_LONG_CLIP);
        lv_label_set_text(top_name,
                          ((item->product_name[0] != '\0') ? item->product_name
                                                           : ((product != NULL) ? product->product_name : "--")));
        lv_obj_set_pos(top_name, checkout_ui_scale_x(8), checkout_ui_scale_y(4));

        checkout_ui_format_money(item->subtotal_cent, total_text, sizeof(total_text));
        top_total = lv_label_create(row);
        checkout_ui_apply_font(top_total);
        lv_obj_set_style_text_color(top_total, lv_color_hex(UI_COLOR_TEXT), 0);
        lv_label_set_text_fmt(top_total, "%s", total_text);
        lv_obj_align(top_total, LV_ALIGN_TOP_RIGHT, -checkout_ui_scale_x(8), checkout_ui_scale_y(4));

        if (item->pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
        {
            snprintf(amount_text,
                     sizeof(amount_text),
                     "%lu.%03lu kg",
                     (unsigned long)(item->total_weight_g / 1000U),
                     (unsigned long)(item->total_weight_g % 1000U));
        }
        else
        {
            snprintf(amount_text,
                     sizeof(amount_text),
                     "%lu %s",
                     (unsigned long)item->total_count,
                     (product != NULL) ? product->display_unit_text : "pc");
        }

        snprintf(price_text,
                 sizeof(price_text),
                 "%lu.%02lu %s",
                 (unsigned long)(item->unit_price_cent / 100U),
                 (unsigned long)(item->unit_price_cent % 100U),
                 (product != NULL) ? product->price_unit_text
                                   : ((item->pricing_mode == PRODUCT_PRICING_BY_WEIGHT) ? "/kg" : "/pc"));

        bottom_amount = lv_label_create(row);
        checkout_ui_apply_font(bottom_amount);
        lv_obj_set_style_text_color(bottom_amount, lv_color_hex(UI_COLOR_TEXT_SECONDARY), 0);
        lv_label_set_text(bottom_amount, amount_text);
        lv_obj_set_pos(bottom_amount, checkout_ui_scale_x(8), checkout_ui_scale_y(34));

        bottom_price = lv_label_create(row);
        checkout_ui_apply_font(bottom_price);
        lv_obj_set_style_text_color(bottom_price, lv_color_hex(UI_COLOR_TEXT_SECONDARY), 0);
        lv_label_set_text(bottom_price, price_text);
        lv_obj_align(bottom_price, LV_ALIGN_BOTTOM_RIGHT, -checkout_ui_scale_x(8), -checkout_ui_scale_y(8));

        divider = lv_obj_create(row);
        lv_obj_set_pos(divider, checkout_ui_scale_x(8), checkout_ui_scale_y(63));
        lv_obj_set_size(divider, row_width - checkout_ui_scale_x(16), 1);
        lv_obj_set_style_radius(divider, 0, 0);
        lv_obj_set_style_border_width(divider, 0, 0);
        lv_obj_set_style_bg_color(divider, lv_color_hex(UI_COLOR_DIVIDER), 0);
        lv_obj_clear_flag(divider, LV_OBJ_FLAG_SCROLLABLE);
    }

    if (valid_count == 0U)
    {
        lv_obj_t *empty_label = lv_label_create(g_ui.cart_scroll);
        checkout_ui_apply_font(empty_label);
        lv_obj_set_style_text_color(empty_label, lv_color_hex(0x999999), 0);
        lv_label_set_text(empty_label, "Cart is empty");
        lv_obj_center(empty_label);
    }

    g_ui.last_total_cent = cart->total_price_cent;
    g_ui.last_item_count = cart->item_count;
    checkout_ui_update_row_selection(selected_index);
}

void checkout_ui_set_total_price(uint32_t total_price_cent)
{
    char value_text[24];

    checkout_ui_format_money(total_price_cent, value_text, sizeof(value_text));
    lv_label_set_text(g_ui.total_value_label, value_text);
    checkout_ui_align_total_unit();
}

void checkout_ui_show_error(const char *title, const char *message)
{
    ui_dialog_show_message(title, message);
}

void checkout_ui_show_checkout_done(uint32_t total_price_cent)
{
    char message[64];

    snprintf(message,
             sizeof(message),
             "Thank you. Paid %lu.%02lu yuan.",
             (unsigned long)(total_price_cent / 100U),
             (unsigned long)(total_price_cent % 100U));
    ui_dialog_show_message("Checkout Done", message);
}

static void checkout_ui_row_event_cb(lv_event_t *e)
{
    int32_t selected_index = (int32_t)(uintptr_t)lv_event_get_user_data(e);

    if (g_ui.workflow == NULL)
    {
        return;
    }

    camera_workflow_select_cart_item(g_ui.workflow, selected_index);
    checkout_ui_update_row_selection(g_ui.workflow->cart.selected_item_index);
}

static void checkout_ui_finish_event_cb(lv_event_t *e)
{
    uint32_t total_cent;
    (void)e;

    if ((g_ui.workflow == NULL) || (g_ui.workflow->cart.item_count == 0U))
    {
        checkout_ui_show_error("Checkout", "Cart is empty.");
        return;
    }

    total_cent = g_ui.workflow->cart.total_price_cent;
    camera_workflow_complete_checkout(g_ui.workflow);
    checkout_ui_show_checkout_done(total_cent);
    g_ui.last_total_cent = 0xFFFFFFFFU;
    ui_checkout_refresh_all(g_ui.workflow);
}

static void checkout_ui_delete_event_cb(lv_event_t *e)
{
    (void)e;

    if (g_ui.workflow == NULL)
    {
        return;
    }

    if (camera_workflow_remove_selected_item(g_ui.workflow) != 0)
    {
        checkout_ui_show_error("Selection Required", "Select an item first.");
    }

    g_ui.last_total_cent = 0xFFFFFFFFU;
    ui_checkout_refresh_all(g_ui.workflow);
}

static void checkout_ui_clear_event_cb(lv_event_t *e)
{
    (void)e;

    if (g_ui.workflow == NULL)
    {
        return;
    }

    if (g_ui.workflow->cart.item_count == 0U)
    {
        checkout_ui_show_error("Clear Cart", "Cart is already empty.");
        return;
    }

    camera_workflow_clear_cart(g_ui.workflow);
    camera_workflow_continue_shopping(g_ui.workflow);
    checkout_ui_show_error("Cart Cleared", "All items were removed.");
    g_ui.last_total_cent = 0xFFFFFFFFU;
    ui_checkout_refresh_all(g_ui.workflow);
}

static void checkout_ui_continue_event_cb(lv_event_t *e)
{
    (void)e;

    if (g_ui.workflow == NULL)
    {
        return;
    }

    camera_workflow_continue_shopping(g_ui.workflow);
    ui_dialog_close();
    g_ui.last_selected_index = -2;
    ui_checkout_refresh_all(g_ui.workflow);
}

static void ui_checkout_refresh_all(camera_workflow_state_t *state)
{
    if ((state == NULL) || (g_ui.workflow == NULL))
    {
        return;
    }

    checkout_ui_set_status(checkout_ui_status_text(state));
    checkout_ui_set_weight_info(state->weight_gram,
                                state->recognition_time_valid,
                                state->recognition_time_ms_x10);
    checkout_ui_update_preview();

    if (state->last_cart_add.valid != 0U)
    {
        checkout_ui_set_recognition_result(&state->last_cart_add);
    }
    else if (state->trigger_state == CAMERA_TRIGGER_WAIT_REMOVE)
    {
        lv_label_set_text(g_ui.result_name_label, "Item: Failed");
        lv_label_set_text(g_ui.result_price_label, "Price: --");
        lv_label_set_text(g_ui.result_amount_label, "Weight/Qty: --");
        lv_label_set_text(g_ui.result_subtotal_label, "Subtotal: Try again");
    }
    else
    {
        checkout_ui_clear_recognition_result();
    }

    checkout_ui_refresh_cart(&state->cart, state->cart.selected_item_index);
    checkout_ui_set_total_price(state->cart.total_price_cent);
    checkout_ui_show_notice(state);
}

void ui_checkout_main_init(camera_workflow_state_t *state)
{
    checkout_ui_init(state);
}

void ui_checkout_main_refresh(camera_workflow_state_t *state)
{
    ui_checkout_refresh_all(state);
}

void ui_checkout_refresh_status(camera_workflow_state_t *state)
{
    if (state == NULL)
    {
        return;
    }

    checkout_ui_set_status(checkout_ui_status_text(state));
    checkout_ui_set_weight_info(state->weight_gram,
                                state->recognition_time_valid,
                                state->recognition_time_ms_x10);
}

void ui_checkout_refresh_result(camera_workflow_state_t *state)
{
    if (state == NULL)
    {
        return;
    }

    checkout_ui_update_preview();
    if (state->last_cart_add.valid != 0U)
    {
        checkout_ui_set_recognition_result(&state->last_cart_add);
    }
    else
    {
        checkout_ui_clear_recognition_result();
    }
}

void ui_checkout_refresh_cart(camera_workflow_state_t *state)
{
    if (state == NULL)
    {
        return;
    }

    checkout_ui_refresh_cart(&state->cart, state->cart.selected_item_index);
    checkout_ui_set_total_price(state->cart.total_price_cent);
}

void ui_checkout_refresh_total(camera_workflow_state_t *state)
{
    if (state == NULL)
    {
        return;
    }

    checkout_ui_set_total_price(state->cart.total_price_cent);
}

void ui_checkout_show_recognize_fail(const char *message)
{
    checkout_ui_show_error("Recognition Failed", (message != NULL) ? message : "Remove the item and try again.");
}

void ui_checkout_show_checkout_done(uint32_t total_price_cent)
{
    checkout_ui_show_checkout_done(total_price_cent);
}
