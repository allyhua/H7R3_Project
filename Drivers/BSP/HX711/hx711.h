#ifndef __HX711_H
#define __HX711_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"

#define HX711_DOUT_GPIO_PORT         GPIOG
#define HX711_DOUT_GPIO_PIN          GPIO_PIN_11
#define HX711_DOUT_GPIO_CLK_ENABLE() do { __HAL_RCC_GPIOG_CLK_ENABLE(); } while (0)

#define HX711_SCK_GPIO_PORT          GPIOG
#define HX711_SCK_GPIO_PIN           GPIO_PIN_12
#define HX711_SCK_GPIO_CLK_ENABLE()  do { __HAL_RCC_GPIOG_CLK_ENABLE(); } while (0)

#define HX711_DOUT_READ()            HAL_GPIO_ReadPin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN)
#define HX711_SCK_WRITE(x)           HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN, (x))

void hx711_init(void);
uint8_t hx711_is_ready(void);
uint32_t hx711_read(void);

#endif
