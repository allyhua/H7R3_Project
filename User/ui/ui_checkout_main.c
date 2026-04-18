#include "ui/ui_checkout_main.h"

#include "ui/ui_dialog.h"
#include "lvgl.h"
#include "ai_class_labels.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

LV_FONT_DECLARE(lv_font_montserrat_20);
LV_FONT_DECLARE(lv_font_montserrat_32);

#define UI_SCREEN_WIDTH        480
#define UI_SCREEN_HEIGHT       800
#define UI_CARD_WIDTH          456
#define UI_PREVIEW_IMAGE_ZOOM  128

typedef struct
{
    camera_workflow_state_t *workflow;
    lv_obj_t *screen;
    lv_obj_t *status_label;
    lv_obj_t *detail_label;
    lv_obj_t *preview_image;
    lv_obj_t *preview_placeholder;
    lv_obj_t *preview_info;
    lv_obj_t *cart_body;
    lv_obj_t *total_label;
    lv_img_dsc_t preview_dsc;
    uint32_t last_capture_counter;
    uint32_t last_total_cent;
    uint32_t last_item_count;
    int32_t last_selected_index;
    char last_status[48];
} checkout_ui_t;

static checkout_ui_t g_ui;

static void ui_checkout_format_row(const cart_item_t *item,
                                   char *amount_text,
                                   uint32_t amount_size,
                                   char *unit_price_text,
                                   uint32_t unit_price_size,
                                   char *subtotal_text,
                                   uint32_t subtotal_size);
static void ui_checkout_rebuild_cart(checkout_ui_t *ui);
static void ui_checkout_update_status(checkout_ui_t *ui);
static void ui_checkout_update_preview(checkout_ui_t *ui);
static void ui_checkout_update_total(checkout_ui_t *ui);
static void ui_checkout_show_notice(checkout_ui_t *ui);
static void ui_checkout_row_event_cb(lv_event_t *e);
static void ui_checkout_delete_event_cb(lv_event_t *e);
static void ui_checkout_clear_event_cb(lv_event_t *e);
static void ui_checkout_continue_event_cb(lv_event_t *e);
static void ui_checkout_finish_event_cb(lv_event_t *e);

void ui_checkout_main_init(camera_workflow_state_t *state)
{
    lv_obj_t *panel;
    lv_obj_t *label;
    lv_obj_t *button;
    lv_obj_t *button_label;
    static const char *button_texts[4] = {"Checkout", "Delete", "Clear", "Continue"};
    lv_event_cb_t button_events[4] = {
        ui_checkout_finish_event_cb,
        ui_checkout_delete_event_cb,
        ui_checkout_clear_event_cb,
        ui_checkout_continue_event_cb
    };
    static const lv_coord_t button_x[4] = {208, 332, 208, 332};
    static const lv_coord_t button_y[4] = {22, 22, 78, 78};
    uint32_t idx;

    memset(&g_ui, 0, sizeof(g_ui));
    g_ui.workflow = state;
    g_ui.last_selected_index = -2;

    g_ui.preview_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    g_ui.preview_dsc.header.w = state->frame_width;
    g_ui.preview_dsc.header.h = state->frame_height;
    g_ui.preview_dsc.data_size = (uint32_t)state->frame_width * (uint32_t)state->frame_height * 2U;
    g_ui.preview_dsc.data = (const uint8_t *)camera_workflow_get_frame_buffer();

    g_ui.screen = lv_obj_create(NULL);
    lv_obj_clear_flag(g_ui.screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(g_ui.screen, lv_color_hex(0xEAF0F4), 0);
    lv_obj_set_style_bg_opa(g_ui.screen, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(g_ui.screen, 0, 0);
    lv_obj_set_style_radius(g_ui.screen, 0, 0);

    panel = lv_obj_create(g_ui.screen);
    lv_obj_set_pos(panel, 0, 0);
    lv_obj_set_size(panel, UI_SCREEN_WIDTH, 82);
    lv_obj_set_style_radius(panel, 0, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x14324A), 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    g_ui.status_label = lv_label_create(panel);
    lv_obj_set_width(g_ui.status_label, 448);
    lv_obj_set_style_text_font(g_ui.status_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(g_ui.status_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_pos(g_ui.status_label, 16, 12);

    g_ui.detail_label = lv_label_create(panel);
    lv_obj_set_width(g_ui.detail_label, 448);
    lv_obj_set_style_text_font(g_ui.detail_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(g_ui.detail_label, lv_color_hex(0xD4E3EE), 0);
    lv_obj_set_pos(g_ui.detail_label, 16, 46);

    panel = lv_obj_create(g_ui.screen);
    lv_obj_set_pos(panel, 12, 94);
    lv_obj_set_size(panel, UI_CARD_WIDTH, 288);
    lv_obj_set_style_radius(panel, 14, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x1F364D), 0);
    lv_label_set_text(label, "Preview");
    lv_obj_set_pos(label, 14, 10);

    label = lv_obj_create(panel);
    lv_obj_set_pos(label, 14, 42);
    lv_obj_set_size(label, 428, 190);
    lv_obj_set_style_radius(label, 10, 0);
    lv_obj_set_style_bg_color(label, lv_color_hex(0xF3F6F8), 0);
    lv_obj_set_style_border_width(label, 1, 0);
    lv_obj_set_style_border_color(label, lv_color_hex(0xD5DEE5), 0);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

    g_ui.preview_image = lv_img_create(label);
    lv_img_set_src(g_ui.preview_image, &g_ui.preview_dsc);
    lv_img_set_zoom(g_ui.preview_image, UI_PREVIEW_IMAGE_ZOOM);
    lv_obj_center(g_ui.preview_image);

    g_ui.preview_placeholder = lv_label_create(label);
    lv_obj_set_style_text_font(g_ui.preview_placeholder, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(g_ui.preview_placeholder, lv_color_hex(0x7C8B96), 0);
    lv_label_set_text(g_ui.preview_placeholder, "Waiting for snapshot");
    lv_obj_center(g_ui.preview_placeholder);

    g_ui.preview_info = lv_label_create(panel);
    lv_obj_set_width(g_ui.preview_info, 428);
    lv_obj_set_style_text_font(g_ui.preview_info, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(g_ui.preview_info, lv_color_hex(0x445462), 0);
    lv_label_set_long_mode(g_ui.preview_info, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(g_ui.preview_info, 14, 242);

    panel = lv_obj_create(g_ui.screen);
    lv_obj_set_pos(panel, 12, 394);
    lv_obj_set_size(panel, UI_CARD_WIDTH, 242);
    lv_obj_set_style_radius(panel, 14, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x1F364D), 0);
    lv_label_set_text(label, "Cart");
    lv_obj_set_pos(label, 14, 10);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x6A7A88), 0);
    lv_label_set_text(label, "Item");
    lv_obj_set_pos(label, 12, 42);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x6A7A88), 0);
    lv_label_set_text(label, "Qty/Wt");
    lv_obj_set_pos(label, 132, 42);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x6A7A88), 0);
    lv_label_set_text(label, "Price");
    lv_obj_set_pos(label, 258, 42);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x6A7A88), 0);
    lv_label_set_text(label, "Total");
    lv_obj_set_pos(label, 360, 42);

    g_ui.cart_body = lv_obj_create(panel);
    lv_obj_set_pos(g_ui.cart_body, 8, 70);
    lv_obj_set_size(g_ui.cart_body, 440, 160);
    lv_obj_set_style_radius(g_ui.cart_body, 10, 0);
    lv_obj_set_style_bg_color(g_ui.cart_body, lv_color_hex(0xF5F8FA), 0);
    lv_obj_set_style_border_width(g_ui.cart_body, 0, 0);
    lv_obj_set_style_pad_all(g_ui.cart_body, 6, 0);
    lv_obj_set_scroll_dir(g_ui.cart_body, LV_DIR_VER);
    lv_obj_set_layout(g_ui.cart_body, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(g_ui.cart_body, LV_FLEX_FLOW_COLUMN);

    panel = lv_obj_create(g_ui.screen);
    lv_obj_set_pos(panel, 12, 648);
    lv_obj_set_size(panel, UI_CARD_WIDTH, 140);
    lv_obj_set_style_radius(panel, 14, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x6A7A88), 0);
    lv_label_set_text(label, "Total");
    lv_obj_set_pos(label, 18, 18);

    g_ui.total_label = lv_label_create(panel);
    lv_obj_set_style_text_font(g_ui.total_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(g_ui.total_label, lv_color_hex(0xE76F51), 0);
    lv_obj_set_pos(g_ui.total_label, 18, 46);

    for (idx = 0U; idx < 4U; ++idx)
    {
        button = lv_btn_create(panel);
        lv_obj_set_size(button, 112, 44);
        lv_obj_set_pos(button, button_x[idx], button_y[idx]);
        lv_obj_set_style_radius(button, 10, 0);
        lv_obj_set_style_border_width(button, 0, 0);
        lv_obj_set_style_bg_color(button,
                                  (idx == 0U) ? lv_color_hex(0x2A9D8F) : lv_color_hex(0x457B9D),
                                  0);
        lv_obj_add_event_cb(button, button_events[idx], LV_EVENT_CLICKED, &g_ui);

        button_label = lv_label_create(button);
    lv_obj_set_style_text_font(button_label, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
        lv_label_set_text(button_label, button_texts[idx]);
        lv_obj_center(button_label);
    }

    lv_scr_load(g_ui.screen);
    ui_checkout_main_refresh(state);
}

void ui_checkout_main_refresh(camera_workflow_state_t *state)
{
    if ((state == NULL) || (g_ui.workflow == NULL))
    {
        return;
    }

    ui_checkout_update_status(&g_ui);
    ui_checkout_update_preview(&g_ui);
    ui_checkout_update_total(&g_ui);

    if ((g_ui.last_total_cent != state->cart.total_price_cent) ||
        (g_ui.last_item_count != state->cart.item_count) ||
        (g_ui.last_selected_index != state->cart.selected_item_index))
    {
        ui_checkout_rebuild_cart(&g_ui);
    }

    ui_checkout_show_notice(&g_ui);
}

static void ui_checkout_format_row(const cart_item_t *item,
                                   char *amount_text,
                                   uint32_t amount_size,
                                   char *unit_price_text,
                                   uint32_t unit_price_size,
                                   char *subtotal_text,
                                   uint32_t subtotal_size)
{
    if (item->pricing_mode == PRODUCT_PRICING_BY_WEIGHT)
    {
        snprintf(amount_text,
                 amount_size,
                 "%lu.%03lu kg",
                 (unsigned long)(item->total_weight_g / 1000U),
                 (unsigned long)(item->total_weight_g % 1000U));
        snprintf(unit_price_text,
                 unit_price_size,
                 "%lu.%02lu/kg",
                 (unsigned long)(item->unit_price_cent / 100U),
                 (unsigned long)(item->unit_price_cent % 100U));
    }
    else
    {
        snprintf(amount_text, amount_size, "%lu pcs", (unsigned long)item->total_count);
        snprintf(unit_price_text,
                 unit_price_size,
                 "%lu.%02lu/pc",
                 (unsigned long)(item->unit_price_cent / 100U),
                 (unsigned long)(item->unit_price_cent % 100U));
    }

    snprintf(subtotal_text,
             subtotal_size,
             "%lu.%02lu",
             (unsigned long)(item->subtotal_cent / 100U),
             (unsigned long)(item->subtotal_cent % 100U));
}

static void ui_checkout_rebuild_cart(checkout_ui_t *ui)
{
    uint32_t idx;

    lv_obj_clean(ui->cart_body);

    for (idx = 0U; idx < PRODUCT_ID_COUNT; ++idx)
    {
        const cart_item_t *item = &ui->workflow->cart.items[idx];
        lv_obj_t *row;
        lv_obj_t *label;
        char amount_text[24];
        char unit_price_text[24];
        char subtotal_text[24];

        if (item->line_valid == 0U)
        {
            continue;
        }

        ui_checkout_format_row(item,
                               amount_text,
                               sizeof(amount_text),
                               unit_price_text,
                               sizeof(unit_price_text),
                               subtotal_text,
                               sizeof(subtotal_text));

        row = lv_btn_create(ui->cart_body);
        lv_obj_set_size(row, 428, 40);
        lv_obj_set_style_radius(row, 8, 0);
        lv_obj_set_style_border_width(row, 0, 0);
        lv_obj_set_style_bg_color(row,
                                  ((int32_t)idx == ui->workflow->cart.selected_item_index) ?
                                      lv_color_hex(0xD7EEF8) : lv_color_hex(0xFFFFFF),
                                  0);
        lv_obj_add_event_cb(row, ui_checkout_row_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)idx);

        label = lv_label_create(row);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0x23313F), 0);
        lv_label_set_text(label, item->product_name);
        lv_obj_set_pos(label, 8, 10);

        label = lv_label_create(row);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0x4C5A67), 0);
        lv_label_set_text(label, amount_text);
        lv_obj_set_pos(label, 124, 10);

        label = lv_label_create(row);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0x4C5A67), 0);
        lv_label_set_text(label, unit_price_text);
        lv_obj_set_pos(label, 246, 10);

        label = lv_label_create(row);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(label, lv_color_hex(0xE76F51), 0);
        lv_label_set_text(label, subtotal_text);
        lv_obj_set_pos(label, 356, 10);
    }

    if (ui->workflow->cart.item_count == 0U)
    {
        lv_obj_t *empty_label = lv_label_create(ui->cart_body);
        lv_obj_set_style_text_font(empty_label, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(empty_label, lv_color_hex(0x7A8A99), 0);
        lv_label_set_text(empty_label, "Cart is empty");
        lv_obj_center(empty_label);
    }

    ui->last_total_cent = ui->workflow->cart.total_price_cent;
    ui->last_item_count = ui->workflow->cart.item_count;
    ui->last_selected_index = ui->workflow->cart.selected_item_index;
}

static void ui_checkout_update_status(checkout_ui_t *ui)
{
    char detail_text[128];

    if (strcmp(ui->last_status, ui->workflow->status_text) != 0)
    {
        lv_label_set_text(ui->status_label, ui->workflow->status_text);
        snprintf(ui->last_status, sizeof(ui->last_status), "%s", ui->workflow->status_text);
    }

    snprintf(detail_text,
             sizeof(detail_text),
             "Weight %lu g   Stable->Show %lu.%lu ms   Total %lu.%lu ms",
             (unsigned long)ui->workflow->weight_gram,
             (unsigned long)(ui->workflow->recognition_time_ms_x10 / 10U),
             (unsigned long)(ui->workflow->recognition_time_ms_x10 % 10U),
             (unsigned long)(ui->workflow->total_flow_time_ms_x10 / 10U),
             (unsigned long)(ui->workflow->total_flow_time_ms_x10 % 10U));
    lv_label_set_text(ui->detail_label, detail_text);
}

static void ui_checkout_update_preview(checkout_ui_t *ui)
{
    char preview_text[128];
    const char *label_name = "Waiting";

    if (ui->workflow->last_cart_add.valid != 0U)
    {
        label_name = ui->workflow->last_cart_add.product_name;
    }
    else if (ui->workflow->last_result_valid != 0U)
    {
        label_name = ai_class_label_get(ui->workflow->last_result.top1_index);
    }

    snprintf(preview_text,
             sizeof(preview_text),
             "Result: %s   Weight: %lu g",
             label_name,
             (unsigned long)ui->workflow->stable_weight_gram);
    lv_label_set_text(ui->preview_info, preview_text);

    if (ui->workflow->capture_counter != 0U)
    {
        lv_obj_add_flag(ui->preview_placeholder, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_clear_flag(ui->preview_placeholder, LV_OBJ_FLAG_HIDDEN);
    }

    if (ui->last_capture_counter != ui->workflow->capture_counter)
    {
        lv_img_set_src(ui->preview_image, &ui->preview_dsc);
        lv_obj_invalidate(ui->preview_image);
        ui->last_capture_counter = ui->workflow->capture_counter;
    }
}

static void ui_checkout_update_total(checkout_ui_t *ui)
{
    char total_text[32];

    snprintf(total_text,
             sizeof(total_text),
             "%lu.%02lu",
             (unsigned long)(ui->workflow->cart.total_price_cent / 100U),
             (unsigned long)(ui->workflow->cart.total_price_cent % 100U));
    lv_label_set_text(ui->total_label, total_text);
}

static void ui_checkout_show_notice(checkout_ui_t *ui)
{
    camera_notice_type_t notice_type;
    char notice_text[64];

    if (camera_workflow_pop_notice(ui->workflow, &notice_type, notice_text, sizeof(notice_text)) == 0)
    {
        return;
    }

    if (notice_type == CAMERA_NOTICE_RECOGNIZE_FAIL)
    {
        ui_dialog_show_message("Recognition", notice_text);
    }
    else
    {
        ui_dialog_show_message("Notice", notice_text);
    }
}

static void ui_checkout_row_event_cb(lv_event_t *e)
{
    checkout_ui_t *ui = &g_ui;
    int32_t selected_index = (int32_t)(uintptr_t)lv_event_get_user_data(e);

    camera_workflow_select_cart_item(ui->workflow, selected_index);
    ui_checkout_rebuild_cart(ui);
}

static void ui_checkout_delete_event_cb(lv_event_t *e)
{
    checkout_ui_t *ui = (checkout_ui_t *)lv_event_get_user_data(e);

    if (camera_workflow_remove_selected_item(ui->workflow) != 0)
    {
        ui_dialog_show_message("Delete", "Select one cart item first");
    }
    ui_checkout_main_refresh(ui->workflow);
}

static void ui_checkout_clear_event_cb(lv_event_t *e)
{
    checkout_ui_t *ui = (checkout_ui_t *)lv_event_get_user_data(e);

    if (ui->workflow->cart.item_count == 0U)
    {
        ui_dialog_show_message("Clear", "Cart is already empty");
        return;
    }

    camera_workflow_clear_cart(ui->workflow);
    camera_workflow_select_cart_item(ui->workflow, -1);
    ui_dialog_show_message("Clear", "Cart cleared");
    ui_checkout_main_refresh(ui->workflow);
}

static void ui_checkout_continue_event_cb(lv_event_t *e)
{
    checkout_ui_t *ui = (checkout_ui_t *)lv_event_get_user_data(e);

    camera_workflow_select_cart_item(ui->workflow, -1);
    ui_dialog_close();
    ui_checkout_main_refresh(ui->workflow);
}

static void ui_checkout_finish_event_cb(lv_event_t *e)
{
    checkout_ui_t *ui = (checkout_ui_t *)lv_event_get_user_data(e);
    char message[64];

    if (ui->workflow->cart.item_count == 0U)
    {
        ui_dialog_show_message("Checkout", "Cart is empty");
        return;
    }

    snprintf(message,
             sizeof(message),
             "Paid %lu.%02lu",
             (unsigned long)(ui->workflow->cart.total_price_cent / 100U),
             (unsigned long)(ui->workflow->cart.total_price_cent % 100U));
    camera_workflow_clear_cart(ui->workflow);
    camera_workflow_select_cart_item(ui->workflow, -1);
    ui_dialog_show_message("Checkout Done", message);
    ui_checkout_main_refresh(ui->workflow);
}
