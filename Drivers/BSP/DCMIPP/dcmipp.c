/**
 ****************************************************************************************************
 * @file        dcmipp.c
 * @brief       DCMIPP driver
 ****************************************************************************************************
 */

#include "./BSP/DCMIPP/dcmipp.h"

extern volatile uint8_t g_ov_frame;

DCMIPP_HandleTypeDef        g_dcmipp_handle = {0};
DCMIPP_PipeConfTypeDef      g_dcmipp_pipecfg = {0};
DCMIPP_ParallelConfTypeDef  g_dcmipp_parallelcfg = {0};
static volatile uint8_t g_dcmipp_frame_done = 0U;

__ALIGNED(16) uint32_t camera_date_buf[1024 * 1024 * 2 / 4] __attribute__((section(".bss.ARM.__at_0x701F4000")));

void dcmipp_init(void)
{
    g_dcmipp_handle.Instance = DCMIPP;
    HAL_DCMIPP_Init(&g_dcmipp_handle);

    g_dcmipp_parallelcfg.Format           = DCMIPP_FORMAT_RGB565;
    g_dcmipp_parallelcfg.SwapCycles       = DCMIPP_SWAPCYCLES_ENABLE;
    g_dcmipp_parallelcfg.SwapBits         = DCMIPP_SWAPBITS_DISABLE;
    g_dcmipp_parallelcfg.VSPolarity       = DCMIPP_VSPOLARITY_LOW;
    g_dcmipp_parallelcfg.HSPolarity       = DCMIPP_HSPOLARITY_LOW;
    g_dcmipp_parallelcfg.PCKPolarity      = DCMIPP_PCKPOLARITY_RISING;
    g_dcmipp_parallelcfg.ExtendedDataMode = DCMIPP_INTERFACE_8BITS;
    g_dcmipp_parallelcfg.SynchroMode      = DCMIPP_SYNCHRO_HARDWARE;
    g_dcmipp_parallelcfg.SynchroCodes.FrameEndCode = 0;
    g_dcmipp_parallelcfg.SynchroCodes.FrameStartCode = 0;
    g_dcmipp_parallelcfg.SynchroCodes.LineEndCode = 0;
    g_dcmipp_parallelcfg.SynchroCodes.LineStartCode = 0;
    HAL_DCMIPP_PARALLEL_SetConfig(&g_dcmipp_handle, &g_dcmipp_parallelcfg);

    g_dcmipp_pipecfg.FrameRate = DCMIPP_FRAME_RATE_ALL;
    HAL_DCMIPP_PIPE_SetConfig(&g_dcmipp_handle, DCMIPP_PIPE0, &g_dcmipp_pipecfg);

    __HAL_DCMIPP_DISABLE_IT(&g_dcmipp_handle,
                            DCMIPP_IT_PIPE0_FRAME |
                            DCMIPP_IT_PIPE0_OVR |
                            DCMIPP_IT_AXI_TRANSFER_ERROR |
                            DCMIPP_IT_PARALLEL_SYNC_ERROR |
                            DCMIPP_IT_PIPE0_VSYNC |
                            DCMIPP_IT_PIPE0_LINE);
    __HAL_DCMIPP_ENABLE_IT(&g_dcmipp_handle, DCMIPP_IT_PIPE0_FRAME);
}

void HAL_DCMIPP_MspInit(DCMIPP_HandleTypeDef *phdcmipp)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    if (phdcmipp->Instance == DCMIPP)
    {
        __HAL_RCC_DCMIPP_CLK_ENABLE();
        __HAL_RCC_DCMIPP_FORCE_RESET();
        __HAL_RCC_DCMIPP_RELEASE_RESET();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        gpio_init_struct.Pin = GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = GPIO_AF13_DCMIPP;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4;
        HAL_GPIO_Init(GPIOE, &gpio_init_struct);

        gpio_init_struct.Pin = GPIO_PIN_3;
        gpio_init_struct.Alternate = GPIO_AF5_DCMIPP;
        HAL_GPIO_Init(GPIOG, &gpio_init_struct);

        HAL_NVIC_SetPriority(DCMIPP_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DCMIPP_IRQn);
    }
}

void dcmipp_start(void)
{
    g_dcmipp_frame_done = 0U;
    SCB_CleanInvalidateDCache();
    HAL_DCMIPP_PIPE_Start(&g_dcmipp_handle, DCMIPP_PIPE0, (uint32_t)camera_date_buf, DCMIPP_MODE_SNAPSHOT);

    while (g_dcmipp_frame_done == 0U)
    {
    }

    HAL_DCMIPP_PIPE_Stop(&g_dcmipp_handle, DCMIPP_PIPE0);
    SCB_InvalidateDCache();
}

void dcmipp_stop(void)
{
    HAL_DCMIPP_PIPE_Stop(&g_dcmipp_handle, DCMIPP_PIPE0);
}

uint8_t dcmipp_wait_frame(uint8_t frame_mark, uint32_t timeout_ms)
{
    const uint32_t start_tick = HAL_GetTick();

    while ((uint8_t)(g_ov_frame - frame_mark) == 0U)
    {
        if ((HAL_GetTick() - start_tick) > timeout_ms)
        {
            return 1U;
        }
    }

    return 0U;
}

void dcmipp_invalidate_frame_cache(void)
{
    SCB_InvalidateDCache();
}

void DCMIPP_IRQHandler(void)
{
    HAL_DCMIPP_IRQHandler(&g_dcmipp_handle);
}

void HAL_DCMIPP_PIPE_FrameEventCallback(DCMIPP_HandleTypeDef *pHdcmipp, uint32_t Pipe)
{
    if ((pHdcmipp->Instance == DCMIPP) && (Pipe == DCMIPP_PIPE0))
    {
        g_dcmipp_frame_done = 1U;
        g_ov_frame++;
    }
}
