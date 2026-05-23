/**
 ****************************************************************************************************
 * @file        ov2640.c
 * @brief       Legacy camera API wrapper for the new MC2640 camera module
 ****************************************************************************************************
 */

#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/ov2640cfg.h"
#include "./BSP/OV2640/sccb.h"
#include "./SYSTEM/delay/delay.h"
#include <stdio.h>

#define MC2640_MID  0x7FA2U
#define MC2640_PID  0x2642U
#define MC2640_ID_CONFIRM_DELAY_MS  1U

static uint16_t g_mc2640_width = 0U;
static uint16_t g_mc2640_height = 0U;

static void mc2640_select_bank(uint8_t bank)
{
    ov2640_write_reg(ATK_MC2640_REG_BANK_SEL, bank);
}

static void mc2640_write_table(const uint8_t (*table)[2], uint32_t count)
{
    uint32_t i;

    for (i = 0; i < count; ++i)
    {
        ov2640_write_reg(table[i][0], table[i][1]);
    }
}

static void mc2640_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    OV_RESET_GPIO_CLK_ENABLE();
    OV_PWDN_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin = OV_RESET_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(OV_RESET_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = OV_PWDN_GPIO_PIN;
    HAL_GPIO_Init(OV_PWDN_GPIO_PORT, &gpio_init_struct);

    OV2640_RST(1);
    OV2640_PWDN(1);
}

static void mc2640_exit_power_down(void)
{
    OV2640_PWDN(0);
    delay_ms(10);
}

static void mc2640_hw_reset(void)
{
    OV2640_RST(0);
    delay_ms(10);
    OV2640_RST(1);
    delay_ms(10);
}

static void mc2640_sw_reset(void)
{
    mc2640_select_bank(1U);
    ov2640_write_reg(ATK_MC2640_REG_SENSOR_COM7, 0x80);
    delay_ms(50);
}

static uint16_t mc2640_get_mid(void)
{
    uint16_t mid;

    mc2640_select_bank(1U);
    mid = ((uint16_t)ov2640_read_reg(ATK_MC2640_REG_SENSOR_MIDH) << 8);
    mid |= ov2640_read_reg(ATK_MC2640_REG_SENSOR_MIDL);

    return mid;
}

static uint16_t mc2640_get_pid(void)
{
    uint16_t pid;

    mc2640_select_bank(1U);
    pid = ((uint16_t)ov2640_read_reg(ATK_MC2640_REG_SENSOR_PIDH) << 8);
    pid |= ov2640_read_reg(ATK_MC2640_REG_SENSOR_PIDL);

    return pid;
}

static void mc2640_init_regs(void)
{
    uint8_t zmow;
    uint8_t zmoh;
    uint8_t zmhh;

    mc2640_write_table(atk_mc2640_init_uxga_cfg,
                       sizeof(atk_mc2640_init_uxga_cfg) / sizeof(atk_mc2640_init_uxga_cfg[0]));

    mc2640_select_bank(0U);
    zmow = ov2640_read_reg(ATK_MC2640_REG_DSP_ZMOW);
    zmoh = ov2640_read_reg(ATK_MC2640_REG_DSP_ZMOH);
    zmhh = ov2640_read_reg(ATK_MC2640_REG_DSP_ZMHH);

    g_mc2640_width = ((uint16_t)zmow | ((uint16_t)(zmhh & 0x03U) << 8)) << 2;
    g_mc2640_height = ((uint16_t)zmoh | ((uint16_t)(zmhh & 0x04U) << 6)) << 2;
}

static uint8_t mc2640_set_output_size(uint16_t width, uint16_t height)
{
    uint16_t output_width;
    uint16_t output_height;

    if (((width & 0x03U) != 0U) || ((height & 0x03U) != 0U))
    {
        return 1U;
    }

    g_mc2640_width = width;
    g_mc2640_height = height;
    output_width = width >> 2;
    output_height = height >> 2;

    mc2640_select_bank(0U);
    ov2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x04);
    ov2640_write_reg(ATK_MC2640_REG_DSP_ZMOW, (uint8_t)(output_width & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_ZMOH, (uint8_t)(output_height & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_ZMHH,
                     (uint8_t)(((output_width >> 8) & 0x03U) | ((output_height >> 6) & 0x04U)));
    ov2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x00);

    return 0U;
}

static uint8_t mc2640_set_image_window(uint16_t off_x, uint16_t off_y, uint16_t width, uint16_t height)
{
    uint16_t hsize;
    uint16_t vsize;
    uint8_t vhyx;

    if (((width & 0x03U) != 0U) || ((height & 0x03U) != 0U))
    {
        return 1U;
    }

    hsize = width >> 2;
    vsize = height >> 2;
    vhyx = (uint8_t)(((vsize >> 1) & 0x80U) | ((off_y >> 4) & 0x70U) |
                     ((hsize >> 5) & 0x08U) | ((off_x >> 8) & 0x07U));

    mc2640_select_bank(0U);
    ov2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x04);
    ov2640_write_reg(ATK_MC2640_REG_DSP_HSIZE, (uint8_t)(hsize & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_VSIZE, (uint8_t)(vsize & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_XOFFL, (uint8_t)(off_x & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_YOFFL, (uint8_t)(off_y & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_VHYX, vhyx);
    ov2640_write_reg(ATK_MC2640_REG_DSP_TEST, (uint8_t)((hsize >> 2) & 0x80U));
    ov2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x00);

    return 0U;
}

static void mc2640_set_image_size(uint16_t width, uint16_t height)
{
    uint8_t sizel;

    sizel = (uint8_t)(((width & 0x07U) << 3) | (height & 0x07U) | ((width >> 4) & 0x80U));

    mc2640_select_bank(0U);
    ov2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x04);
    ov2640_write_reg(ATK_MC2640_REG_DSP_HSIZE8, (uint8_t)((width >> 3) & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_VSIZE8, (uint8_t)((height >> 3) & 0xFFU));
    ov2640_write_reg(ATK_MC2640_REG_DSP_SIZEL, sizel);
    ov2640_write_reg(ATK_MC2640_REG_DSP_RESET, 0x00);
}

uint8_t ov2640_read_reg(uint16_t reg)
{
    uint8_t data;

    sccb_start();
    sccb_send_byte((OV2640_ADDR << 1) | 0x00U);
    sccb_send_byte((uint8_t)reg);
    sccb_stop();

    sccb_start();
    sccb_send_byte((OV2640_ADDR << 1) | 0x01U);
    data = sccb_read_byte();
    sccb_stop();

    return data;
}

void ov2640_write_reg(uint16_t reg, uint8_t data)
{
    sccb_start();
    sccb_send_byte((OV2640_ADDR << 1) | 0x00U);
    sccb_send_byte((uint8_t)reg);
    sccb_send_byte(data);
    sccb_stop();
}

uint8_t ov2640_init(void)
{
    uint16_t mid;
    uint16_t pid;
    uint16_t confirm_value;

    mc2640_hw_init();
    mc2640_exit_power_down();
    mc2640_hw_reset();
    sccb_init();
    mc2640_sw_reset();

    mid = mc2640_get_mid();
    if (mid != MC2640_MID)
    {
        delay_ms(MC2640_ID_CONFIRM_DELAY_MS);
        confirm_value = mc2640_get_mid();

        if (confirm_value != MC2640_MID)
        {
            printf("[CAM] MC2640 MID mismatch: 0x%04X -> 0x%04X\r\n", mid, confirm_value);
            return 1U;
        }
    }

    pid = mc2640_get_pid();
    if (pid != MC2640_PID)
    {
        delay_ms(MC2640_ID_CONFIRM_DELAY_MS);
        confirm_value = mc2640_get_pid();

        if (confirm_value != MC2640_PID)
        {
            printf("[CAM] MC2640 PID mismatch: 0x%04X -> 0x%04X\r\n", pid, confirm_value);
            return 1U;
        }
    }

    mc2640_init_regs();
    return 0U;
}

void ov2640_flash_ctrl(uint8_t sw)
{
    if (sw != 0U)
    {
        mc2640_select_bank(1U);
        ov2640_write_reg(ATK_MC2640_REG_SENSOR_COM22, 0x81);
        ov2640_write_reg(ATK_MC2640_REG_SENSOR_COM22, 0x01);
    }
}

void ov2640_rgb565_mode(void)
{
    mc2640_write_table(atk_mc2640_set_rgb565_cfg,
                       sizeof(atk_mc2640_set_rgb565_cfg) / sizeof(atk_mc2640_set_rgb565_cfg[0]));
}

void ov2640_jpeg_mode(void)
{
    mc2640_write_table(atk_mc2640_set_yuv422_cfg,
                       sizeof(atk_mc2640_set_yuv422_cfg) / sizeof(atk_mc2640_set_yuv422_cfg[0]));
    mc2640_write_table(atk_mc2640_set_jpeg_cfg,
                       sizeof(atk_mc2640_set_jpeg_cfg) / sizeof(atk_mc2640_set_jpeg_cfg[0]));
}

void ov2640_light_mode(uint8_t mode)
{
    mc2640_select_bank(0U);

    switch (mode)
    {
        case 0U:
            ov2640_write_reg(0xC7, 0x00);
            break;
        case 1U:
            ov2640_write_reg(0xC7, 0x40);
            ov2640_write_reg(0xCC, 0x5E);
            ov2640_write_reg(0xCD, 0x41);
            ov2640_write_reg(0xCE, 0x54);
            break;
        case 2U:
            ov2640_write_reg(0xC7, 0x40);
            ov2640_write_reg(0xCC, 0x65);
            ov2640_write_reg(0xCD, 0x41);
            ov2640_write_reg(0xCE, 0x4F);
            break;
        case 3U:
            ov2640_write_reg(0xC7, 0x40);
            ov2640_write_reg(0xCC, 0x52);
            ov2640_write_reg(0xCD, 0x41);
            ov2640_write_reg(0xCE, 0x66);
            break;
        default:
            ov2640_write_reg(0xC7, 0x40);
            ov2640_write_reg(0xCC, 0x42);
            ov2640_write_reg(0xCD, 0x3F);
            ov2640_write_reg(0xCE, 0x71);
            break;
    }
}

void ov2640_exposure(uint8_t level)
{
    (void)level;
}

void ov2640_brightness(uint8_t level)
{
    uint8_t value;

    if (level <= 1U) value = 0x40U;
    else if (level <= 3U) value = 0x30U;
    else if (level <= 5U) value = 0x20U;
    else if (level <= 7U) value = 0x10U;
    else value = 0x00U;

    mc2640_select_bank(0U);
    ov2640_write_reg(0x7C, 0x00);
    ov2640_write_reg(0x7D, 0x04);
    ov2640_write_reg(0x7C, 0x09);
    ov2640_write_reg(0x7D, value);
    ov2640_write_reg(0x7D, 0x00);
}

void ov2640_color_saturation(uint8_t level)
{
    uint8_t value;

    if (level <= 1U) value = 0x68U;
    else if (level == 2U) value = 0x58U;
    else if (level == 3U) value = 0x48U;
    else if (level == 4U) value = 0x38U;
    else value = 0x28U;

    mc2640_select_bank(0U);
    ov2640_write_reg(0x7C, 0x00);
    ov2640_write_reg(0x7D, 0x02);
    ov2640_write_reg(0x7C, 0x03);
    ov2640_write_reg(0x7D, value);
    ov2640_write_reg(0x7D, value);
}

void ov2640_contrast(uint8_t level)
{
    uint8_t p1;
    uint8_t p2;

    if (level <= 1U) { p1 = 0x28U; p2 = 0x0CU; }
    else if (level == 2U) { p1 = 0x24U; p2 = 0x16U; }
    else if (level == 3U) { p1 = 0x20U; p2 = 0x20U; }
    else if (level == 4U) { p1 = 0x1CU; p2 = 0x2AU; }
    else { p1 = 0x18U; p2 = 0x34U; }

    mc2640_select_bank(0U);
    ov2640_write_reg(0x7C, 0x00);
    ov2640_write_reg(0x7D, 0x04);
    ov2640_write_reg(0x7C, 0x07);
    ov2640_write_reg(0x7D, 0x20);
    ov2640_write_reg(0x7D, p1);
    ov2640_write_reg(0x7D, p2);
    ov2640_write_reg(0x7D, 0x06);
}

void ov2640_sharpness(uint8_t level)
{
    (void)level;
}

void ov2640_special_effects(uint8_t effects)
{
    mc2640_select_bank(0U);

    switch (effects)
    {
        case 0U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x18); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x40); ov2640_write_reg(0x7D, 0xA6);
            break;
        case 1U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x18); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0xA0); ov2640_write_reg(0x7D, 0x40);
            break;
        case 2U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x18); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x40); ov2640_write_reg(0x7D, 0x40);
            break;
        case 3U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x18); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x40); ov2640_write_reg(0x7D, 0xC0);
            break;
        case 4U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x18); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x80); ov2640_write_reg(0x7D, 0x80);
            break;
        case 5U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x40); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x80); ov2640_write_reg(0x7D, 0x80);
            break;
        case 6U:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x58); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x80); ov2640_write_reg(0x7D, 0x80);
            break;
        default:
            ov2640_write_reg(0x7C, 0x00); ov2640_write_reg(0x7D, 0x00); ov2640_write_reg(0x7C, 0x05); ov2640_write_reg(0x7D, 0x80); ov2640_write_reg(0x7D, 0x80);
            break;
    }
}

void ov2640_test_pattern(uint8_t mode)
{
    uint8_t com7;

    mc2640_select_bank(1U);
    com7 = ov2640_read_reg(ATK_MC2640_REG_SENSOR_COM7);

    if (mode != 0U)
    {
        com7 |= (1U << 1);
    }
    else
    {
        com7 &= (uint8_t)~(1U << 1);
    }

    ov2640_write_reg(ATK_MC2640_REG_SENSOR_COM7, com7);
}

void ov2640_hmirror(uint8_t enable)
{
    uint8_t reg04;

    mc2640_select_bank(1U);
    reg04 = ov2640_read_reg(ATK_MC2640_REG_SENSOR_REG04);
    if (enable != 0U)
    {
        reg04 |= 0x80U;
    }
    else
    {
        reg04 &= (uint8_t)~0x80U;
    }
    ov2640_write_reg(ATK_MC2640_REG_SENSOR_REG04, reg04);
    printf("[CAM] OV2640 horizontal mirror %s, REG04=0x%02X\r\n",
           (enable != 0U) ? "on" : "off",
           reg04);
}

uint8_t ov2640_outsize_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    (void)offx;
    (void)offy;
    return mc2640_set_output_size(width, height);
}

uint8_t ov2640_image_window_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    if (mc2640_set_image_window(offx, offy, width, height) != 0U)
    {
        return 1U;
    }

    mc2640_set_image_size(width, height);
    return 0U;
}

uint8_t ov2640_focus_init(void)
{
    return 0U;
}

uint8_t ov2640_focus_single(void)
{
    return 0U;
}

uint8_t ov2640_focus_constant(void)
{
    return 0U;
}
