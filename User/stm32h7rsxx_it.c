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
#include "./BSP/LED/led.h"
#include "./SYSTEM/delay/delay.h"
/* USER CODE END Includes */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void fault_halt(const char *name)
{
  printf("\r\n[FAULT] %s\r\n", name);
  printf("[FAULT] CFSR=0x%08" PRIX32 " HFSR=0x%08" PRIX32 "\r\n", SCB->CFSR, SCB->HFSR);
  printf("[FAULT] MMFAR=0x%08" PRIX32 " BFAR=0x%08" PRIX32 "\r\n", SCB->MMFAR, SCB->BFAR);

  while (1)
  {
    LED0_TOGGLE();
    delay_ms(120);
  }
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
void HardFault_Handler(void)
{
  fault_halt("HardFault");
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  fault_halt("MemManage");
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  fault_halt("BusFault");
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  fault_halt("UsageFault");
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
