#include "./BSP/HX711/hx711.h"

void hx711_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    HX711_DOUT_GPIO_CLK_ENABLE();
    HX711_SCK_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin = HX711_SCK_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_SCK_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = HX711_DOUT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HX711_DOUT_GPIO_PORT, &gpio_init_struct);

    HX711_SCK_WRITE(GPIO_PIN_RESET);
}

uint8_t hx711_is_ready(void)
{
    return (HX711_DOUT_READ() == GPIO_PIN_RESET) ? 1U : 0U;
}

uint32_t hx711_read(void)
{
    uint8_t i;
    uint32_t value = 0U;
    uint32_t timeout = 0x00FFFFFFU;

    HX711_SCK_WRITE(GPIO_PIN_RESET);

    while (HX711_DOUT_READ() == GPIO_PIN_SET)
    {
        if (timeout-- == 0U)
        {
            return 0U;
        }
    }

    for (i = 0U; i < 24U; ++i)
    {
        HX711_SCK_WRITE(GPIO_PIN_SET);
        value <<= 1;
        HX711_SCK_WRITE(GPIO_PIN_RESET);

        if (HX711_DOUT_READ() == GPIO_PIN_SET)
        {
            value |= 0x01U;
        }

        if ((i & 0x03) == 0x03)
        {
            delay_us(1);
        }
    }

    HX711_SCK_WRITE(GPIO_PIN_SET);
    value ^= 0x800000U;
    delay_us(1);
    HX711_SCK_WRITE(GPIO_PIN_RESET);

    return value;
}




