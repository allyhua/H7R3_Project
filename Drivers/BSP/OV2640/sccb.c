/**
 ****************************************************************************************************
 * @file        sccb.c
 * @author      姝ｇ偣鍘熷瓙鍥㈤槦(ALIENTEK)
 * @version     V1.0
 * @date        2024-05-21
 * @brief       SCCB 椹卞姩浠ｇ爜
 * @license     Copyright (c) 2023-2035, 骞垮窞甯傛槦缈肩數瀛愮鎶€鏈夐檺鍏徃
 ****************************************************************************************************
 * @attention
 *
 * 瀹為獙骞冲彴:姝ｇ偣鍘熷瓙 H7R3寮€鍙戞澘
 * 鍦ㄧ嚎瑙嗛:www.yuanzige.com
 * 鎶€鏈鍧?www.openedv.com
 * 鍏徃缃戝潃:www.alientek.com
 * 璐拱鍦板潃:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#include "./BSP/OV2640/sccb.h"
#include "./SYSTEM/delay/delay.h"

static void sccb_delay(void)
{
    delay_us(5);
}

static void sccb_set_sda_output(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    gpio_init_struct.Pin = SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCCB_SDA_GPIO_PORT, &gpio_init_struct);
}

static void sccb_set_sda_input(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    gpio_init_struct.Pin = SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCCB_SDA_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief   鍒濆鍖朣CCB
 * @param   鏃?
 * @retval  鏃?
 */
void sccb_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* 鏃堕挓浣胯兘 */
    SCCB_SCL_GPIO_CLK_ENABLE();
    SCCB_SDA_GPIO_CLK_ENABLE();
    HAL_PWREx_EnableUSBVoltageDetector();   /* 浣跨敤GPIOM闇€瑕佸紑鍚鐢垫簮 */
    
    /* 閰嶇疆SCL寮曡剼 */
    gpio_init_struct.Pin = SCCB_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCCB_SCL_GPIO_PORT, &gpio_init_struct);
    
    /* 閰嶇疆SDA寮曡剼 */
    gpio_init_struct.Pin = SCCB_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCCB_SDA_GPIO_PORT, &gpio_init_struct);
    
    sccb_stop();
}

/**
 * @brief   浜х敓SCCB璧峰淇″彿
 * @param   鏃?
 * @retval  鏃?
 */
void sccb_start(void)
{
    SCCB_SDA(1);
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SDA(0);
    sccb_delay();
    SCCB_SCL(0);
}

/**
 * @brief   浜х敓SCCB鍋滄淇″彿
 * @param   鏃?
 * @retval  鏃?
 */
void sccb_stop(void)
{
    SCCB_SDA(0);
    sccb_delay();
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SDA(1);
    sccb_delay();
}

/**
 * @brief   浜х敓SCCB NACK淇″彿
 * @param   鏃?
 * @retval  鏃?
 */
void sccb_nack(void)
{
    sccb_delay();
    SCCB_SDA(1);
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SCL(0);
    sccb_delay();
    SCCB_SDA(0);
    sccb_delay();
}

/**
 * @brief   SCCB鍙戦€佷竴涓瓧鑺?
 * @param   data: 寰呭彂閫佺殑涓€瀛楄妭鏁版嵁
 * @retval  鍙戦€佺粨鏋?
 * @arg     0: 鍙戦€佹垚鍔?
 * @arg     1: 鍙戦€佸け璐?
 */
uint8_t sccb_send_byte(uint8_t data)
{
    uint8_t i;

    for (i=0; i<8; i++)
    {
        SCCB_SDA((data & 0x80) >> 7);
        sccb_delay();
        SCCB_SCL(1);
        sccb_delay();
        SCCB_SCL(0);
        data <<= 1;
    }
    SCCB_SDA(1);
    sccb_delay();
    
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SCL(0);

    return 0;
}

/**
 * @brief   SCCB璇诲彇涓€涓瓧鑺?
 * @param   鏃?
 * @retval  璇诲彇鍒扮殑涓€瀛楄妭鏁版嵁
 */
uint8_t sccb_read_byte(void)
{
    uint8_t i;
    uint8_t receive = 0;
    
    sccb_set_sda_input();

    for (i=0; i<8; i++)
    {
        receive <<= 1;
        SCCB_SCL(1);
        sccb_delay();
        
        if (SCCB_SDA_READ)
        {
            receive++;
        }
        
        SCCB_SCL(0);
        sccb_delay();
    }
    sccb_delay();
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SCL(0);
    sccb_delay();
    SCCB_SDA(0);
    sccb_delay();
    sccb_set_sda_output();

    return receive;
}
