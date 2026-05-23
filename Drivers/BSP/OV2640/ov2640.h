/**
 ****************************************************************************************************
 * @file        ov2640.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-05-21
 * @brief       OV2640 驱动代码
 * @license     Copyright (c) 2023-2035, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 H7R3开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#ifndef __OV2640_H
#define __OV2640_H

#include "./SYSTEM/sys/sys.h"

/* 引脚定义 */
#define OV_RESET_GPIO_PORT          GPIOE
#define OV_RESET_GPIO_PIN           GPIO_PIN_2
#define OV_RESET_GPIO_CLK_ENABLE()  do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0)
#define OV_PWDN_GPIO_PORT           GPIOF
#define OV_PWDN_GPIO_PIN            GPIO_PIN_5
#define OV_PWDN_GPIO_CLK_ENABLE()   do { __HAL_RCC_GPIOF_CLK_ENABLE(); } while(0)

/* IO操作 */
#define OV2640_PWDN(x)              do { (x) ?                                                                      \
                                        HAL_GPIO_WritePin(OV_PWDN_GPIO_PORT, OV_PWDN_GPIO_PIN, GPIO_PIN_SET):       \
                                        HAL_GPIO_WritePin(OV_PWDN_GPIO_PORT, OV_PWDN_GPIO_PIN, GPIO_PIN_RESET);     \
                                    } while (0)
#define OV2640_RST(x)               do { (x) ?                                                                      \
                                        HAL_GPIO_WritePin(OV_RESET_GPIO_PORT, OV_RESET_GPIO_PIN, GPIO_PIN_SET):     \
                                        HAL_GPIO_WritePin(OV_RESET_GPIO_PORT, OV_RESET_GPIO_PIN, GPIO_PIN_RESET);   \
                                    } while (0)

/* 为兼容现有工程，接口名仍沿用 OV2640，内部目标器件为 OV2640 */
#define OV2640_ID                   0x2642
#define OV2640_ADDR                 0x30

/* 函数声明 */
uint8_t ov2640_read_reg(uint16_t reg);                                                          /* 读OV2640寄存器 */
void ov2640_write_reg(uint16_t reg, uint8_t data);                                              /* 写OV2640寄存器 */
uint8_t ov2640_init(void);                                                                      /* 初始化OV2640 */
void ov2640_flash_ctrl(uint8_t sw);                                                             /* 控制OV2640闪光灯 */
void ov2640_rgb565_mode(void);                                                                  /* 配置OV2640 RGB565输出 */
void ov2640_jpeg_mode(void);                                                                    /* 配置OV2640 JPEG输出 */
void ov2640_light_mode(uint8_t mode);                                                           /* 设置OV2640灯光模式 */
void ov2640_exposure(uint8_t level);                                                            /* 设置OV2640曝光等级 */
void ov2640_brightness(uint8_t level);                                                          /* 设置OV2640亮度 */
void ov2640_color_saturation(uint8_t level);                                                    /* 设置OV2640色彩饱和度 */
void ov2640_contrast(uint8_t level);                                                            /* 设置OV2640对比度 */
void ov2640_sharpness(uint8_t level);                                                           /* 设置OV2640锐度 */
void ov2640_special_effects(uint8_t effects);                                                   /* 设置OV2640特效 */
void ov2640_test_pattern(uint8_t mode);                                                         /* OV2640测试 */
void ov2640_hmirror(uint8_t enable);                                                            /* OV2640 horizontal mirror */
uint8_t ov2640_outsize_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height);      /* 设置OV2640输出图像大小 */
uint8_t ov2640_image_window_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height); /* 设置OV2640图像开窗大小 */
uint8_t ov2640_focus_init(void);                                                                /* 初始化OV2640自动对焦 */
uint8_t ov2640_focus_single(void);                                                              /* OV2640单次自动对焦 */
uint8_t ov2640_focus_constant(void);                                                            /* OV2640连续自动对焦 */

#endif

