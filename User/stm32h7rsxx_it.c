/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7rsxx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7rsxx_it.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <inttypes.h>
#include <stdio.h>

#include "main.h"
#include "ui/lvgl_port.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/delay/delay.h"
/* USER CODE END Includes */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void fault_halt(const char *name, const uint32_t *stack)
{
  printf("\r\n[FAULT] %s\r\n", name);
  printf("[FAULT] CFSR=0x%08" PRIX32 " HFSR=0x%08" PRIX32 "\r\n", SCB->CFSR, SCB->HFSR);
  printf("[FAULT] MMFAR=0x%08" PRIX32 " BFAR=0x%08" PRIX32 "\r\n", SCB->MMFAR, SCB->BFAR);
  if (stack != NULL)
  {
    printf("[FAULT] R0=0x%08" PRIX32 " R1=0x%08" PRIX32 " R2=0x%08" PRIX32 " R3=0x%08" PRIX32 "\r\n",
           stack[0], stack[1], stack[2], stack[3]);
    printf("[FAULT] R12=0x%08" PRIX32 " LR=0x%08" PRIX32 " PC=0x%08" PRIX32 " xPSR=0x%08" PRIX32 "\r\n",
           stack[4], stack[5], stack[6], stack[7]);
  }

  while (1)
  {
    LED0_TOGGLE();
    delay_ms(120);
  }
}

__attribute__((used, noinline)) static void HardFault_Handler_C(uint32_t *stack)
{
  fault_halt("HardFault", stack);
}

__attribute__((used, noinline)) static void MemManage_Handler_C(uint32_t *stack)
{
  fault_halt("MemManage", stack);
}

__attribute__((used, noinline)) static void BusFault_Handler_C(uint32_t *stack)
{
  fault_halt("BusFault", stack);
}

__attribute__((used, noinline)) static void UsageFault_Handler_C(uint32_t *stack)
{
  fault_halt("UsageFault", stack);
}
/* USER CODE END 0 */

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
__attribute__((naked)) void HardFault_Handler(void)
{
  __asm volatile(
    "tst lr, #4\n"
    "ite eq\n"
    "mrseq r0, msp\n"
    "mrsne r0, psp\n"
    "b HardFault_Handler_C\n");
}

/**
  * @brief This function handles Memory management fault.
  */
__attribute__((naked)) void MemManage_Handler(void)
{
  __asm volatile(
    "tst lr, #4\n"
    "ite eq\n"
    "mrseq r0, msp\n"
    "mrsne r0, psp\n"
    "b MemManage_Handler_C\n");
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
__attribute__((naked)) void BusFault_Handler(void)
{
  __asm volatile(
    "tst lr, #4\n"
    "ite eq\n"
    "mrseq r0, msp\n"
    "mrsne r0, psp\n"
    "b BusFault_Handler_C\n");
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
__attribute__((naked)) void UsageFault_Handler(void)
{
  __asm volatile(
    "tst lr, #4\n"
    "ite eq\n"
    "mrseq r0, msp\n"
    "mrsne r0, psp\n"
    "b UsageFault_Handler_C\n");
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void GPDMA1_Channel0_IRQHandler(void)
{
  lvgl_port_gpdma_irqhandler();
}
