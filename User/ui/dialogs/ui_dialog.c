#include "ui/ui_dialog.h"

#include "lvgl.h"

LV_FONT_DECLARE(Font20);

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
    lv_disp_t *disp;
    lv_obj_t *panel;
    lv_obj_t *label;
    lv_obj_t *button;
    lv_obj_t *button_label;
    lv_coord_t screen_w = 480;
    lv_coord_t screen_h = 800;

    ui_dialog_close();

    disp = lv_disp_get_default();
    if (disp != NULL)
    {
        screen_w = lv_disp_get_hor_res(disp);
        screen_h = lv_disp_get_ver_res(disp);
    }

    g_dialog_overlay = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(g_dialog_overlay);
    lv_obj_set_size(g_dialog_overlay, screen_w, screen_h);
    lv_obj_set_style_bg_color(g_dialog_overlay, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(g_dialog_overlay, LV_OPA_40, 0);
    lv_obj_center(g_dialog_overlay);

    panel = lv_obj_create(g_dialog_overlay);
    lv_obj_set_size(panel, 360, 180);
    lv_obj_center(panel);
    lv_obj_set_style_radius(panel, 12, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, lv_color_hex(0xE0E0E0), 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_pad_all(panel, 16, 0);

    label = lv_label_create(panel);
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x222222), 0);
    lv_label_set_text(label, title);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 4);

    label = lv_label_create(panel);
    lv_obj_set_width(label, 300);
    lv_obj_set_style_text_font(label, &Font20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x666666), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label, message);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 48);

    button = lv_btn_create(panel);
    lv_obj_set_size(button, 132, 46);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -14);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x1E88E5), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x1565C0), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(button, 0, 0);
    lv_obj_set_style_radius(button, 10, 0);
    lv_obj_add_event_cb(button, ui_dialog_close_event_cb, LV_EVENT_CLICKED, NULL);

    button_label = lv_label_create(button);
    lv_obj_set_style_text_font(button_label, &Font20, 0);
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text(button_label, "确定");
    lv_obj_center(button_label);
}
