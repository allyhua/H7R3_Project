/**
 ****************************************************************************************************
 * @file        dcmipp.c
 * @brief       DCMIPP driver
 ****************************************************************************************************
 */

#include "./BSP/DCMIPP/dcmipp.h"
#include <stdio.h>
#include <stdint.h>

extern int printf(const char *format, ...);

extern volatile uint8_t g_ov_frame;

DCMIPP_HandleTypeDef        g_dcmipp_handle = {0};
DCMIPP_PipeConfTypeDef      g_dcmipp_pipecfg = {0};
DCMIPP_ParallelConfTypeDef  g_dcmipp_parallelcfg = {0};
static volatile uint8_t g_dcmipp_frame_done = 0U;
#define DCMIPP_SNAPSHOT_TIMEOUT_MS  (1000U)
#define DCMIPP_FRAME_CACHE_LINE     (32UL)
#ifndef DCMIPP_LOG_LEVEL
#define DCMIPP_LOG_LEVEL            (1U)
#endif
#define DCMIPP_LOG_ERROR            (1U)
#define DCMIPP_LOG(level, ...)      do { if (DCMIPP_LOG_LEVEL >= (level)) { printf(__VA_ARGS__); } } while (0)

#if (DCMIPP_CAPTURE_CACHE_BYTES > DCMIPP_CAMERA_BUFFER_BYTES)
#error "DCMIPP capture cache range exceeds camera_date_buf"
#endif

typedef char dcmipp_capture_cache_line_check[(DCMIPP_FRAME_CACHE_LINE == 32UL) ? 1 : -1];
typedef char dcmipp_capture_buffer_word_check[((DCMIPP_CAMERA_BUFFER_BYTES % 4UL) == 0UL) ? 1 : -1];

__ALIGNED(16) uint32_t camera_date_buf[DCMIPP_CAMERA_BUFFER_BYTES / 4UL] __attribute__((section(".bss.ARM.__at_0x701F4000")));

static void dcmipp_invalidate_camera_buffer_cache(void)
{
    uintptr_t start = (uintptr_t)camera_date_buf;
    uintptr_t end = start + DCMIPP_CAPTURE_CACHE_BYTES;

    /*
     * Only invalidate the active RGB565 capture frame. The second half of
     * camera_date_buf is reused by payment QR grayscale data and ATKQR heap.
     */
    start &= ~(DCMIPP_FRAME_CACHE_LINE - 1UL);
    end = (end + DCMIPP_FRAME_CACHE_LINE - 1UL) & ~(DCMIPP_FRAME_CACHE_LINE - 1UL);
    SCB_InvalidateDCache_by_Addr((uint32_t *)start, (int32_t)(end - start));
}

static void dcmipp_clear_pending_frame_event(void)
{
    __HAL_DCMIPP_CLEAR_FLAG(&g_dcmipp_handle, DCMIPP_FLAG_PIPE0_FRAME);
    HAL_NVIC_ClearPendingIRQ(DCMIPP_IRQn);
}

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

int dcmipp_start(void)
{
    const uint32_t start_tick = HAL_GetTick();

    g_dcmipp_frame_done = 0U;
    dcmipp_clear_pending_frame_event();
    dcmipp_invalidate_camera_buffer_cache();
    MODIFY_REG(g_dcmipp_handle.Instance->P0FCTCR,
               DCMIPP_P0FCTCR_CPTMODE,
               DCMIPP_MODE_SNAPSHOT);
    HAL_DCMIPP_PIPE_Start(&g_dcmipp_handle, DCMIPP_PIPE0, (uint32_t)camera_date_buf, DCMIPP_MODE_SNAPSHOT);

    while (g_dcmipp_frame_done == 0U)
    {
        if ((HAL_GetTick() - start_tick) > DCMIPP_SNAPSHOT_TIMEOUT_MS)
        {
            HAL_DCMIPP_PIPE_Stop(&g_dcmipp_handle, DCMIPP_PIPE0);
            DCMIPP_LOG(DCMIPP_LOG_ERROR, "[CAM] Snapshot timeout waiting for DCMIPP frame event\r\n");
            return -1;
        }
    }

    HAL_DCMIPP_PIPE_Stop(&g_dcmipp_handle, DCMIPP_PIPE0);
    dcmipp_invalidate_camera_buffer_cache();
    return 0;
}

int dcmipp_start_continuous(void)
{
    g_dcmipp_frame_done = 0U;
    dcmipp_clear_pending_frame_event();
    dcmipp_invalidate_camera_buffer_cache();
    MODIFY_REG(g_dcmipp_handle.Instance->P0FCTCR,
               DCMIPP_P0FCTCR_CPTMODE,
               DCMIPP_MODE_CONTINUOUS);
    if (HAL_DCMIPP_PIPE_Start(&g_dcmipp_handle,
                              DCMIPP_PIPE0,
                              (uint32_t)camera_date_buf,
                              DCMIPP_MODE_CONTINUOUS) != HAL_OK)
    {
        DCMIPP_LOG(DCMIPP_LOG_ERROR, "[CAM] Continuous DCMIPP start failed\r\n");
        return -1;
    }

    return 0;
}

void dcmipp_stop(void)
{
    HAL_DCMIPP_PIPE_Stop(&g_dcmipp_handle, DCMIPP_PIPE0);
    g_dcmipp_frame_done = 0U;
    dcmipp_clear_pending_frame_event();
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
    dcmipp_invalidate_camera_buffer_cache();
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
