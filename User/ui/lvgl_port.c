#include "ui/lvgl_port.h"

#include "lvgl.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/TOUCH/touch.h"

#define LVGL_PORT_BUFFER_LINES  (24U)
#define LVGL_PORT_DMA_THRESHOLD_PIXELS  (512U)
#define LVGL_PORT_DCACHE_LINE_SIZE      (32U)

static void lvgl_port_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static void lvgl_port_touch_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static void lvgl_port_lcd_dma_init(void);
static uint8_t lvgl_port_try_dma_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static void lvgl_port_dma_complete_cb(DMA_HandleTypeDef *hdma);
static void lvgl_port_dma_error_cb(DMA_HandleTypeDef *hdma);
static void lvgl_port_clean_dcache(const void *addr, uint32_t size);

static DMA_HandleTypeDef g_lvgl_lcd_dma;
static uint8_t g_lvgl_lcd_dma_ready = 0U;
static volatile uint8_t g_lvgl_lcd_dma_active = 0U;
static lv_disp_drv_t *g_lvgl_pending_disp_drv = NULL;

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

    lvgl_port_lcd_dma_init();

    tp_dev.init();

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvgl_port_touch_read_cb;
    lv_indev_drv_register(&indev_drv);
}

static void lvgl_port_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (lvgl_port_try_dma_flush(disp_drv, area, color_p) == 0U)
    {
        lcd_color_fill(area->x1,
                       area->y1,
                       area->x2,
                       area->y2,
                       (uint16_t *)color_p);
        lv_disp_flush_ready(disp_drv);
    }
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

static void lvgl_port_lcd_dma_init(void)
{
    __HAL_RCC_GPDMA1_CLK_ENABLE();

    g_lvgl_lcd_dma.Instance = GPDMA1_Channel0;
    g_lvgl_lcd_dma.Init.Request = 0U;
    g_lvgl_lcd_dma.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    g_lvgl_lcd_dma.Init.Direction = DMA_MEMORY_TO_MEMORY;
    g_lvgl_lcd_dma.Init.SrcInc = DMA_SINC_INCREMENTED;
    g_lvgl_lcd_dma.Init.DestInc = DMA_DINC_FIXED;
    g_lvgl_lcd_dma.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
    g_lvgl_lcd_dma.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
    g_lvgl_lcd_dma.Init.Priority = DMA_HIGH_PRIORITY;
    g_lvgl_lcd_dma.Init.SrcBurstLength = 16U;
    g_lvgl_lcd_dma.Init.DestBurstLength = 16U;
    g_lvgl_lcd_dma.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1 | DMA_DEST_ALLOCATED_PORT0;
    g_lvgl_lcd_dma.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    g_lvgl_lcd_dma.Init.Mode = DMA_NORMAL;

    if (HAL_DMA_Init(&g_lvgl_lcd_dma) == HAL_OK)
    {
        g_lvgl_lcd_dma.XferCpltCallback = lvgl_port_dma_complete_cb;
        g_lvgl_lcd_dma.XferErrorCallback = lvgl_port_dma_error_cb;
        HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 5U, 0U);
        HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
        g_lvgl_lcd_dma_ready = 1U;
    }
}

static uint8_t lvgl_port_try_dma_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t pixel_count;
    uint32_t byte_count;

    if ((g_lvgl_lcd_dma_ready == 0U) || (g_lvgl_lcd_dma_active != 0U))
    {
        return 0U;
    }

    pixel_count = (uint32_t)(area->x2 - area->x1 + 1) * (uint32_t)(area->y2 - area->y1 + 1);
    if (pixel_count < LVGL_PORT_DMA_THRESHOLD_PIXELS)
    {
        return 0U;
    }

    byte_count = pixel_count * sizeof(lv_color_t);
    lvgl_port_clean_dcache(color_p, byte_count);

    lcd_set_window((uint16_t)area->x1,
                   (uint16_t)area->y1,
                   (uint16_t)(area->x2 - area->x1 + 1),
                   (uint16_t)(area->y2 - area->y1 + 1));
    lcd_write_ram_prepare();

    g_lvgl_pending_disp_drv = disp_drv;
    g_lvgl_lcd_dma_active = 1U;

    if (HAL_DMA_Start_IT(&g_lvgl_lcd_dma,
                         (uint32_t)color_p,
                         (uint32_t)&LCD->LCD_RAM,
                         byte_count) != HAL_OK)
    {
        g_lvgl_pending_disp_drv = NULL;
        g_lvgl_lcd_dma_active = 0U;
        return 0U;
    }

    return 1U;
}

static void lvgl_port_dma_complete_cb(DMA_HandleTypeDef *hdma)
{
    lv_disp_drv_t *disp_drv = g_lvgl_pending_disp_drv;

    (void)hdma;

    g_lvgl_pending_disp_drv = NULL;
    g_lvgl_lcd_dma_active = 0U;

    if (disp_drv != NULL)
    {
        lv_disp_flush_ready(disp_drv);
    }
}

static void lvgl_port_dma_error_cb(DMA_HandleTypeDef *hdma)
{
    lv_disp_drv_t *disp_drv = g_lvgl_pending_disp_drv;

    (void)hdma;

    g_lvgl_pending_disp_drv = NULL;
    g_lvgl_lcd_dma_active = 0U;

    if (disp_drv != NULL)
    {
        lv_disp_flush_ready(disp_drv);
    }
}

static void lvgl_port_clean_dcache(const void *addr, uint32_t size)
{
    uint32_t aligned_addr;
    uint32_t aligned_size;

    if (size == 0U)
    {
        return;
    }

    aligned_addr = ((uint32_t)addr) & ~(LVGL_PORT_DCACHE_LINE_SIZE - 1U);
    aligned_size = (((uint32_t)addr + size + LVGL_PORT_DCACHE_LINE_SIZE - 1U) & ~(LVGL_PORT_DCACHE_LINE_SIZE - 1U)) - aligned_addr;
    SCB_CleanDCache_by_Addr((uint32_t *)aligned_addr, (int32_t)aligned_size);
}

void lvgl_port_gpdma_irqhandler(void)
{
    HAL_DMA_IRQHandler(&g_lvgl_lcd_dma);
}

