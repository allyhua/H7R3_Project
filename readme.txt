STM32H7R3 自助结账项目接力入口
最后更新：2026-05-21

新对话先读：
- `AGENTS.md`
- `.agents\skills\embedded-project\SKILL.md`
- `START_HERE.md`

历史完整记录已归档到：
- `readme_archive.txt`

当前工程主线：
- `Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx`
- `Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx`

当前重点模块：
- `User\main.c`
- `User\ui\checkout_ui.c`
- `User\app\camera_workflow.c`
- `User\app\payment_qr.c`

当前状态摘要：
- 主识别页、购物车页、扫码支付页、支付成功页都已接入。
- 字库使用 `Middlewares\LVGL\Font\Font20.c`，新增中文文案前先更新 `checkout_ui_font_texts.txt` 再生成字体。
- full / fast 两个工程都要保持可编译。

最近一次已处理的问题：
- 扫码支付识别成功后偶发 MemManage，已限制 DCMIPP cache invalidate 只覆盖摄像头帧区域，避免误伤 QR 灰度缓冲和 ATKQR 私有堆。

本次更新：
- 进一步做了三个基础优化：
  - `Drivers\BSP\DCMIPP\dcmipp.h/.c`：新增 `DCMIPP_CAPTURE_WIDTH/HEIGHT/BYTES_PER_PIXEL`，`DCMIPP_CAPTURE_CACHE_BYTES` 改为由实际 RGB565 帧尺寸计算，并加编译期检查，避免 cache 失效范围和帧布局脱节。
  - `User\stm32h7rsxx_it.c`：HardFault / MemManage / BusFault / UsageFault 改为 naked handler 转发，fault 日志新增 stacked `R0/R1/R2/R3/R12/LR/PC/xPSR`，后续可直接用 PC/LR 定位异常地址。
  - `User\main.c`、`User\app\payment_qr.c`、`Drivers\BSP\DCMIPP\dcmipp.c`：新增轻量日志等级宏。默认保留启动、支付主流程和错误日志；QR 模块默认只保留错误日志，高频扫描日志需打开 `PAYMENT_QR_VERBOSE_SCAN_LOG` 或提高 `PAYMENT_QR_LOG_LEVEL`。
- 修复付款成功页及相关弹窗残留乱码/缺字：
  - `User\ui\checkout_ui.c` 中“返回”“返回首页”“无效的付款码”“购物车为空，无法结账”“结账完成”等文案恢复为正常中文。
  - `Middlewares\LVGL\Font\checkout_ui_font_texts.txt` 补充“支付成功”“支付方式”“扫码支付（模拟）”“感谢您的购买！”“订单已完成。”等成功页文案。
  - 已重新生成 `Middlewares\LVGL\Font\Font20.c/.h`，当前 unique chars 为 188。
- 修复付款成功返回后主识别页仍显示旧图的问题：
  - 支付成功进入首页前主动清空主页面识别结果并隐藏旧摄像头预览，恢复默认购物框占位。
- 修复再次进入扫码支付页时中间框沿用旧摄像头图的问题：
  - 新增 `checkout_ui_reset_payment_camera_preview()`，每次进入支付页和启动支付扫码时恢复默认模拟二维码占位。
- 优化二维码识别卡顿：
  - 默认 ROI 从 `320/280/240/200/160/120 + normal/hmirror` 最多 12 次解码，收敛为 `280/240/200/160 + normal` 最多 4 次解码。
  - 本轮继续改为动态扫描顺序：优先最近一次成功识别的 ROI；没有历史时先扫 `240`，再回退 `280/200/160`，避免每帧固定从最大尺寸开始。
  - 默认关闭每次 `ret=3` 的高频串口日志，仅保留成功/有效码等关键日志。
  - 支付页摄像头预览改为约 500ms 刷新一次，避免每次解码前都重绘。
  - 去掉解码前额外 `lv_timer_handler()`，避免在扫码路径中重复插入 UI 刷新。
- 修复支付页扫码中点击“重新扫描”后卡在 `[PAY] payment QR rescan`：
  - `User\main.c`：`main_payment_rescan()` 先判断支付摄像头是否已处于扫码运行态。
  - 若已经在 `PAYMENT_FLOW_SCANNING`，仅重置 QR 状态和计时，不再执行 `dcmipp_stop()`/`dcmipp_start_continuous()`，也不清掉当前摄像头预览。
  - 只有超时/无效码等暂停态重新扫码时，才重新启动连续采集，并补充 init/start 失败日志。
  - 支付扫码等待新帧超过 1 秒时，会打印 frame/mark 诊断日志并自动重启一次 DCMIPP 连续采集，便于区分按钮回调与摄像头帧中断问题；自动重启限制为每次支付流程一次，避免反复 stop/start。
- 修复支付页连续采集只出一帧的问题：
  - `Drivers\BSP\DCMIPP\dcmipp.c`：`dcmipp_start()` / `dcmipp_start_continuous()` 在调用 HAL start 前显式写 `P0FCTCR.CPTMODE`。
  - 原因是 HAL 的 `HAL_DCMIPP_PIPE_Start()` 对 capture mode 使用 `|= CaptureMode`，从 snapshot 切到 continuous 时传入 0 无法清掉旧 snapshot 位，导致连续模式可能仍按 snapshot 只采一帧。
- 修复支付成功后下一轮商品识别沿用上一张二维码帧的问题：
  - `Drivers\BSP\DCMIPP\dcmipp.c`：新增启动/停止前后的旧帧事件清理，清 `DCMIPP_FLAG_PIPE0_FRAME` 和 NVIC pending。
  - 原因是支付连续采集停止后可能残留帧完成标志；下一次商品 snapshot 一打开中断就消费旧事件，导致 `dcmipp_start()` 误判新帧已完成，实际仍识别 `camera_date_buf` 中的上一张二维码图。
- 首页“继续购物”按钮：
  - 原先接空回调，无实际动作。
  - 现在点击后隐藏当前商品预览图，恢复默认购物框占位；不清空购物车，不改变结账/支付状态。
- 重量稳定判定提速：
  - `User\app\camera_workflow.c/.h`：HX711 中值滤波从 5 点改为 3 点，采样间隔从 100ms 改为 60ms。
  - WAIT_STABLE 新增连续稳定计数：重量变化 `<= CAMERA_STABLE_DELTA_G` 连续 2 次，且等待超过 180ms，即可提前进入拍照识别。
  - 明显波动 `> 60g` 才重置稳定起点；普通小波动只清稳定计数，避免反复拖慢。
- 降低 ATKQR 扫码期间 UsageFault 风险：
  - `Drivers\CMSIS\Device\ST\STM32H7RSxx\Source\Templates\arm\startup_stm32h7r3xx.s`：主栈从 `0x1000` 提高到 `0x4000`。ATKQR 文档要求至少 `0x1000`，当前主循环还叠加 LVGL/扫码/图像转换，保留更多余量。

本机编译验证：
- full：`F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`，0 Error(s), 0 Warning(s)。
- fast：`F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`，0 Error(s), 0 Warning(s)。
- 注意：本轮 Keil 命令行受已打开/锁定的 Output 产物影响，没有刷新 `Output` 下的旧 axf/map；已用同一工程 lnp 手动重编 `startup/main/dcmipp` 并链接生成 `Projects\MDK-ARM\atk_h7r3_full_camrun_fast_manual.hex`，manual map 确认 STACK 为 `0x4000`。
- 本轮 QR 动态扫描改动已用 ArmClang 对 `User\app\payment_qr.c` 做单文件语法检查，通过；当前 Keil 增量/重建命令未刷新 `payment_qr.o`，需要在 uVision 中 Clean/Rebuild 后再下载验证。

需要板上验证：
- 付款成功页“感谢您的购买！”、“订单已完成。”、“返回首页”等文案是否完整显示。
- 付款成功返回首页后，中间图像区域是否恢复默认购物框占位。
- 第二次点击结账进入扫码支付页时，中间框是否先显示默认模拟二维码，而不是上一张商品图或二维码图。
- 扫码时界面卡顿是否明显减轻；若 normal 单路识别率下降，再评估是否临时打开 `PAYMENT_QR_SCAN_TRY_HMIRROR`。
- 支付页正在扫码时点击“重新扫描”，串口不应再停在 `[PAY] payment QR rescan`，后续应继续扫码、超时或识别成功。
- 若仍无摄像头画面，关注是否出现 `[PAY] payment camera frame stalled, restart continuous`；出现则说明主循环未等到 DCMIPP 帧中断。

完整背景、历史变更、验证记录见 `START_HERE.md` 和 `readme_archive.txt`。
