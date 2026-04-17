#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/HYPERRAM/hyperram.h"
#include "./BSP/NORFLASH/norflash_ex.h"
#include <inttypes.h>
#include <stdio.h>
#include "app_x-cube-ai.h"
#include "app/camera_workflow.h"
#include "network.h"
#include "network_data.h"

volatile uint8_t g_ov_frame = 0U;

void jpeg_data_process(void)
{
    /* The camera workflow uses single-frame RGB565 capture after weight becomes stable. */
}

static void boot_error_halt(void)
{
    while (1)
    {
        LED0_TOGGLE();
        delay_ms(150);
    }
}

static void boot_enable_fault_traps(void)
{
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk |
                  SCB_SHCSR_BUSFAULTENA_Msk |
                  SCB_SHCSR_USGFAULTENA_Msk;
}

static void boot_log_weight_bytes(void)
{
    uint8_t source_header[16];
    const uint8_t *runtime_bytes = (const uint8_t *)ai_network_data_weights_get();
    const uintptr_t runtime_weights_addr = (uintptr_t)runtime_bytes;
    uint32_t idx;

    printf("[BOOT] Embedded model: %s\r\n", AI_NETWORK_ORIGIN_MODEL_NAME);
    printf("[BOOT] Runtime weights @ 0x%08" PRIXPTR " (%u bytes)\r\n",
           runtime_weights_addr,
           (unsigned)AI_NETWORK_DATA_WEIGHTS_SIZE);

    if ((runtime_weights_addr >= 0x70000000UL) && (runtime_weights_addr < 0x80000000UL))
    {
        printf("[BOOT] Weight copy-to-RAM is enabled\r\n");
    }

    if (norflash_ex_read(0x00000000U, source_header, sizeof(source_header)) != 0)
    {
        printf("[BOOT] NOR Flash read failed @ offset 0x00000000\r\n");
        boot_error_halt();
    }

    printf("[BOOT] NOR[0x00000000..0x0000000F] =");
    for (idx = 0U; idx < sizeof(source_header); ++idx)
    {
        printf(" %02X", source_header[idx]);
    }
    printf("\r\n");

    printf("[BOOT] MEM[0x%08" PRIXPTR "..0x%08" PRIXPTR "] =",
           runtime_weights_addr,
           runtime_weights_addr + (uintptr_t)sizeof(source_header) - 1U);
    for (idx = 0U; idx < sizeof(source_header); ++idx)
    {
        printf(" %02X", runtime_bytes[idx]);
    }
    printf("\r\n");
}

int main(void)
{
    camera_workflow_state_t camera_state;

    sys_mpu_config();
    sys_cache_enable();
    HAL_Init();
    boot_enable_fault_traps();
    sys_stm32_clock_init(300, 6, 2);
    delay_init(600);
    usart_init(115200);
    led_init();

    printf("\r\n============= WEIGHT LABEL DEMO START =============\r\n");
    printf("[BOOT] USART1 ready on PB14/PB15 @115200\r\n");
    printf("[BOOT] Firmware build: auto-weight-stable-trigger-v1\r\n");
    printf("[BOOT] Initializing HyperRAM for copied weights and camera frame buffer...\r\n");

    if (hyperram_init() != 0U)
    {
        printf("[BOOT] HyperRAM init failed\r\n");
        boot_error_halt();
    }

    printf("[BOOT] HyperRAM init ok\r\n");
    printf("[BOOT] Initializing NOR Flash for AI weights...\r\n");

    if (norflash_ex_init() != 0U)
    {
        printf("[BOOT] NOR Flash init failed\r\n");
        boot_error_halt();
    }

    printf("[BOOT] NOR Flash init ok\r\n");
    boot_log_weight_bytes();
    printf("[BOOT] Initializing X-CUBE-AI...\r\n");
    MX_X_CUBE_AI_Init();

    if (camera_workflow_init(&camera_state) != 0)
    {
        printf("[BOOT] Camera init failed\r\n");
        boot_error_halt();
    }

    printf("[BOOT] System ready: stable weight triggers one capture per load\r\n");

    while (1)
    {
        camera_workflow_process(&camera_state);
        LED0_TOGGLE();
        delay_ms(10);
    }
}

void Error_Handler(void)
{
    printf("[BOOT] Error_Handler entered\r\n");
    boot_error_halt();
}
