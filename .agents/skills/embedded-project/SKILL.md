---
name: embedded-project
description: Use this skill when a project involves STM32, HAL, FreeRTOS, LVGL, BSP, startup files, linker scripts, LCD, touch, UART, SPI, I2C, DMA, interrupts, or C firmware.
---

# Embedded Project

当项目涉及 STM32、HAL、LL、CMSIS、FreeRTOS、LVGL、BSP、启动文件、链接脚本、LCD、触摸屏、串口、SPI、I2C、DMA、中断或 C 固件时使用本技能。

## 总原则

- 嵌入式项目优先保守修改。
- 先读懂硬件、时钟、引脚、外设和任务关系。
- 不凭 PC 软件经验直接推断固件行为。
- 不随意改初始化顺序、时序、延时和中断配置。
- 无法实机验证时，必须给出板上验证步骤。

## 修改前检查

确认或查找：

- MCU/开发板型号。
- HAL/LL/CMSIS/FreeRTOS/LVGL 版本。
- 时钟树和外设时钟来源。
- GPIO 复用、片选、复位、背光、中断脚。
- DMA stream/channel、buffer 地址和 cache 策略。
- FreeRTOS 任务、队列、信号量、互斥锁和优先级。
- 构建系统：STM32CubeIDE、Makefile、CMake、Keil、IAR。

## 高风险文件

修改这些文件前必须特别谨慎：

- 启动文件：`startup_*.s`
- 链接脚本：`*.ld`、scatter file
- 系统时钟：`SystemClock_Config`
- 中断：`stm32*_it.c`、vector table
- FreeRTOS 配置：`FreeRTOSConfig.h`
- LVGL 配置：`lv_conf.h`
- BSP 初始化、LCD/触摸驱动、DMA 配置

如果必须改，说明原因和验证方式。

## C 代码规则

- 保持简单直接，匹配项目现有风格。
- 谨慎处理 `volatile`、`static`、弱符号、段属性、对齐属性。
- 不随意改变全局变量生命周期。
- 注意整数溢出、数组越界、指针生命周期、栈大小。
- 中断与任务共享数据时考虑原子性、临界区或同步机制。

## FreeRTOS 注意事项

- ISR 中使用 `FromISR` API。
- 不在中断中调用可能阻塞的函数。
- 检查任务栈大小和优先级反转风险。
- 避免长时间关中断。
- 检查队列、信号量、互斥锁创建和失败路径。

## LVGL / 显示注意事项

- 确认 `lv_tick_inc` 或 tick 定时是否正常。
- 确认 `lv_timer_handler` 是否被周期调用。
- 检查 flush 回调是否最终调用 `lv_disp_flush_ready`。
- 检查颜色格式、横竖屏、分辨率、stride、buffer 大小。
- DMA 或缓存系统中检查 clean/invalidate 和内存区域属性。
- 屏幕黑屏时同时检查背光、复位、初始化序列和接口时序。

## 串口 / SPI / I2C / DMA

- 串口：检查波特率、GPIO 复用、时钟、重定向、TX/RX 是否接反。
- SPI：检查 CPOL/CPHA、片选、频率、位宽、DMA 完成回调。
- I2C：检查地址 7-bit/8-bit 表示、上拉、电源、ACK、超时。
- DMA：检查方向、长度、对齐、cache、一致性和回调。

## 验证建议

优先运行：

- 项目构建命令。
- 静态检查或编译警告检查。
- 最小外设测试。
- 串口日志验证。
- 实机烧录和现象确认。

回复中明确区分“已在本机验证”和“需要用户在硬件上验证”。
