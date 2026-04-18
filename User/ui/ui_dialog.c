#include "ui/ui_dialog.h"

#include "lvgl.h"

LV_FONT_DECLARE(lv_font_montserrat_20);

static lv_obj_t *g_dialog_overlay = NULL;

static void ui_dialog_close_event_cb(lv_event_t *e)
{
    (void)e;
    ui_dialog_close();
}

void ui_dialog_close(void)
{
    if (g_dialog_overlay != NULL)
    {
        lv_obj_del(g_dialog_overlay);
        g_dialog_overlay = NULL;
    }
}

void ui_dialog_show_message(const char *title, const char *message)
{
    lv_obj_t *panel;
    lv_obj_t *label;
    lv_obj_t *button;
    lv_obj_t *button_label;

    ui_dialog_close();

    g_dialog_overlay = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(g_dialog_overlay);
    lv_obj_set_size(g_dialog_overlay, 480, 800);
    lv_obj_set_style_bg_color(g_dialog_overlay, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(g_dialog_overlay, LV_OPA_50, 0);
    lv_obj_center(g_dialog_overlay);

    panel = lv_obj_create(g_dialog_overlay);
    lv_obj_set_size(panel, 360, 220);
    lv_obj_center(panel);
    lv_obj_set_style_radius(panel, 16, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_pad_all(panel, 18, 0);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x16324A), 0);
    lv_label_set_text(label, title);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 4);

    label = lv_label_create(panel);
    lv_obj_set_width(label, 300);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x2F3B45), 0);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, message);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 52);

    button = lv_btn_create(panel);
    lv_obj_set_size(button, 132, 46);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -12);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x2A9D8F), 0);
    lv_obj_set_style_border_width(button, 0, 0);
    lv_obj_set_style_radius(button, 10, 0);
    lv_obj_add_event_cb(button, ui_dialog_close_event_cb, LV_EVENT_CLICKED, NULL);

    button_label = lv_label_create(button);
    lv_obj_set_style_text_font(button_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(button_label, "OK");
    lv_obj_center(button_label);
}
