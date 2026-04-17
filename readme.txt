/**
 * 项目说明 — 视觉识别自助结账系统（基于 STM32H7R3）
 *
 * 概述
 * 本工程为基于 STM32H7R3 的视觉识别自助结账示例：通过摄像头采集商品图像，使用内置的 X-CUBE-AI 模型进行分类识别，
 * 结合称重模块（HX711）、LCD 显示和串口通信，实现简单的无人收银（演示/实验平台）。工程基于 STM32 HAL/LL 驱动。
 *
 * 主要功能
 * - 摄像头采集并通过神经网络模型进行图像分类（模型及标签位于 `Output/`）
 * - LCD 实时显示识别结果与界面信息
 * - HX711 称重模块用于称量并校验商品重量（见 `Drivers/BSP/HX711/`）
 * - 按键与 LED 指示交互状态
 * - 串口（USART1）用于调试或与上位机通信（默认 115200, 8N1）
 *
 * 硬件平台（示例）
 * - MCU: STM32H7R3 系列
 * - 摄像头: OV2640（或兼容模块），接口代码位于 `Drivers/BSP/OV2640/` 和 `Drivers/OV2640/`
 * - LCD: TFT 显示模块，驱动位于 `Drivers/BSP/LCD/` 与 `Drivers/LCD/`
 * - 称重: HX711，驱动位于 `Drivers/BSP/HX711/` 和 `Drivers/HX711/`
 * - LED0: PD14
 * - USART1: TX - PB14, RX - PB15 （波特率 115200）
 *
 * 重要文件位置
 * - 主程序：`User/main.c`（应用入口，系统/外设初始化与主流程）
 * - 串口：`Drivers/SYSTEM/usart/usart.c`（UART 初始化及接收/发送封装）
 * - 摄像头：`Drivers/BSP/OV2640/`，`Drivers/OV2640/`（采集/配置）
 * - 显示：`Drivers/BSP/LCD/`, `Drivers/LCD/`（界面绘制）
 * - 称重：`Drivers/BSP/HX711/`, `Drivers/HX711/`
 * - AI 模型及标签：`Output/`（包含 `network_data.*`, `ai_class_labels.*`，以及 X-CUBE-AI 产生的文件）
 * - HAL 驱动：`Drivers/STM32H7RSxx_HAL_Driver/`
 * - 项目文件（Keil MDK-ARM）：`Projects/MDK-ARM/atk_h7r3_full.uvprojx`（或同目录下的 uvprojx/uvoptx）
 *
 * 快速开始（在 Windows + Keil uVision 下）
 * 1) 使用 Keil uVision 打开 `Projects/MDK-ARM/atk_h7r3_full.uvprojx`。
 * 2) 选择合适的目标工具链/调试器（例如 ST-Link），编译工程（Build）。
 * 3) 将板子通过 ST-Link 连接到主机，下载程序（Flash/Load）并重启 MCU。
 * 4) 打开串口调试工具，配置：115200 8N1。程序运行过程中会通过串口输出调试/识别信息。
 *
 * 使用说明（演示流程示例）
 * - 上电后系统初始化摄像头、LCD、称重与神经网络模块。
 * - 将商品放在称重台并对准摄像头，系统会自动拍照并进行识别，结果会显示在 LCD 上，并通过串口输出识别类别与置信度。
 * - 根据需要按确认键完成结账或重拍。
 *
 * 开发与调试提示
 * - 若要查看/替换模型：参考 `Output/` 中的网络数据与标签文件，或在 X-CUBE-AI 中重新生成模型。
 * - 若要调整串口波特率或引脚，请查看 `Drivers/SYSTEM/usart/usart.c` 与 `User` 中的初始化代码。
 * - 若出现摄像头或 LCD 相关问题，优先检查电源、时钟与接口连线，并在 `Drivers/BSP/OV2640/` 与 `Drivers/BSP/LCD/` 中查找初始化过程。
 *
 * 许可证与致谢
 * 本工程包含来自 STM32 HAL、CMSIS 与第三方 BSP 驱动代码，版权归原作者/供应商所有（详见 `Drivers/` 内各文件头部）。
 * 原厂/示例代码与技术支持信息：
 * - 公司/出处：阿里云/创客/示例来源（示例源头信息保留于原始 readme 与驱动注释）
 * - 参考资源：www.alientek.com, http://www.openedv.com
 *
 * 联系/支持（示例保留）
 * 电话: 020-38271790
 * 售后: 020-36773971
 * 网站: www.alientek.com
 *
 * 简短变更记录
 * - 2026-04-14: 将原简短说明扩展为结构化 README，增加项目简介、文件位置、快速开始与使用说明。
 */