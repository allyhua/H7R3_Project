# 自助结账项目接力说明

## 先读什么

1. `AGENTS.md`
2. `.agents/skills/embedded-project/SKILL.md`
3. `readme.txt`
4. `readme_archive.txt`（只在需要追历史时再读）

## 当前项目

- STM32H7R3 自助结账固件
- 主工程：`atk_h7r3_full_camrun`
- 快速工程：`atk_h7r3_full_camrun_fast`
- 语言栈：STM32 HAL / LVGL / BSP / X-CUBE-AI

## 当前功能

- 商品识别
- 购物车管理
- 扫码支付页
- 支付成功页
- 无效付款码弹窗
- 支付超时弹窗

## 当前关键文件

- `User\main.c`
- `User\ui\checkout_ui.c`
- `User\app\camera_workflow.c`
- `User\app\payment_qr.c`
- `User\app\cart_service.c`
- `Middlewares\LVGL\Font\checkout_ui_font_texts.txt`
- `tools\generate_checkout_ui_font.py`

## 当前约定

- 新增中文文案先改 `checkout_ui_font_texts.txt`，再生成 `Font20.c/.h`
- full / fast 两个 Keil 工程都要保持能编译
- 实质改动结束前要更新 `readme.txt`

## 最近状态

- 刚完成三项基础优化：DCMIPP cache 范围由 480x360 RGB565 帧尺寸宏计算；fault 日志新增 stacked PC/LR 等寄存器；支付/QR/DCMIPP 新链路日志增加等级宏收口
- full / fast 构建均为 0 Error(s), 0 Warning(s)
- 刚处理付款成功页缺字/乱码、支付返回后旧图残留、二次进入支付页旧预览残留、扫码卡顿问题
- `checkout_ui_font_texts.txt` 已补成功页文案并重新生成 `Font20.c/.h`，当前 unique chars 188
- `checkout_ui_reset_payment_camera_preview()` 会在进入支付页/启动支付扫码时恢复默认模拟二维码占位
- 扫码默认尝试范围已收敛为 240/200/160 normal 单路，默认关闭高频 ret=3 日志，支付页预览约 500ms 刷新一次
- 用户要求补一个更大的支付 QR 识别尺寸，当前默认尝试范围改为 280/240/200/160 normal 单路
- 扫码支付识别成功后偶发 MemManage 已做最小修复
- `Drivers\BSP\DCMIPP\dcmipp.c` 现在只 invalidate 摄像头帧前 1MB，避开 `0x702F4000` 起的 QR 灰度缓冲和后续 ATKQR 私有堆
- 刚继续修复支付页扫码卡住/UsageFault：DCMIPP start 前显式设置 `P0FCTCR.CPTMODE`，避免 snapshot 旧位导致 continuous 只采一帧；支付帧等待自动重启限制为每次支付一次；startup 主栈从 `0x1000` 提到 `0x4000`
- 又修复支付成功后下一轮商品识别沿用上一张二维码帧：DCMIPP start/stop 会清 `DCMIPP_FLAG_PIPE0_FRAME` 和 NVIC pending，避免 snapshot 消费支付连续采集残留帧事件
- 首页“继续购物”按钮原先为空回调，现在点击会隐藏当前商品图并恢复默认购物框占位，不清空购物车
- 重量稳定判定已做速度优先优化：HX711 5 点中值改 3 点、采样 100ms 改 60ms，WAIT_STABLE 连续 2 次稳定且超过 180ms 可提前触发识别，明显波动 >60g 才重置稳定起点
- 当前普通 Keil 命令行没有刷新 `Output` 旧产物；已手动重编 `startup/main/dcmipp` 并链接出 `Projects\MDK-ARM\atk_h7r3_full_camrun_fast_manual.hex`，manual map 确认 STACK=`0x4000`
- 目前建议下载 `camrun_fast`，重复扫码验证成功页文案、页面默认图恢复、扫码流畅度，以及是否还出现 `elapsed` 异常和 MemManage

## 历史

完整历史、构建记录、板上验证结论在 `readme_archive.txt`
