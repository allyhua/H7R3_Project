# 视觉识别自助结账系统 — 项目结构说明

## 整体架构

```
实验5 串口通信实验/
├── User/                          # 应用层（用户代码）
│   ├── main.c                    # 主程序入口 ⭐ 核心流程
│   ├── main.h
│   ├── network.c/h               # 神经网络推理实现
│   ├── network_data.c/h          # 模型权重/参数数据
│   ├── network_data_params.c/h   # 模型配置（输入/输出形状）
│   ├── network_details.h         # 模型详细信息
│   ├── network_weights_bin.s     # 二进制权重（汇编格式）
│   ├── ai_class_labels.c/h       # 商品分类标签映射（0→苹果, 1→香蕉...）
│   ├── app_x-cube-ai.c/h         # X-CUBE-AI 中间件集成
│   ├── constants_ai.h            # AI 常数定义
│   ├── stm32h7rsxx_hal_conf.h   # HAL 配置（外设使能）
│   ├── stm32h7rsxx_it.c/h       # 中断服务例程（ISR）
│   ├── RTE_Components.h          # Keil 中间件声明
│   └── Script/                   # 链接脚本（内存分配）
│       ├── ATK-DNH7R3_flash_ROMxspi1.sct
│       └── ATK-DNH7R3_flash_ROMxspi1_fast.sct
│
├── Drivers/                       # 驱动层
│   ├── BSP/                      # Board Support Package（板级支持）
│   │   ├── OV2640/               # 摄像头驱动 📷
│   │   │   ├── ov2640.c/h
│   │   │   ├── ov2640cfg.h       # 摄像头配置
│   │   │   ├── ov2640af.h        # 自动对焦（可选）
│   │   │   └── sccb.c/h          # SCCB 通信协议（摄像头控制）
│   │   ├── LCD/                  # LCD 显示驱动 🖥️
│   │   │   ├── lcd.c/h           # LCD 操作接口
│   │   │   ├── lcd_ex.c          # 扩展功能（图形绘制）
│   │   │   └── lcdfont.h         # 字体库
│   │   ├── HX711/                # 称重模块（传感器） ⚖️
│   │   │   ├── hx711.c/h
│   │   │   └── 配置: 增益/分辨率
│   │   ├── KEY/                  # 按键输入 🔘
│   │   │   └── key.c/h
│   │   ├── LED/                  # LED 指示灯 💡
│   │   │   └── led.c/h
│   │   ├── DCMIPP/               # DCMI++ 摄像接口 📡
│   │   │   └── dcmipp.c/h        # 图像采集/DMA 配置
│   │   ├── HYPERRAM/             # 超大容量 RAM 📦
│   │   │   └── hyperram.c/h
│   │   ├── NORFLASH/             # NOR Flash（闪存存储）
│   │   │   ├── norflash.c/h
│   │   │   ├── norflash_ex.c/h
│   │   │   ├── norflash_mx25um25645g.c/h     # MX25 Flash 芯片
│   │   │   └── norflash_w25q128_dual.c/h    # W25Q128 Flash 芯片（Dual SPI）
│   │   └── readme.txt            # BSP 驱动说明
│   │
│   ├── CMSIS/                    # ARM CMSIS 核心库
│   │   ├── Device/               # STM32H7R3 设备文件
│   │   └── Include/              # CMSIS 头文件
│   │
│   ├── STM32H7RSxx_HAL_Driver/  # ST 官方 HAL 驱动库
│   │   ├── Inc/                  # 驱动头文件
│   │   │   └── stm32h7rsxx_hal_*.h
│   │   │       ├── stm32h7rsxx_hal.h              # HAL 主文件
│   │   │       ├── stm32h7rsxx_hal_uart.h        # UART（串口）
│   │   │       ├── stm32h7rsxx_hal_gpio.h        # GPIO（引脚）
│   │   │       ├── stm32h7rsxx_hal_dma.h         # DMA（直接内存访问）
│   │   │       ├── stm32h7rsxx_hal_dcmipp.h      # DCMI++（摄像）
│   │   │       ├── stm32h7rsxx_hal_rcc.h         # RCC（时钟）
│   │   │       ├── stm32h7rsxx_hal_cortex.h      # Cortex-M7 核心
│   │   │       └── ...
│   │   ├── Src/                  # 驱动实现文件
│   │   │   └── stm32h7rsxx_hal_*.c
│   │   └── readme.txt            # HAL 驱动说明
│   │
│   ├── SYSTEM/                   # 系统级模块
│   │   ├── delay/                # 延时驱动
│   │   │   ├── delay.c/h
│   │   │   └── DWT 计时器配置
│   │   ├── sys/                  # 系统初始化
│   │   │   ├── sys.c/h
│   │   │   └── 时钟/电源配置
│   │   └── usart/                # 串口通信 📡
│   │       ├── usart.c/h         # UART 初始化、收发
│   │       ├── 波特率: 115200
│   │       └── 接收中断处理
│   └── readme.txt                # Drivers 总说明
│
├── Middlewares/                  # 中间件
│   ├── ST/
│   │   └── AI/                   # X-CUBE-AI（STM32 AI 中间件）
│   │       └── 模型推理框架
│   └── readme.txt
│
├── Projects/                     # 工程配置
│   ├── MDK-ARM/                  # Keil uVision 工程文件
│   │   ├── atk_h7r3_full.uvprojx    # ⭐ 项目文件（编译配置）
│   │   ├── atk_h7r3_full.uvoptx     # 编辑器设置
│   │   ├── atk_h7r3_full.uvguix.allyh  # GUI 配置
│   │   └── DebugConfig/             # 调试配置
│   └── readme.txt
│
├── Output/                       # 编译输出（自动生成）
│   ├── *.axf                     # ELF 可执行文件
│   ├── *.hex                     # Intel HEX 固件
│   ├── *.map                     # 链接映射（内存布局）
│   ├── *.o                       # 对象文件
│   ├── *.d                       # 依赖文件
│   ├── *.htm                     # 构建报告（Keil 生成）
│   ├── *.lnp                     # 链接参数
│   ├── atk_h7r3_full.axf         # 最终输出的固件 📦
│   └── build_log.htm             # 详细构建日志
│
├── readme.txt                    # ⭐ 项目说明（已改进）
├── keilkill.bat                  # 清理脚本（删除编译产物）
└── PROJECT_STRUCTURE.md          # 本文件

```

---

## 关键文件详解

### 🎯 应用层（User/）

| 文件 | 功能 | 关键变量/函数 |
|-----|------|-------------|
| **main.c** | 主程序 | `main()` → 初始化 → 摄像头采集 → AI推理 → LCD显示 → 称重 |
| | | `g_ov_frame` → 摄像头帧更新标志 |
| | | `g_camera_last_result` → AI识别结果（类别、置信度） |
| | | `g_camera_weight_gram` → HX711 称重结果（克） |
| **network.c/h** | 神经网络接口 | `network_run()` → 执行推理 |
| **ai_class_labels.c/h** | 商品标签 | 标签列表：`["苹果", "香蕉", "橙子", ...]` |
| **app_x-cube-ai.c/h** | AI 中间件 | X-CUBE-AI 框架封装 |
| **stm32h7rsxx_it.c** | 中断处理 | `OV_FRAME_END_ISR()` → 摄像头帧中断 |
| | | `HAL_UART_RxCpltCallback()` → 串口接收中断 |

### 🔧 驱动层（Drivers/）

#### 摄像头（OV2640）
```c
// 典型流程：
OV2640_Init()         // 初始化
→ SCCB_WriteReg()     // 通过 SCCB 写配置寄存器
→ DCMIPP_Start()      // 启动图像采集（DMA）
→ ISR 触发            // 帧完成中断
```

#### LCD 显示
```c
LCD_Init()            // 初始化
→ LCD_Fill()          // 填充背景
→ LCD_DrawString()    // 显示文本（识别结果）
→ LCD_DrawImage()     // 绘制摄像头图像
```

#### HX711 称重
```c
HX711_Init()          // 初始化
→ HX711_Read()        // 读取称重值（24-bit ADC）
→ 中位数滤波          // 平滑噪声
→ g_camera_weight_gram = (raw - tare) / SCALE_FACTOR
```

#### 串口通信（UART）
```c
USART_Init(115200)              // 初始化波特率
→ HAL_UART_Receive_IT()         // 启用接收中断
→ HAL_UART_RxCpltCallback()     // 接收数据回调
→ 发送识别结果到上位机（调试/UI）
```

---

## 硬件资源映射

| 功能 | HAL/接口 | 引脚/资源 |
|------|---------|---------|
| 摄像头 | DCMI++/CSI | 专用接口 |
| 显示 | GPIO/SPI/LTDC | 并口或 SPI |
| 称重 | GPIO/ADC | HX711 的 CLK、DT 脚 |
| 按键 | GPIO/EXTI | 中断输入 |
| LED | GPIO | PD14 等 |
| 串口 | USART1 | TX=PB14, RX=PB15 |

---

## 编译与下载流程

1. **编译**（Keil uVision）
   - 打开 `Projects/MDK-ARM/atk_h7r3_full.uvprojx`
   - 点击 `Build`（F7）
   - 输出：`Output/atk_h7r3_full.axf` / `.hex`

2. **下载**（ST-Link）
   - 连接开发板
   - 点击 `Download`（F8）
   - 程序写入 Flash

3. **运行**
   - 重启 MCU
   - 打开串口调试工具（115200, 8N1）
   - 放置商品，系统自动识别并显示结果

---

## 文件重要性排序（优先阅读）

🔴 **必读**（系统核心）
- `User/main.c` — 应用主流程
- `Projects/MDK-ARM/atk_h7r3_full.uvprojx` — 编译配置

🟠 **重要**（外设驱动）
- `Drivers/BSP/OV2640/ov2640.c` — 摄像头
- `Drivers/BSP/LCD/lcd.c` — 显示
- `Drivers/BSP/HX711/hx711.c` — 称重
- `Drivers/SYSTEM/usart/usart.c` — 串口

🟡 **参考**（AI/模型）
- `User/network.c` — 推理实现
- `User/ai_class_labels.c` — 标签定义

---

## 常见修改点

| 场景 | 修改位置 |
|------|---------|
| 更改识别类别 | `User/ai_class_labels.c` |
| 更换 AI 模型 | `User/network_data.c` + `User/network_config.h` |
| 调整串口波特率 | `Drivers/SYSTEM/usart/usart.c` + `stm32h7rsxx_hal_conf.h` |
| 调整称重阈值 | `User/main.c` 中的 `HX711_SCALE_FACTOR` 等常数 |
| 修改摄像头参数 | `Drivers/BSP/OV2640/ov2640cfg.h` |
| 改变 LCD 显示内容 | `User/main.c` 的显示函数调用 |

---

**总结**：该工程遵循标准的嵌入式系统分层架构（应用 → 驱动 → HAL），结构清晰，便于维护与扩展。
