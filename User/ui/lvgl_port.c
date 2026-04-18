#include "ui/lvgl_port.h"

#include "lvgl.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/TOUCH/touch.h"

#define LVGL_PORT_BUFFER_LINES  (8U)

static void lvgl_port_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static void lvgl_port_touch_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

void lvgl_port_init(void)
{
    static lv_color_t draw_buf_1[800U * LVGL_PORT_BUFFER_LINES];
    static lv_disp_draw_buf_t draw_buf;
    static lv_disp_drv_t disp_drv;
    static lv_indev_drv_t indev_drv;

    lv_disp_draw_buf_init(&draw_buf,
                          draw_buf_1,
                          NULL,
                          lcddev.width * LVGL_PORT_BUFFER_LINES);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = lcddev.width;
    disp_drv.ver_res = lcddev.height;
    disp_drv.flush_cb = lvgl_port_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    tp_dev.init();

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvgl_port_touch_read_cb;
    lv_indev_drv_register(&indev_drv);
}

static void lvgl_port_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lcd_color_fill(area->x1,
                   area->y1,
                   area->x2,
                   area->y2,
                   (uint16_t *)color_p);
    lv_disp_flush_ready(disp_drv);
}

static void lvgl_port_touch_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    (void)indev_drv;

    tp_dev.scan(0);
    if ((tp_dev.sta & TP_PRES_DOWN) != 0U)
    {
        last_x = (lv_coord_t)tp_dev.x[0];
        last_y = (lv_coord_t)tp_dev.y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    data->point.x = last_x;
    data->point.y = last_y;
}

