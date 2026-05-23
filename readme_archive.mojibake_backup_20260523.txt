STM32H7R3 鑷姪缁撹处椤圭洰浜ゆ帴璇存槑
鏈€鍚庢洿鏂帮細2026-05-13 17:25

杩欐槸缁欐柊 AI 瀵硅瘽鎺ュ姏鐢ㄧ殑椤圭洰浜ゆ帴鏂囦欢銆?鏂板紑瀵硅瘽鏃讹紝璇峰厛璁?AI 闃呰锛?
E:\H7R3\鑷姪缁撹处\readme.txt


0. 鍚庣画缁存姢瑙勫垯

閲嶈椤圭洰瑙勫垯锛?
- 浠ュ悗姣忔鏈夊疄璐ㄦ敼鍔紝閮借鍦ㄤ换鍔＄粨鏉熷墠鍚屾鏇存柊 readme.txt銆?- 瀹炶川鏀瑰姩鍖呮嫭锛氭簮鐮併€並eil 宸ョ▼銆乻catter銆乁I銆佸瓧浣撱€佹瀯寤烘柟寮忋€佹枃浠舵竻鐞嗐€佷笟鍔℃祦绋嬨€佺‖浠堕獙璇佺粨璁虹瓑銆?- 鏇存柊鍐呭搴旇鏄庯細鏀逛簡浠€涔堛€佷负浠€涔堟敼銆佹€庝箞楠岃瘉銆佽繕闇€瑕佸摢浜涙澘涓婇獙璇併€?- 鍚庣画鎺ュ姏鍓嶄篃瑕侀槄璇诲苟閬靛畧锛?  - E:\H7R3\鑷姪缁撹处\AGENTS.md
  - E:\H7R3\鑷姪缁撹处\.agents\skills\embedded-project\SKILL.md

杩欐槸 STM32 / HAL / LVGL / BSP 鍥轰欢椤圭洰锛屽悗缁慨鏀瑰繀椤讳繚瀹堛€佸皬姝ャ€佸彲楠岃瘉銆?涓嶈闅忔剰鏀瑰惎鍔ㄦ枃浠躲€乻catter銆佹椂閽熼厤缃€佷腑鏂€丏MA銆乧ache 灞炴€с€丩VGL 閰嶇疆鎴栫‖浠跺垵濮嬪寲椤哄簭銆?
2026-05-15 22:37 鏇存柊锛氬疄鐜版敮浠樿秴鏃跺脊绐?
- 鏍规嵁鍥綪6 鏂板鐙珛鈥滄敮浠樿秴鏃垛€濆脊绐椼€?- 璋冩暣 `User\main.c`锛?  - 鏂板瀹?`PAYMENT_TIMEOUT_MS`锛岄粯璁?`30000U`銆?  - 鏂板 `g_payment_start_tick`锛岃繘鍏ユ敮浠橀〉鍜岄噸鏂版壂鐮佹椂閲嶇疆璁℃椂銆?  - `main_payment_process()` 鍦?`PAYMENT_FLOW_SCANNING` 鐘舵€佷笅妫€鏌ヨ秴鏃躲€?  - 瓒呮椂鍚庤緭鍑?`[PAY] payment timeout`锛岃皟鐢?`dcmipp_stop()` 鍋滄鏀粯椤佃繛缁壂鐮侊紝鏄剧ず `checkout_ui_show_payment_timeout_popup()`锛屾祦绋嬭繘鍏?`PAYMENT_FLOW_PAUSED`銆?- 璋冩暣 `User\ui\checkout_ui.h/.c`锛?  - `checkout_ui_page_handles_t` 鏂板 `payment_timeout_popup`銆?  - 鏂板鍐呴儴 `checkout_ui_payment_timeout_popup_handles_t`銆?  - 鏂板鍏紑鎺ュ彛 `checkout_ui_show_payment_timeout_popup()`銆?  - `checkout_ui_hide_all_popups()` 宸插姞鍏ラ殣钘?`payment_timeout_popup`銆?- 寮圭獥鏍峰紡锛?  - 浣跨敤鐜版湁鍏ㄥ睆鍗婇€忔槑榛戣壊 `popup_mask`銆?  - 灞呬腑鐧借壊鍦嗚寮圭獥锛屽昂瀵?`320x230`銆?  - 姗欒壊鍦嗗舰鏃堕挓鍥炬爣銆?  - 鏍囬锛歚鏀粯瓒呮椂`銆?  - 姝ｆ枃锛?    - `鏀粯鏃堕棿宸茶秴鏃禶
    - `璇烽噸鏂版壂鐮佹敮浠榒
  - 鎸夐挳锛歚鍙栨秷鏀粯`銆乣閲嶆柊鎵爜`銆?- 浜や簰锛?  - 鐐瑰嚮鈥滃彇娑堟敮浠樷€濓細鍏抽棴寮圭獥锛岃Е鍙戞敮浠樺彇娑堝洖璋冿紝鎭㈠ `VISION_MODE_PRODUCT`锛岃繑鍥炶繘鍏ユ敮浠橀〉涔嬪墠鐨勯〉闈紝璐墿杞︿笉娓呯┖銆?  - 鐐瑰嚮鈥滈噸鏂版壂鐮佲€濓細鍏抽棴寮圭獥锛屾仮澶嶅浘P1绛夊緟鎵爜鐘舵€侊紝瑙﹀彂鏀粯閲嶆柊鎵爜鍥炶皟锛沗main_payment_rescan()` 浼氳皟鐢?`payment_qr_reset()`銆侀噸鏂板惎鍔ㄨ繛缁壂鐮侊紝骞堕噸鏂板紑濮嬭鏃躲€?- 瀛椾綋锛?  - 宸叉柊澧炩€滄敮浠樿秴鏃躲€佹敮浠樻椂闂村凡瓒呮椂銆佽閲嶆柊鎵爜鏀粯鈥濆埌 `checkout_ui_font_texts.txt`銆?  - 宸查噸鏂拌繍琛?`python tools\generate_checkout_ui_font.py`锛屽綋鍓?unique chars 涓?176銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 杩涘叆鎵爜鏀粯椤靛悗涓嶆壂鏈夋晥鐮侊紝绾?30 绉掑悗寮瑰嚭鏀粯瓒呮椂寮圭獥銆?  - 鐐瑰嚮鈥滃彇娑堟敮浠樷€濊繑鍥炴潵婧愰〉闈紝璐墿杞︿笉娓呯┖銆?  - 鐐瑰嚮鈥滈噸鏂版壂鐮佲€濆悗寮圭獥鍏抽棴锛岄〉闈㈠洖鍒板浘P1锛屼覆鍙ｇ‘璁ら噸鏂板紑濮?QR 鎵弿銆?
2026-05-15 22:29 鏇存柊锛氬疄鐜版棤鏁堜粯娆剧爜寮圭獥

- 鏍规嵁鍥綪5 鏂板鐙珛鈥滄棤鏁堜粯娆剧爜鈥濆脊绐楋紝涓嶅啀澶嶇敤鏅€氳瘑鍒け璐ュ脊绐椼€?- 璋冩暣 `User\ui\checkout_ui.h/.c`锛?  - `checkout_ui_page_handles_t` 鏂板 `payment_invalid_popup`銆?  - 鏂板鍐呴儴 `checkout_ui_payment_invalid_popup_handles_t`銆?  - `checkout_ui_show_payment_invalid_popup()` 鐜板湪鏄剧ず鐙珛鐨勬棤鏁堜粯娆剧爜寮圭獥銆?  - `checkout_ui_hide_all_popups()` 宸插姞鍏ラ殣钘?`payment_invalid_popup`銆?- 寮圭獥鏍峰紡锛?  - 浣跨敤鐜版湁鍏ㄥ睆鍗婇€忔槑榛戣壊 `popup_mask`銆?  - 灞呬腑鐧借壊鍦嗚寮圭獥锛屽昂瀵?`320x230`銆?  - 椤堕儴绾㈣壊鍦嗗舰鍙夊彿銆?  - 鏍囬锛歚鏃犳晥鐨勪粯娆剧爜`銆?  - 姝ｆ枃锛?    - `鏈瘑鍒埌鏈夋晥鐨勪粯娆剧爜`
    - `璇锋鏌ヤ簩缁寸爜鏄惁姝ｇ‘`
  - 鎸夐挳锛歚鎴戠煡閬撲簡`銆?- 浜や簰锛?  - `鎴戠煡閬撲簡` 浜嬩欢涓?`checkout_ui_payment_invalid_ok_event_cb()`銆?  - 鐐瑰嚮鍚庡叧闂脊绐椼€?  - 淇濇寔鍦ㄦ壂鐮佹敮浠橀〉锛屼笉杩斿洖棣栭〉銆?  - 璋冪敤 `checkout_ui_set_payment_scanning()` 鎭㈠鍥綪1绛夊緟鎵爜鐘舵€併€?  - 閫氳繃鏀粯椤甸噸鏂版壂鐮佸洖璋冭Е鍙?`main_payment_rescan()`锛屽唴閮ㄤ細璋冪敤 `payment_qr_reset()` 骞堕噸鏂板惎鍔ㄦ敮浠?QR 杩炵画鎵弿銆?  - 涓嶆竻绌鸿喘鐗╄溅銆?- 瑙﹀彂鏉′欢浠嶅湪 `User\main.c` 鐨?`main_payment_process()`锛?  - `payment_qr_process_current_frame()` 杩斿洖 `PAYMENT_QR_INVALID` 鏃讹紝璋冪敤 `checkout_ui_set_payment_invalid()` 鍜?`checkout_ui_show_payment_invalid_popup()`銆?- 瀛椾綋锛?  - 宸叉柊澧炩€滄棤鏁堢殑浠樻鐮併€佹湭璇嗗埆鍒版湁鏁堢殑浠樻鐮併€佽妫€鏌ヤ簩缁寸爜鏄惁姝ｇ‘銆佹垜鐭ラ亾浜嗏€濆埌 `checkout_ui_font_texts.txt`銆?  - 宸查噸鏂拌繍琛?`python tools\generate_checkout_ui_font.py`锛屽綋鍓?unique chars 涓?175銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 鎵潪 `PAY_OK_SELF_CHECKOUT_001` 鐨勪簩缁寸爜鏃跺脊鍑哄浘P5椋庢牸寮圭獥銆?  - 鐐瑰嚮鈥滄垜鐭ラ亾浜嗏€濆悗鐣欏湪鎵爜鏀粯椤靛苟鎭㈠鍥綪1銆?  - 涓插彛纭閲嶆柊鎵爜鏃?`payment_qr_reset()` 鐢熸晥锛屽悗缁粛鍙瘑鍒纭粯娆剧爜銆?
2026-05-15 22:23 鏇存柊锛歱ayment_qr 鎺ュ叆鎵爜鏀粯椤?
- 鏍规嵁瑕佹眰锛屽皢 `payment_qr` 妯″潡鎺ュ叆鎵爜鏀粯椤碉紝瀹炵幇鍥哄畾浜岀淮鐮?`PAY_OK_SELF_CHECKOUT_001` 鐨勬ā鎷熸敮浠樿瘑鍒€?- 鏂板瑙嗚妯″紡锛屼綅缃湪 `User\main.c`锛?  - `VISION_MODE_PRODUCT`锛氬晢鍝佽瘑鍒ā寮忋€?  - `VISION_MODE_PAYMENT_QR`锛氭敮浠樹簩缁寸爜璇嗗埆妯″紡銆?- 鏂板鏀粯娴佺▼鐘舵€侊紝浣嶇疆鍦?`User\main.c`锛?  - `PAYMENT_FLOW_IDLE`
  - `PAYMENT_FLOW_SCANNING`
  - `PAYMENT_FLOW_PAUSED`
  - `PAYMENT_FLOW_SUCCESS_PENDING`
  - `PAYMENT_FLOW_DONE`
- UI 鍥炶皟鎺ュ叆锛?  - `User\ui\checkout_ui.h/.c` 鏂板 `checkout_ui_set_payment_action_callbacks(start_cb, cancel_cb, rescan_cb, user_data)`銆?  - 杩涘叆鎵爜鏀粯椤靛悗瑙﹀彂 `main_payment_start()`銆?  - 杩斿洖/鍙栨秷鏀粯瑙﹀彂 `main_payment_cancel()`銆?  - 閲嶆柊鎵爜瑙﹀彂 `main_payment_rescan()`銆?- 杩涘叆鎵爜鏀粯椤垫椂锛?  - 涓荤嚎浠?`VISION_MODE_PRODUCT` 鍒囧埌 `VISION_MODE_PAYMENT_QR`銆?  - 鏆傚仠鏅€氬晢鍝佽瘑鍒姸鎬佹満锛屼笉鍐嶈皟鐢?`camera_workflow_process()`銆?  - 璋冪敤 `payment_qr_reset()`銆?  - 璋冪敤 `payment_qr_init()`锛屼笉閲嶅鍒濆鍖栨憚鍍忓ご锛屼笉鍦ㄦ瘡娆¤瘑鍒悗 destroy銆?  - 璋冪敤 `dcmipp_start_continuous()` 淇濇寔 DCMIPP 杩炵画浼犺緭銆?  - 椤甸潰鏄剧ず鍥綪1鐘舵€侊細`checkout_ui_set_payment_scanning()`銆?- 鏀粯鎵爜杩囩▼涓細
  - 涓诲惊鐜湪 `VISION_MODE_PAYMENT_QR` 涓嬭皟鐢?`main_payment_process()`锛屼笉杩愯鍟嗗搧璇嗗埆銆?  - 浣跨敤 `dcmipp_wait_frame(g_payment_last_frame_mark, 20ms)` 绛夊緟瀹屾暣鏂板抚銆?  - 鏂板抚鍒版潵鍚?`dcmipp_invalidate_frame_cache()`銆?  - 寮€濮嬪鐞嗗抚鍓嶈皟鐢?`checkout_ui_set_payment_recognizing()`锛屾樉绀哄浘P2銆?  - 璋冪敤浣嶇疆锛歚main_payment_process()` 鍐呰皟鐢?`payment_qr_process_current_frame(state->frame_width, state->frame_height)`銆?  - `payment_qr_process_current_frame()` 浠庡綋鍓?`camera_date_buf` 瑁佸壀澶氭。涓績 ROI锛孯GB565 杞伆搴﹀悗璋冪敤 ATKQR锛涗笉浼氭妸 RGB565 鐩存帴閫佸叆 `atk_qr_decode()`銆?- UI 鐘舵€佽仈鍔細
  - `PAYMENT_QR_VALID`锛氱疮璁¤繛缁垚鍔熸鏁般€?  - `PAYMENT_QR_INVALID`锛氳皟鐢?`checkout_ui_set_payment_invalid()` 骞跺脊鍑?`checkout_ui_show_payment_invalid_popup()`锛屾彁绀轰粯娆剧爜鏃犳晥锛涙祦绋嬫殏鍋滐紝绛夊緟鐢ㄦ埛閲嶆柊鎵爜銆?  - `PAYMENT_QR_RECO_ERR` / 鏈瘑鍒細涓嶅垽澶辫触锛屽洖鍒版垨淇濇寔鍥綪1绛夊緟鎵爜鐘舵€侊紝缁х画鎵弿鍚庣画甯с€?- 鏀粯鎴愬姛鏉′欢锛?  - 褰撳墠瑕佹眰杩炵画璇嗗埆鍒扮洰鏍囦簩缁寸爜 2 娆★細`PAYMENT_QR_SUCCESS_CONFIRM_COUNT = 2U`銆?  - 杩炵画 2 娆℃垚鍔熷悗鍋滄杩炵画 DCMIPP锛岃皟鐢?`checkout_ui_set_payment_success()` 鏄剧ず鍥綪3銆?  - 寤舵椂 500ms锛歚PAYMENT_QR_SUCCESS_DELAY_MS = 500U`銆?  - 寤舵椂鍚庤皟鐢?`checkout_ui_show_checkout_done_popup(total_price)`锛岀敤鎴风偣纭畾鍚庤蛋鍘?`main_complete_checkout()` 娓呯┖璐墿杞﹀苟鎭㈠涓婚〉闈€?- 绂诲紑鎵爜鏀粯椤碉細
  - 鍙栨秷鏀粯銆佽繑鍥炪€佹敮浠樻垚鍔熷脊绐楃‘瀹氶兘浼氭仮澶?`VISION_MODE_PRODUCT`銆?  - 璋冪敤 `dcmipp_stop()` 鍋滄鏀粯椤佃繛缁紶杈撱€?  - 鍙栨秷鏀粯涓嶆竻绌鸿喘鐗╄溅锛屼笉鏀瑰彉鍟嗗搧璇嗗埆缁撴灉銆?  - 鍙湁鏀粯鎴愬姛鍚庣偣缁撹处瀹屾垚寮圭獥纭畾锛屾墠璋冪敤 `camera_workflow_complete_checkout()` 娓呯┖璐墿杞︺€?- 瀛椾綋锛?  - 宸叉柊澧炩€滀粯娆剧爜鏃犳晥鈥濆埌 `checkout_ui_font_texts.txt`銆?  - 宸查噸鏂拌繍琛?`python tools\generate_checkout_ui_font.py`锛屽綋鍓?unique chars 涓?169銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 闈炵┖璐墿杞︾偣鍑荤粨璐﹁繘鍏ユ壂鐮佹敮浠橀〉鍚庯紝涓插彛杈撳嚭 `[PAY] enter payment QR mode`銆?  - 鏀粯椤垫壂鐮佹椂鍟嗗搧璇嗗埆涓嶅簲缁х画瑙﹀彂鎷嶇収/AI 鍒嗙被銆?  - 鍥哄畾浜岀淮鐮?`PAY_OK_SELF_CHECKOUT_001` 杩炵画璇嗗埆涓ゆ鍚庤繘鍏ュ浘P3锛屽啀寤舵椂杩涘叆缁撹处瀹屾垚寮圭獥銆?  - 闈炵洰鏍囦簩缁寸爜搴斿脊鍑轰粯娆剧爜鏃犳晥鎻愮ず锛岀偣鍑荤‘瀹氬悗鍙寜鈥滈噸鏂版壂鐮佲€濈户缁€?  - 鍙栨秷鏀粯杩斿洖鏉ユ簮椤甸潰锛岃喘鐗╄溅鍜岃瘑鍒粨鏋滀笉鍙樸€?
2026-05-13 21:50 鏇存柊锛氫富绾垮垏鍥炲晢鍝佽瘑鍒?
- 鏍规嵁瑕佹眰鈥滀富绾垮垏鍥炲晢鍝佽瘑鍒€濓紝鍏抽棴浜岀淮鐮佺嫭绔嬫祴璇曟ā寮忋€?- 璋冩暣 `User\app\payment_qr.h`锛?  - `PAYMENT_QR_TEST_MODE` 榛樿鍊间粠 `1U` 鏀逛负 `0U`銆?- 褰撳墠鍚姩琛屼负锛?  - `User\main.c` 涓嶅啀璋冪敤 `payment_qr_test_start()` / `payment_qr_test_loop()`銆?  - 涓诲惊鐜仮澶嶆墽琛?`camera_workflow_process(&camera_state)` 鍜?`main_refresh_checkout_ui(&camera_state)`銆?  - `payment_qr` 妯″潡銆佽繛缁抚娴嬭瘯鍑芥暟鍜屾壂鐮佹敮浠橀〉 UI 淇濈暀锛屽悗缁渶瑕佹椂鍙噸鏂版墦寮€鎴栨帴鍏ユ寮忔敮浠樻祦绋嬨€?- 鏈涓嶆敼鍟嗗搧璇嗗埆鐘舵€佹満銆佷笉鏀硅喘鐗╄溅搴曞眰閫昏緫銆佷笉鏀规壂鐮佹敮浠橀〉 UI銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 鍚姩鍚庝笉鍐嶈緭鍑?`QR test mode start`銆?  - 鏀剧疆鍟嗗搧鍚庝粛鎸夌ǔ瀹氶噸閲忚Е鍙戞媿鐓с€丄I 鍟嗗搧璇嗗埆鍜屽姞鍏ヨ喘鐗╄溅銆?  - 鐐瑰嚮缁撹处浠嶈繘鍏ユ壂鐮佹敮浠橀〉 UI锛屼絾鏆備笉鎺ョ湡瀹?`payment_qr` 鏀粯璇嗗埆銆?
2026-05-13 18:08 鏇存柊锛氱粨璐︽寜閽繘鍏ユ壂鐮佹敮浠橀〉

- 璋冩暣 `User\ui\checkout_ui.c` 鐨勭粨璐﹀叆鍙ｉ€昏緫锛屼富璇嗗埆缁撹处椤碘€滅粨璐︹€濆拰璐墿杞︾鐞嗛〉鈥滃幓缁撹处鈥濅粛鍏辩敤 `checkout_ui_request_checkout()`銆?- 褰撳墠鎸夐挳閫昏緫锛?  - 鑻ヨ喘鐗╄溅涓虹┖锛岃皟鐢ㄥ唴閮?`checkout_ui_show_empty_checkout_popup()`锛屾彁绀衡€滆喘鐗╄溅涓虹┖锛屾棤娉曠粨璐︹€濄€?  - 鑻ヨ喘鐗╄溅闈炵┖锛岃皟鐢?`checkout_ui_show_payment_page(cart->total_price_cent)` 杩涘叆鎵爜鏀粯椤碉紝涓嶅啀鐩存帴璋冪敤缁撹处瀹屾垚寮圭獥銆?- 鏀粯椤靛叆鍙ｏ細
  - `checkout_ui_show_payment_page(total_price_cent)` 浼氭樉绀哄綋鍓嶆€讳环銆?  - 姣忔杩涘叆鏀粯椤甸€掑鐢熸垚璁㈠崟鍙凤紝褰撳墠鏍煎紡涓?`20260513-001`銆乣20260513-002` 绛夈€?  - 杩涘叆鍚庤嚜鍔ㄨ皟鐢?`checkout_ui_set_payment_scanning()`锛岄〉闈㈡仮澶嶅浘P1鈥滅瓑寰呮壂鐮佲€濈姸鎬併€?  - 杩涘叆鏀粯椤靛墠浼氳褰曟潵婧愰〉闈細涓昏瘑鍒〉鎴栬喘鐗╄溅椤点€?- 鍙栨秷鏀粯閫昏緫锛?  - 椤堕儴鈥滆繑鍥炩€濄€侀《閮ㄢ€滃彇娑堚€濄€佸簳閮ㄢ€滃彇娑堟敮浠樷€濋兘璋冪敤鍚屼竴杩斿洖浜嬩欢銆?  - 杩斿洖杩涘叆鏀粯椤典箣鍓嶇殑椤甸潰銆?  - 涓嶆竻绌鸿喘鐗╄溅锛屼笉鏀瑰彉鍟嗗搧璇嗗埆缁撴灉銆?- 閲嶆柊鎵爜閫昏緫锛?  - 搴曢儴鈥滈噸鏂版壂鐮佲€濆彧璋冪敤 `checkout_ui_set_payment_scanning()` 鎭㈠鍥綪1鐘舵€併€?  - 鏆傛椂涓嶆帴鐪熷疄 `payment_qr` 璇嗗埆銆?- 鍘?`checkout_ui_show_checkout_done_popup(total_price_cent)` 淇濈暀锛屽悗缁敮浠樼‘璁ゆ垚鍔熷悗浠嶅彲澶嶇敤锛涘綋鍓嶇粨璐︽寜閽笉鐩存帴璋冪敤瀹冦€?- 瀛椾綋锛?  - 宸叉柊澧炩€滆喘鐗╄溅涓虹┖锛屾棤娉曠粨璐︹€濆埌 `checkout_ui_font_texts.txt`銆?  - 宸查噸鏂拌繍琛?`python tools\generate_checkout_ui_font.py`锛屽綋鍓?unique chars 涓?169銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 绌鸿喘鐗╄溅鏃朵富椤甸潰鈥滅粨璐︹€濆拰璐墿杞﹂〉鈥滃幓缁撹处鈥濋兘鏄剧ず鈥滆喘鐗╄溅涓虹┖锛屾棤娉曠粨璐︹€濄€?  - 闈炵┖璐墿杞︽椂涓ゅ鎸夐挳閮借繘鍏ユ壂鐮佹敮浠橀〉锛屽苟鏄剧ず姝ｇ‘鎬讳环銆?  - 浠庝富椤甸潰杩涘叆鏀粯椤靛悗鍙栨秷杩斿洖涓婚〉闈紱浠庤喘鐗╄溅椤佃繘鍏ユ敮浠橀〉鍚庡彇娑堣繑鍥炶喘鐗╄溅椤点€?  - 鍙栨秷鏀粯鍚庤喘鐗╄溅鍐呭鍜屾湰娆¤瘑鍒粨鏋滀笉鍙樸€?
2026-05-13 18:06 鏇存柊锛氭柊澧炴壂鐮佹敮浠橀〉 UI

- 鏍规嵁鍥綪1/P2/P3 鏂板 480x800 绔栧睆鎵爜鏀粯椤?UI锛屾湰闃舵鍙仛 UI锛屼笉鎺?`payment_qr`锛屼笉鏀瑰晢鍝佽瘑鍒拰璐墿杞﹀簳灞傞€昏緫銆?- 璋冩暣 `User\ui\checkout_ui.h/.c`锛?  - `checkout_ui_page_handles_t` 鏂板 `payment_page`銆?  - 鏂板鍏紑鎺ュ彛锛?    - `checkout_ui_show_payment_page(total_price_cent)`
    - `checkout_ui_set_payment_scanning()`
    - `checkout_ui_set_payment_recognizing()`
    - `checkout_ui_set_payment_success()`
    - `checkout_ui_set_payment_invalid()`
    - `checkout_ui_update_payment_amount(total_price_cent)`
    - `checkout_ui_update_order_no(order_no)`
  - 鏂板鍐呴儴鎵爜鏀粯椤靛彞鏌勫拰鐘舵€佹灇涓撅紝鏀寔鎵爜涓€佽瘑鍒腑銆佽瘑鍒垚鍔熴€佹棤鏁堜粯娆剧爜 4 绉?UI 鐘舵€併€?- 椤甸潰甯冨眬锛?  - 椤堕儴鏍囬鏍?`0,0,480,60`锛岃儗鏅?`#144B5C`锛屽乏渚р€滆繑鍥炩€濄€佷腑闂粹€滄壂鐮佹敮浠樷€濄€佸彸渚р€滃彇娑堚€濄€?  - 閲戦璁㈠崟鍗＄墖 `15,75,450,95`锛屾樉绀衡€滃簲浠橀噾棰?锟x.xx鈥濆拰鈥滆鍗曞彿/20260506-001鈥濄€?  - 鎽勫儚澶存壂鐮佸尯鍩?`15,185,450,330`锛屾繁鑹茶儗鏅紝涓績 `260x260` 鎵爜妗嗭紱褰撳墠鐢?UI 鍗犱綅浜岀淮鐮佸潡妯℃嫙鎵爜鍖哄煙锛屼笉鎺ョ湡瀹炴憚鍍忓ご鐢婚潰銆?  - 鎻愮ず璇存槑鍗＄墖 `15,530,450,70`锛岃儗鏅?`#EAF4FF`锛岃竟妗?`#90CAF9`銆?  - 鎵爜鐘舵€佸崱鐗?`15,615,450,80`銆?  - 搴曢儴鎸夐挳锛歚鍙栨秷鏀粯` 鍜?`閲嶆柊鎵爜`銆?- 鐘舵€佽涓猴細
  - `checkout_ui_set_payment_scanning()`锛氳摑鑹插洓瑙掓锛岃摑鑹叉壂鎻忕嚎锛岀姸鎬佲€滅瓑寰呮壂鐮?..鈥濄€?  - `checkout_ui_set_payment_recognizing()`锛氱豢鑹叉壂鎻忕嚎锛岀姸鎬佲€滄鍦ㄨ瘑鍒簩缁寸爜...鈥濄€?  - `checkout_ui_set_payment_success()`锛氱豢鑹插洓瑙掓锛屾樉绀烘垚鍔熷渾鏍囷紝鐘舵€佲€滀簩缁寸爜璇嗗埆鎴愬姛 / 姝ｅ湪纭鏀粯...鈥濓紝搴曢儴鎸夐挳缃伆銆?  - `checkout_ui_set_payment_invalid()`锛氱孩鑹插洓瑙掓鍜岀姸鎬佲€滄湭璇嗗埆鍒版湁鏁堜粯娆剧爜鈥濄€?- 鎸夐挳閫昏緫锛?  - 椤堕儴杩斿洖銆侀《閮ㄥ彇娑堛€佸簳閮ㄥ彇娑堟敮浠樺潎杩斿洖杩涘叆鏀粯椤靛墠鐨勯〉闈紝涓嶆竻绌鸿喘鐗╄溅銆?  - 搴曢儴閲嶆柊鎵爜鍙皟鐢?`checkout_ui_set_payment_scanning()` 鎭㈠鍥綪1鐘舵€併€?  - 鐜版湁 UI 灞傗€滅粨璐?鍘荤粨璐︹€濇寜閽幇鍦ㄨ繘鍏?`checkout_ui_show_payment_page(cart->total_price_cent)`锛屼粛涓嶈皟鐢?`payment_qr`锛屼笉纭鏀粯锛屼笉娓呯┖璐墿杞︺€?- 瀛椾綋锛?  - 宸叉妸鈥滄壂鐮佹敮浠樸€佸簲浠橀噾棰濄€佽鍗曞彿銆佽灏嗕粯娆剧爜瀵瑰噯妗嗗唴銆佹鍦ㄨ瘑鍒簩缁寸爜銆佷簩缁寸爜璇嗗埆鎴愬姛銆佸彇娑堟敮浠樸€侀噸鏂版壂鐮佲€濈瓑鏂版枃妗堝姞鍏?`Middlewares\LVGL\Font\checkout_ui_font_texts.txt`銆?  - 宸查噸鏂拌繍琛?`python tools\generate_checkout_ui_font.py` 鐢熸垚 `Font20.c/.h`锛屽綋鍓?unique chars 涓?167銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 浠庝富椤甸潰/璐墿杞﹂〉杩涘叆鎵爜鏀粯椤靛悗锛岄噾棰濄€佽鍗曞彿銆佹爣棰樻爮銆佹壂鐮佹銆佺姸鎬佸崱鐗囧拰搴曢儴鎸夐挳鏄惁涓庡浘P1鎺ヨ繎銆?  - 璋冪敤 4 涓姸鎬佹帴鍙ｆ椂鏄惁鍒嗗埆鎺ヨ繎鍥綪1/P2/P3/鏃犳晥鐘舵€併€?  - 杩斿洖/鍙栨秷鏄惁鍥炲埌杩涘叆鍓嶉〉闈笖璐墿杞︿笉娓呯┖銆?  - 鏂板涓枃鏂囨鍦?Font20 涓嬫槸鍚︾己瀛楁垨閿欎綅銆?
2026-05-13 17:53 鏇存柊锛歱ayment_qr 澧炲姞宸﹀彸闀滃儚绾犳灏濊瘯

- 閽堝鏉夸笂鍙嶉鈥滃浘鍍忔槸宸﹀彸闀滃儚鐨勨€濓紝浜岀淮鐮佽В鐮佽緭鍏ュ鍔犳按骞抽暅鍍忕籂姝ｅ皾璇曘€?- 璋冩暣 `User\app\payment_qr.c`锛?  - 鏂板瀹?`PAYMENT_QR_SCAN_TRY_HMIRROR`锛岄粯璁?`1U`銆?  - `payment_qr_rgb565_to_gray_roi()` 澧炲姞 `hmirror` 鍙傛暟銆?  - `payment_qr_process_current_frame()` 瀵规瘡涓腑蹇?ROI 鍏堝皾璇?`mode=normal`锛屽啀灏濊瘯 `mode=hmirror`銆?  - `mode=hmirror` 鍙奖鍝嶉€佺粰 ATKQR 鐨勭伆搴?ROI锛屼笉鏀瑰彉灞忓箷棰勮銆佷笉鏀?OV2640 鍒濆鍖栥€佷笉鏀瑰晢鍝佽瘑鍒祦绋嬨€?  - 鑻ラ暅鍍忕籂姝ｈ矾寰勮瘑鍒垚鍔燂紝涓插彛浼氳緭鍑?`QR detected mode=hmirror`銆?- 鏈涓嶆帴鎵爜鏀粯椤甸潰銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓婃澘楠岃瘉锛?  - 涓嬭浇鍚庤瀵熶覆鍙ｆ槸鍚﹀嚭鐜?`mode=hmirror`銆?  - 鑻ュ嚭鐜?`QR detected mode=hmirror` 鍜?`PAYMENT QR VALID`锛岃鏄庡綋鍓嶅乏鍙抽暅鍍忔槸涓昏褰卞搷鍥犵礌銆?  - 鑻ヤ粛 error3锛屽啀缁х画鎸変簩缁寸爜灞呬腑銆佸ぇ灏忋€佹洕鍏夈€佺劍璺濆拰鐏板害 ROI 鏂瑰悜鎺掓煡銆?
2026-05-13 17:41 鏇存柊锛歱ayment_qr 鐙珛杩炵画甯ф祴璇曟ā寮?
- 鏍规嵁鏈樁娈甸渶姹傦紝娣诲姞/璋冩暣 `PAYMENT_QR_TEST_MODE` 涓嬬殑鍥哄畾浜岀淮鐮佽瘑鍒祴璇曟祦绋嬶紝鐩爣浜岀淮鐮佸唴瀹逛粛涓?`PAY_OK_SELF_CHECKOUT_001`銆?- 鏈涓嶆帴鎵爜鏀粯椤甸潰銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉鏀瑰彉璐墿杞︿笟鍔℃祦绋嬨€?- 娴嬭瘯鍏ュ彛锛?  - `User\main.c` 宸插湪 `PAYMENT_QR_TEST_MODE != 0U` 鏃惰皟鐢?`payment_qr_test_start(&camera_state)`銆?  - 涓诲惊鐜腑璋冪敤 `payment_qr_test_loop(&camera_state)`锛屼笉杩愯鏅€氬晢鍝佽瘑鍒姸鎬佹満銆?  - `PAYMENT_QR_TEST_MODE` 褰撳墠瀹氫箟鍦?`User\app\payment_qr.h`锛岄粯璁や负 `1U`锛涙敼涓?`0U` 鍙叧闂祴璇曟ā寮忓苟鎭㈠鏅€氳嚜鍔╃粨璐︽祦绋嬨€?- 璋冩暣 DCMIPP锛?  - `Drivers\BSP\DCMIPP\dcmipp.c/.h` 鏂板 `dcmipp_start_continuous()`锛屼娇鐢?`DCMIPP_MODE_CONTINUOUS` 鍚姩杩炵画閲囬泦銆?  - 淇濈暀鍘?`dcmipp_start()` 鍗曞抚 snapshot 鍏ュ彛锛屼緵鍟嗗搧璇嗗埆娴佺▼缁х画浣跨敤銆?  - `dcmipp_invalidate_frame_cache()` 浠嶅彧澶辨晥鎽勫儚澶村抚缂撳啿鑼冨洿锛屼笉鍐嶅叏灞€ invalidate cache銆?- 璋冩暣 `User\app\payment_qr.c`锛?  - `payment_qr_process_current_frame(frame_width, frame_height)` 鏀逛负澶勭悊褰撳墠宸插畬鎴愮殑 `camera_date_buf` 甯э紝涓嶅啀鍐呴儴璋冪敤 `dcmipp_start()`銆?  - 浠嶄粠涓績澶氭。 ROI 鎵弿锛氭渶澶?`320x320`銆佹渶灏?`120x120`銆佹杩?`40`銆?  - 璇嗗埆鎴愬姛鍚庝笉 destroy锛宍payment_qr_init()` 鍚庣画閲嶅璋冪敤鍙繑鍥?ready銆?  - 鏃ュ織琛ュ厖 `QR decode ret = x`銆乣QR detected: ...`銆乣PAYMENT QR VALID`銆乣QR invalid: ...`銆乣QR decode error 3`銆?  - error3 鏃舵彁绀烘鏌ヤ簩缁寸爜鏄惁瀹屾暣澶勪簬 ROI銆佷簩缁寸爜瀹藉害鏄惁绾﹀崰 ROI 鐨?1/3 鍒?2/3銆佹槸鍚﹁繃鏇?澶辩劍銆丷OI 鏄惁杩囧ぇ鎴栬繃灏忋€佹槸鍚﹂渶瑕佷复鏃舵樉绀虹伆搴?ROI銆?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 涓嶅啀浣跨敤 KEY1 瑙﹀彂娴嬭瘯銆?  - 鍚姩鏃跺垵濮嬪寲 `payment_qr`锛岄殢鍚庤皟鐢?`dcmipp_start_continuous()` 淇濇寔鎽勫儚澶翠紶杈撱€?  - 寰幆涓€氳繃 `dcmipp_wait_frame(g_ov_frame mark, 1000ms)` 绛夊緟瀹屾暣鏂板抚銆?  - 姣忓抚瀹屾垚鍚?`dcmipp_invalidate_frame_cache()`锛屽埛鏂?`checkout_ui` 涓婚〉闈㈠浘鍍忓尯鍩熼瑙堬紝鍐嶈皟鐢?`payment_qr_process_current_frame()` 杩炵画鎵弿澶氬抚銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓婃澘娴嬭瘯鍥綪7锛?  - 涓嬭浇褰撳墠 `camrun_fast`锛涘鏋?fast 璇嗗埆寮傚父锛屽厛鐢?full camrun 鍏ㄩ噺鐑у綍涓€娆＄‘璁ゅ閮?AI 鏉冮噸/Flash 鐘舵€佷笉褰卞搷鏁翠綋鍥轰欢銆?  - 鎵撳紑涓插彛 115200锛屽惎鍔ㄥ悗搴旂湅鍒?`QR test mode start`銆乣QR init ok`銆乣QR scanning...`銆?  - 灏嗗唴瀹逛负 `PAY_OK_SELF_CHECKOUT_001` 鐨勫浘P7浜岀淮鐮佹斁鍦ㄦ憚鍍忓ご鐢婚潰涓績锛屼簩缁寸爜瀹藉害浼樺厛璋冩暣鍒颁腑蹇冭瘑鍒绾?1/3 鍒?2/3銆?  - 鎴愬姛鏃朵覆鍙ｅ簲鍑虹幇 `QR detected: PAY_OK_SELF_CHECKOUT_001` 鍜?`PAYMENT QR VALID`銆?  - 鑻ュ嚭鐜?`QR invalid: xxxx`锛岃鏄庤瘑鍒埌浜嗕簩缁寸爜浣嗗唴瀹逛笉鏄洰鏍囧瓧绗︿覆銆?  - 鑻ユ寔缁?`QR decode error 3`锛屼紭鍏堣皟浜岀淮鐮佸眳涓€佸昂瀵搞€佺劍璺濄€佹洕鍏?鍙嶅厜锛屽啀鑰冭檻涓存椂鏄剧ず鐏板害 ROI 鎴栫户缁皟鏁翠腑蹇?ROI 灏哄銆?- 浠嶉渶鏉夸笂楠岃瘉锛?  - 杩炵画閲囬泦棰勮鏄惁绋冲畾锛屼笉搴斿洜 QR 瑙ｇ爜瀵艰嚧鎽勫儚澶村仠浣忋€?  - 鎴愬姛璇嗗埆鍚庝笉 destroy锛屽悗缁抚浠嶅彲缁х画鎵弿銆?  - error3 鏃ュ織涓嶅簲瀵艰嚧涓诲惊鐜崱姝汇€?
2026-05-13 17:25 鏇存柊锛氭寜瀹為獙38灏佽 payment_qr 浜岀淮鐮佽瘑鍒ā鍧?
- 鏍规嵁宸茶窇閫氱殑 `E:\H7R3\瀹為獙38 鎽勫儚澶村疄楠宍 浜岀淮鐮佽瘑鍒祦绋嬶紝鍙皝瑁呬簩缁寸爜璇嗗埆妯″潡锛屼笉鎺ユ壂鐮佹敮浠?UI銆佷笉鏀圭粨璐︽寜閽€昏緫銆佷笉鏀瑰彉鍟嗗搧璇嗗埆鐘舵€佹満銆?- 鏂板/璋冩暣 ATKQR 鏂囦欢浣嶇疆锛?  - 浠庡疄楠?8澶嶅埗 `Drivers\BSP\ATKQR\atk_qrdecode.c`
  - 浠庡疄楠?8澶嶅埗 `Drivers\BSP\ATKQR\atk_qrdecode.h`
  - 浠庡疄楠?8澶嶅埗 `Drivers\BSP\ATKQR\ATK_QR_V1.4.lib`
  - 涓や釜 Keil 宸ョ▼鏀逛负缂栬瘧/閾炬帴 `Drivers\BSP\ATKQR` 涓嬬殑閫傞厤灞傚拰搴撴枃浠讹紝涓嶅啀浠庡伐绋嬫枃浠朵腑寮曠敤 `Middlewares\ATKQR\Lib\ATK_QR_V1.4.lib`銆?- 璋冩暣 `User\app\payment_qr.c/.h`锛?  - 澧炲姞姝ｅ紡鎺ュ彛 `payment_qr_process_current_frame(frame_width, frame_height)`銆?  - 淇濈暀 `payment_qr_process_frame(gray_image, width, height)` 浣滀负鐏板害鍥惧吋瀹瑰叆鍙ｏ紝渚涚幇鏈夋祴璇曚唬鐮佺户缁娇鐢ㄣ€?  - `payment_qr_init()` 鍙湪棣栨璋冪敤鏃舵墽琛?`atk_qr_init()`锛屽悗缁噸澶嶈皟鐢ㄧ洿鎺ヨ繑鍥?ready锛屼笉鍦ㄦ瘡娆℃壂鐮佸悗 destroy銆?  - 鍙傝€冨疄楠?8锛岀伆搴︾紦鍐蹭娇鐢?`camera_date_buf + 1024 * 1024`锛屾渶澶?`512 * 1024` 瀛楄妭銆?  - `payment_qr_process_current_frame()` 璋冪敤鐜版湁 `dcmipp_start()` 鑾峰彇瀹屾暣 RGB565 蹇収锛屽啀浠?`camera_date_buf` 涓績瑁佸壀澶氭。 ROI锛氭渶澶?`320x320`锛屾渶灏?`120x120`锛屾杩?`40`銆?  - RGB565 杞?8bit 鐏板害鍚庡仛 min/max 瀵规瘮搴︽媺浼革紝鍐嶈皟鐢?`atk_qr_decode()`銆?  - 璇嗗埆缁撴灉杩囨护鎴愬彲鎵撳嵃 ASCII锛岀洰鏍囧瓧绗︿覆鍥哄畾涓?`PAY_OK_SELF_CHECKOUT_001`銆?  - 鐘舵€佸鍔?`PAYMENT_QR_RECO_ERR`锛宔rror3 鏃ュ織鏄庣‘鎻愮ず浼樺厛妫€鏌?ROI銆佺劍璺濄€佹洕鍏夊拰浜岀淮鐮佸昂瀵搞€?- 褰撳墠浠嶆湭鎶婃敮浠樹簩缁寸爜娴佺▼鎺ュ叆缁撹处 UI锛涘悗缁簲鍦ㄦ敮浠樻ā寮忎笅鐙崰鎽勫儚澶达紝閬垮厤鍟嗗搧璇嗗埆鍜屼簩缁寸爜璇嗗埆鍚屾椂澶勭悊鍚屼竴甯с€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 纭 `payment_qr_init()` 鍙墦鍗颁竴娆″垵濮嬪寲鎴愬姛锛岄噸澶嶆壂鐮佷笉鍙嶅 init/destroy銆?  - 杩炵画澶氭璋冪敤 `payment_qr_process_current_frame()` 鏃讹紝error3 涓嶅簲瀵艰嚧娴佺▼鍗℃锛屽彲缁х画鎵弿鍚庣画甯с€?  - 鐢ㄥ唴瀹逛负 `PAY_OK_SELF_CHECKOUT_001` 鐨勪簩缁寸爜楠岃瘉 `PAYMENT_QR_VALID`锛岀敤鍏朵粬鍐呭楠岃瘉 `PAYMENT_QR_INVALID`銆?  - 鑻ユ寔缁?error3锛屼紭鍏堣皟浜岀淮鐮佸ぇ灏忋€佸眳涓€佺劍璺濄€佹洕鍏夛紝骞剁‘璁や腑蹇?ROI 瑕嗙洊浜岀淮鐮併€?
2026-05-06 21:34 鏇存柊锛氫复鏃跺叧闂?payment_qr LVGL 棰勮鎭㈠绋冲畾鎬?
- 鏉夸笂鍙嶉鈥滄寜澶嶄綅娌″弽搴斾簡鈥濄€傚綋鍓嶄紭鍏堟寜璺戦/澶栬鐘舵€佸紓甯稿鐞嗭紝鍏堟仮澶嶄竴涓洿淇濆畧鐨勬祴璇曠増鏈€?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - `PAYMENT_QR_TEST_LVGL_PREVIEW` 榛樿鍊间粠 `1U` 鏀瑰洖 `0U`銆?  - 鏆傚仠 KEY1 鍚庨€氳繃 LVGL 鏄剧ず鎽勫儚澶村抚銆?  - 淇濈暀 KEY1 鍗曟瑙﹀彂閲囧浘銆佸唴閮?SRAM 鐏板害缂撳啿銆丷OI 杞伆搴﹀拰涓插彛鏃ュ織銆?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 鏉夸笂鎭㈠寤鸿锛?  - 鑻ユ寜 RESET 娌℃湁閲嶆柊鍑虹幇鍚姩 banner锛屽厛鏂數閲嶄笂鐢点€?  - 濡備粛鏃犱覆鍙ｈ緭鍑猴紝鐢?ST-Link 閲嶆柊涓嬭浇褰撳墠鎭㈠鐗堟湰銆?  - 鎭㈠鍚庡厛涓嶈鎵撳紑 LVGL/LCD 棰勮锛岀户缁敤涓插彛纭 KEY1 瑙﹀彂鏄惁绋冲畾銆?
2026-05-06 21:30 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡澧炲姞 LVGL 鎽勫儚澶村抚鏄剧ず

- 鏍规嵁闇€姹傗€滆兘涓嶈兘鍔犱笂鏄剧ず鈥濓紝鎭㈠娴嬭瘯妯″紡涓嬬殑鎽勫儚澶寸敾闈㈡樉绀猴紝浣嗕笉鍐嶄娇鐢ㄦ鍓嶅鑷?BusFault 椋庨櫓杈冮珮鐨勭洿鎺?LCD 鍐欏叆鏂瑰紡銆?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 鏂板瀹?`PAYMENT_QR_TEST_LVGL_PREVIEW`锛岄粯璁?`1U`銆?  - KEY1 瑙﹀彂閲囧浘鎴愬姛鍚庯紝璋冪敤鐜版湁 UI 鎺ュ彛锛?    - `checkout_ui_main_set_image_source(camera_state->frame_width, camera_state->frame_height, rgb565_frame)`
  - 鎽勫儚澶村抚鏄剧ず鍦ㄥ綋鍓?`checkout_ui` 涓婚〉闈㈠浘鍍忓尯鍩熴€?  - 淇濈暀 `PAYMENT_QR_TEST_LCD_PREVIEW` 榛樿 `0U`锛屼笉鍐嶇洿鎺?`lcd_color_fill()` 鍐?LCD 鎬荤嚎銆?- 褰撳墠鏄剧ず琛屼负锛?  - 鍚姩鍚庝笉浼氬疄鏃舵樉绀烘憚鍍忓ご鐢婚潰銆?  - 姣忔鎸?KEY1 閲囧浘鍚庯紝灞忓箷鍥惧儚鍖哄煙鏄剧ず鏈鎷嶅埌鐨?RGB565 甯с€?  - 涓嶇敾 ROI 绾㈡锛汻OI 浣嶇疆浠嶉€氳繃涓插彛鏃ュ織纭锛屼緥濡?`roi=(120,60 240x240)`銆?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 鎸?KEY1 鍚庣‘璁や富椤甸潰鍥惧儚鍖哄煙鍑虹幇鏈鎽勫儚澶寸敾闈€?  - 鑻ユ樉绀烘甯革紝鍐嶆牴鎹覆鍙?ROI 鍧愭爣鎶婁簩缁寸爜灏介噺鏀惧湪鐢婚潰涓績銆?  - 鑻ュ啀娆″嚭鐜?BusFault锛屽垯鍏抽棴 `PAYMENT_QR_TEST_LVGL_PREVIEW`锛岀户缁彧鐢ㄤ覆鍙ｈ皟璇曘€?
2026-05-06 21:29 鏇存柊锛歱ayment_qr KEY1 瑙﹀彂鏀逛负鐩存帴杈规部妫€娴?
- 閽堝鏉夸笂鍙嶉锛?  - 鈥滅敾闈㈡病浜嗏€濓細杩欐槸鍥犱负 `PAYMENT_QR_TEST_LCD_PREVIEW` 宸查粯璁ゅ叧闂紝閬垮厤姝ゅ墠 LCD 棰勮璺緞寮曞彂 BusFault锛涙憚鍍忓ご浠嶅湪鎸?KEY1 鍚庨噰鍥撅紝鍙槸涓嶅啀鏄剧ず瀹炴椂鐢婚潰銆?  - 鈥滄寜涓ゆ鍐嶆寜娌″弽搴斺€濓細娴嬭瘯妯″紡姝ゅ墠浠嶄娇鐢?`key_scan(0U)`锛屽彲鑳藉彈鍏跺唴閮ㄩ噴鏀剧姸鎬佸奖鍝嶃€?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 涓嶅啀浣跨敤 `key_scan()` 浣滀负娴嬭瘯瑙﹀彂銆?  - 鏀逛负鐩存帴璇诲彇 `KEY1` 鐢靛钩锛屽仛鎸変笅杈规部瑙﹀彂銆?  - 鏂板 `PAYMENT_QR_TEST_KEY_DEBOUNCE_MS = 200U`銆?  - 姣忔鏉惧紑鍚庡啀鎸?KEY1锛岄兘浼氳Е鍙戜竴娆￠噰鍥惧拰瑙ｇ爜銆?- 淇濇寔涓嶅彉锛?  - LCD 鎽勫儚澶撮瑙堥粯璁ゅ叧闂€?  - KEY1 瑙﹀彂鍗曟閲囧浘璇嗗埆銆?  - 鐏板害缂撳啿浣嶄簬鍐呴儴 AXI SRAM銆?  - ROI 涓?`240x240`锛岄粯璁ゅ紑鍚伆搴﹀姣斿害鎷変几銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 鍚姩鍚庡睆骞曟病鏈夋憚鍍忓ご棰勮灞炰簬褰撳墠棰勬湡銆?  - 姣忔鐭寜 KEY1 搴旇緭鍑?`QR KEY1 pressed: capture and decode once`銆?  - 鑻ラ渶瑕佹仮澶?LCD 棰勮锛屽繀椤诲崟鐙帓鏌?LCD 鍐欏叆 BusFault 鍚庡啀鎵撳紑 `PAYMENT_QR_TEST_LCD_PREVIEW`銆?
2026-05-06 21:27 鏇存柊锛歱ayment_qr 娴嬭瘯 ROI 璋冩暣涓?240x240 骞跺鍔犵伆搴﹀姣斿害鎷変几

- 鏍规嵁鏉夸笂鏃ュ織锛?  - 鐏板害缂撳啿宸蹭綅浜庡唴閮?AXI SRAM锛歚QR gray buffer @ 24050fe0`
  - KEY1 鍚庝笉鍐?BusFault銆?  - 褰撳墠 `192x192` ROI 杈撳嚭 `gray min=82/96, max=255`锛岃鏄?ROI 鍐呮渶鏆楀儚绱犱笉澶熼粦锛屼簩缁寸爜鍙兘鍋忓皬銆佽繃鏇濄€佽櫄鐒︽垨鏈厖鍒嗚惤鍏?ROI銆?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - `PAYMENT_QR_TEST_ROI_SIZE` 浠?`192U` 璋冩暣涓?`240U`銆?  - 鏂板瀹?`PAYMENT_QR_TEST_CONTRAST_STRETCH`锛岄粯璁?`1U`銆?  - 鍦?RGB565 -> 8 浣嶇伆搴﹀悗锛屽 ROI 鐏板害鍋氱嚎鎬ф媺浼革細
    - 褰撳墠 ROI 鏈€鏆楀€兼槧灏勫埌 0銆?    - 褰撳墠 ROI 鏈€浜€兼槧灏勫埌 255銆?  - 鐩殑锛氭敼鍠勫睆骞曚簩缁寸爜鎷嶆憚鏃堕粦鑹插彂鐏般€佺櫧鑹茶繃浜鑷?ATKQR 璇嗗埆涓嶅埌鐨勯棶棰樸€?- 鏋勫缓楠岃瘉锛?  - full camrun 鍜?camrun_fast 鍧囦负 0 Error(s), 0 Warning(s)銆?  - map 纭鐏板害缂撳啿浠嶄綅浜庡唴閮?AXI SRAM锛?    - `g_payment_qr_test_gray_buffer 0x24050FE0`
    - size `57600` bytes (`0xE100`)
    - `RW_IRAM1 Size: 0x00075C18 / Max: 0x00080000`
- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 鍚姩鍚庣‘璁ょ伆搴︾紦鍐?size 鍙樹负 `57600 bytes`銆?  - 鎸?KEY1 鍚庤瀵?`QR debug` 鏄惁鍙樹负 `roi=(120,60 240x240)`锛屼笖鐏板害鑼冨洿缁忔媺浼稿悗搴旀帴杩?`min=0, max=255`銆?  - 浜岀淮鐮侀渶灏介噺浣嶄簬鐢婚潰涓績锛屽苟鍗?240x240 ROI 鐨勮緝澶ф瘮渚嬨€?  - 鑻ヤ粛 `res=3`锛屼笅涓€姝ヨ€冭檻浜屽€煎寲銆佺缉鏀惧埌鏇撮€傚悎 ATKQR 鐨勫昂瀵革紝鎴栧崟鐙祴璇?`PAYMENT_QR_TEST_GRAY_MODE_RGB565_SWAP`銆?
2026-05-06 21:24 鏇存柊锛歱ayment_qr 鐏板害缂撳啿杩佺Щ鍒板唴閮?AXI SRAM 骞剁缉灏?ROI

- 鏍规嵁鏉夸笂鏃ュ織锛屽湪鍏抽棴 LCD 棰勮鍚庢寜 KEY1 浠嶅嚭鐜帮細
  - `QR debug: mode=rgb565, roi=(80,20 320x320), gray min=0, max=255`
  - `QR try: rgb565`
  - `[FAULT] BusFault`
  - `CFSR=0x00000400`
- 褰撳墠鍒ゆ柇锛氭晠闅滃凡鎺掗櫎 LCD 棰勮璺緞锛屾洿鍙兘涓?ATKQR 璁块棶鐏板害鍥剧紦鍐叉墍鍦ㄥ唴瀛樺尯鍩熸湁鍏炽€備笂涓€鐗堢伆搴︾紦鍐蹭綅浜?HyperRAM 鎽勫儚澶村抚鍚庢柟銆?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 灏嗙伆搴﹀浘缂撳啿浠?HyperRAM 甯у悗鏂规敼涓烘ā鍧楀唴闈欐€佹暟缁勶細
    - `g_payment_qr_test_gray_buffer`
  - 缂撳啿 32 瀛楄妭瀵归綈銆?  - 鍚姩鏃惰緭鍑虹伆搴︾紦鍐插湴鍧€鍜屽ぇ灏忥細
    - `QR gray buffer @ ..., size=... bytes`
  - 涓洪伩鍏嶆尋鐖嗗綋鍓?`RW_IRAM1 0x24000000 0x00080000`锛屽皢娴嬭瘯 ROI 浠?`320x320` 缂╁皬鍒?`192x192`銆?  - 鐏板害缂撳啿澶у皬鍙樹负 `36864` 瀛楄妭銆?- 鏋勫缓楠岃瘉锛?  - 鍒濇灏濊瘯 `320x320` 鍐呴儴闈欐€佺伆搴︾紦鍐叉椂锛岄摼鎺ュけ璐ワ紝鍘熷洜鏄?AXI SRAM 绌洪棿涓嶈冻銆?  - 鏀逛负 `192x192` 鍚庨噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)銆?  - map 纭鐏板害缂撳啿浣嶄簬鍐呴儴 AXI SRAM锛?    - `g_payment_qr_test_gray_buffer 0x24050FE0, size 0x9000`
    - `RW_IRAM1 Size: 0x00070B18 / Max: 0x00080000`
- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 涓嬭浇鏈€鏂扮増鏈悗纭鍚姩鏃ュ織閲岀殑鐏板害缂撳啿鍦板潃鏄惁涓?`0x240xxxxx`銆?  - 鎸?KEY1 鍚庣‘璁ゆ槸鍚︿笉鍐?BusFault銆?  - 鑻ヤ笉鍐?BusFault 浣嗕粛 `res=3`锛岄渶瑕佹妸浜岀淮鐮佹斁寰楁洿澶с€佹洿灞呬腑锛屽洜涓?ROI 宸茬缉灏忎负 `192x192`銆?  - 鑻ヤ粛 BusFault锛屼笅涓€姝ュ簲閲嶇偣妫€鏌?ATKQR 瑙ｇ爜杩囩▼涓姩鎬佸唴瀛樼敵璇蜂綅缃紝鐩墠 `atk_qr_malloc/realloc/free` 浠嶅鐢?LVGL 鍐呭瓨姹犮€?
2026-05-06 21:20 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡榛樿鍏抽棴 LCD 棰勮浠ヨ閬?BusFault

- 鏍规嵁鏉夸笂鏃ュ織锛?  - `QR KEY1 pressed: capture and decode once`
  - `QR debug: mode=rgb565, roi=(80,20 320x320), gray min=0, max=255`
  - `QR try: rgb565`
  - 闅忓悗杩涘叆 `[FAULT] BusFault`
  - `CFSR=0x00000400`
- 褰撳墠鍒ゆ柇锛氭晠闅滃彂鐢熷湪杩涘叆 ATKQR 杩斿洖鏃ュ織涔嬪墠锛屼笖涓婁竴鐗堟柊澧炰簡娴嬭瘯妯″紡涓嬬殑 LCD 鎽勫儚澶村抚棰勮鍜?ROI 绾㈡锛沗CFSR=0x00000400` 灞炰簬鎬荤嚎鏁呴殰鏂瑰悜锛孡CD 鎬荤嚎鍐欏叆鎴栧叾寤惰繜閿欒鏄綋鍓嶄紭鍏堟帓鏌ュ璞°€?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 鏂板瀹?`PAYMENT_QR_TEST_LCD_PREVIEW`銆?  - 榛樿鍊间负 `0U`锛屽嵆鍏抽棴 KEY1 娴嬭瘯璇嗗埆鍓嶇殑 LCD 鎽勫儚澶寸敾闈㈡樉绀哄拰绾㈡缁樺埗銆?  - KEY1 瑙﹀彂銆丏CMIPP 閲囧浘銆佷腑蹇?ROI 鐏板害杞崲銆丄TKQR 瑙ｇ爜鍜屼覆鍙ｆ棩蹇椾粛淇濈暀銆?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 涓嬭浇榛樿鍏抽棴 LCD 棰勮鐨勭増鏈€?  - 鎸?KEY1 鍚庣‘璁ゆ槸鍚︿笉鍐嶈繘鍏?BusFault銆?  - 鑻ヤ笉鍐?BusFault锛岃鏄?LCD 棰勮璺緞涓庢晠闅滃己鐩稿叧锛屽悗缁嫢浠嶉渶鐢婚潰棰勮搴斿崟鐙帓鏌?LCD 鍐欑獥鍙ｃ€佸潗鏍囪寖鍥淬€佹€荤嚎鏃跺簭鍜屾槸鍚︿笌 LVGL/LCD 鐘舵€佸啿绐併€?  - 鑻ヤ粛 BusFault锛屽垯缁х画妫€鏌?ATKQR 瀵圭伆搴︾紦鍐插湴鍧€/鍐呭瓨鍖哄煙鐨勮闂姹傦紝蹇呰鏃跺皢鐏板害 ROI 缂撳啿杩佺Щ鍒?AXI SRAM 闈欐€佹暟缁勩€?
2026-05-06 21:15 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡鏀逛负 KEY1 鎵嬪姩瑙﹀彂璇嗗埆

- 鏍规嵁娴嬭瘯闇€姹傦紝璋冩暣 `User\app\payment_qr_test.c`锛?  - 鍒濆鍖?`payment_qr` 鍚庝笉鍐嶈嚜鍔ㄥ畾鏃堕噰闆?璇嗗埆銆?  - 娴嬭瘯妯″紡鍚姩鏃惰皟鐢?`key_init()` 鍒濆鍖栨寜閿€?  - 涓诲惊鐜腑閫氳繃 `key_scan(0U)` 绛夊緟 `KEY1_PRES`銆?  - 鍙湁鎸変笅 KEY1 鏃舵墠鎵ц涓€娆★細
    - DCMIPP 鍗曞抚閲囬泦銆?    - LCD 鏄剧ず鎽勫儚澶寸敾闈㈠拰 ROI 绾㈡銆?    - 涓績 ROI 杞?8 浣嶇伆搴︺€?    - 璋冪敤 `payment_qr_process_frame()` / ATKQR 瑙ｇ爜銆?- 涓插彛棰勬湡锛?  - 鍚姩鍚庯細
    - `QR test waits for KEY1 press`
    - `QR test ready: press KEY1 to capture and decode once`
  - 鎸?KEY1 鍚庯細
    - `QR KEY1 pressed: capture and decode once`
    - `QR debug: mode=..., roi=(...), gray min=..., max=...`
    - `QR try: ...`
    - `[PAY_QR] atk_qr_decode res=...`
    - 璇嗗埆鎴愬姛鏃?`QR detected: <浜岀淮鐮佸唴瀹?`锛屾湭璇嗗埆鏃?`QR scanning... mode=...`
  - 濡傛灉鎸夊埌鍏朵粬閿紝浼氳緭鍑?`QR test ignored key=..., press KEY1 to decode`銆?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 鍚姩鍚庣‘璁や笉鍐嶈嚜鍔ㄨ繛缁瘑鍒€?  - 鎶婁簩缁寸爜鏀惧埌 LCD 绾㈡涓績銆?  - 鎸?KEY1 瑙﹀彂涓€娆¤瘑鍒苟瑙傚療涓插彛銆?  - 璋冩暣璺濈/鍏夌収鍚庡彲鍙嶅鎸?KEY1 瀵规瘮璇嗗埆缁撴灉銆?
2026-05-06 21:09 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡澧炲姞 LCD 鎽勫儚澶撮瑙堝拰 ROI 绾㈡

- 涓轰簡纭 ATKQR 瀹為檯杈撳叆鍥惧儚鏄惁娓呮櫚銆佸眳涓紝璋冩暣 `User\app\payment_qr_test.c`锛?  - 姣忔娴嬭瘯妯″紡閲囬泦 DCMIPP 鍗曞抚鍚庯紝灏嗗綋鍓?RGB565 鎽勫儚澶寸敾闈㈢洿鎺ユ樉绀哄埌 LCD銆?  - 鍦?LCD 涓婄敤绾㈣壊鐭╁舰妗嗘爣鍑洪€佺粰 ATKQR 鐨勪腑蹇?ROI銆?  - 褰撳墠 ROI 涓轰腑蹇冩渶澶?`320x320`锛屾棩蹇椾粛杈撳嚭锛?    - `QR debug: mode=rgb565, roi=(80,20 320x320), gray min=..., max=...`
- 璇ユ敼鍔ㄤ粎鐢ㄤ簬浜岀淮鐮佹祴璇曟ā寮忎笅鐨勬澘涓婅皟璇曪紝甯姪纭锛?  - 浜岀淮鐮佹槸鍚﹀湪绾㈡鍐呫€?  - 浜岀淮鐮佹槸鍚﹁冻澶熷ぇ銆?  - 鏄惁铏氱劍銆佽繃鏇濄€佸弽鍏夋垨鐢婚潰鏃嬭浆/闀滃儚銆?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 瑙傚療 LCD 涓婃憚鍍忓ご鐢婚潰鏄惁姝ｅ父銆?  - 灏嗕簩缁寸爜绉诲姩鍒扮孩妗嗕腑蹇冿紝骞惰浜岀淮鐮佸崰绾㈡杈冨ぇ姣斾緥銆?  - 鑻?LCD 涓婁簩缁寸爜娓呮櫚涓斾覆鍙ｄ粛 `res=3`锛岀户缁鏌?ATKQR 杈撳叆灏哄銆佹槸鍚﹂渶瑕佺缉鏀?浜屽€煎寲銆佸浘鍍忔棆杞?闀滃儚鍜屽簱瀵逛簩缁寸爜鐗堟湰/绾犻敊绛夌骇鐨勯檺鍒躲€?
2026-05-06 21:05 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡鏀逛负鍗曠伆搴︽ā寮忓畯閫夋嫨

- 鏍规嵁鏉夸笂鏃ュ織锛?  - `QR try: rgb565`
  - `[PAY_QR] atk_qr_decode res=3`
  - `QR try: rgb565_invert`
  鍚庣画鏈绗簩娆?`atk_qr_decode` 杩斿洖锛屽垽鏂弽鐩哥伆搴﹁緭鍏ュ彲鑳藉鑷?ATKQR 瑙ｇ爜鑰楁椂寮傚父鎴栧崱浣忋€?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 鍙栨秷姣忚疆杩炵画灏濊瘯 4 绉嶇伆搴﹀彉浣撱€?  - 鏀逛负閫氳繃瀹?`PAYMENT_QR_TEST_GRAY_MODE` 姣忔鍙€夋嫨涓€绉嶇伆搴︽ā寮忔祴璇曘€?  - 榛樿妯″紡锛?    - `PAYMENT_QR_TEST_GRAY_MODE_RGB565`
  - 鍙€夋ā寮忥細
    - `PAYMENT_QR_TEST_GRAY_MODE_RGB565`
    - `PAYMENT_QR_TEST_GRAY_MODE_RGB565_INVERT`
    - `PAYMENT_QR_TEST_GRAY_MODE_RGB565_SWAP`
    - `PAYMENT_QR_TEST_GRAY_MODE_RGB565_SWAP_INV`
  - 鎵弿鏃ュ織鏀逛负锛?    - `QR scanning... mode=<mode>`
- 杩欐牱鍙互閬垮厤鏌愪竴绉嶈緭鍏ユā寮忓崱浣忓悗闃绘柇涓诲惊鐜紝涔熶究浜庨€愰」涓嬫澘楠岃瘉銆?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇佸缓璁細
  - 鍏堢敤榛樿 `rgb565` 妯″紡纭涓嶅啀鍗″湪 `rgb565_invert`銆?  - 鑻ヤ粛 `res=3`锛屼紭鍏堣皟鏁翠簩缁寸爜璺濈/瀵圭劍/灞呬腑姣斾緥銆?  - 濡傞渶娴嬭瘯瀛楄妭搴忥紝鍐嶅崟鐙垏鎹㈠埌 `PAYMENT_QR_TEST_GRAY_MODE_RGB565_SWAP` 鏋勫缓涓嬫澘銆?  - 鏆備笉寤鸿鐩存帴娴嬭瘯鍙嶇浉妯″紡锛岄櫎闈為渶瑕佺‘璁?ATKQR 瀵归粦鐧芥瀬鎬х殑琛屼负銆?
2026-05-06 21:01 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡澧炲姞鐏板害鍙嶇浉鍜岄€愭ā寮忔棩蹇?
- 鏍规嵁鏉夸笂鏃ュ織锛?  - `QR debug: mode=rgb565, roi=(80,20 320x320), gray min=0, max=255`
  - `[PAY_QR] atk_qr_decode res=3`
  鍙垽鏂腑蹇?ROI 宸叉湁寮洪粦鐧藉姣旓紝闂鏇村彲鑳藉湪鍥惧儚鏋佹€с€丷GB565 瀛楄妭搴忋€佹柟鍚?闀滃儚銆佷簩缁寸爜灏哄鎴?ATKQR 杈撳叆闄愬埗銆?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 姣忚疆渚濇灏濊瘯 4 绉嶇伆搴﹁緭鍏ワ細
    - `rgb565`
    - `rgb565_invert`
    - `rgb565_swap`
    - `rgb565_swap_invert`
  - 姣忕妯″紡璋冪敤 ATKQR 鍓嶈緭鍑猴細
    - `QR try: <mode>`
  - 濡傛灉鏌愪釜妯″紡璇嗗埆鎴愬姛锛岃緭鍑猴細
    - `QR detected mode: <mode>`
    - `QR detected: <浜岀淮鐮佸唴瀹?`
  - 濡傛灉鍏ㄩ儴澶辫触锛岃緭鍑猴細
    - `QR scanning... tried gray variants`
- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 璁板綍 4 涓?`QR try` 鍚庡搴旂殑 `[PAY_QR] atk_qr_decode res=...`銆?  - 濡傛灉 4 绉嶅潎涓?`res=3` 涓?`gray min/max` 瀵规瘮鏄庢樉锛屼笅涓€姝ヤ紭鍏堟鏌ュ浘鍍忔槸鍚﹂暅鍍?鏃嬭浆銆佷簩缁寸爜鏄惁澶ぇ/澶皬/铏氱劍锛屼互鍙婃槸鍚﹂渶瑕佸 ROI 鍋氫簩鍊煎寲鎴栫缉鏀惧埌 ATKQR 鏇撮€傚悎鐨勫昂瀵搞€?
2026-05-06 20:57 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡澧炲姞 RGB565 瀛楄妭搴忓弻璺緞灏濊瘯

- 閽堝鏉夸笂浠嶆寔缁緭鍑猴細
  - `[PAY_QR] atk_qr_decode res=3`
  - `QR scanning...`
  璇存槑 ATKQR 鍒濆鍖栧拰璋冪敤姝ｅ父锛屼絾杈撳叆鍥惧儚浠嶆湭琚瘑鍒负浜岀淮鐮併€?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 涓嶅啀鍘熷湴瑕嗙洊鎽勫儚澶村畬鏁?RGB565 鍘熷浘銆?  - 鐏板害 ROI 缂撳啿鍖烘敼鏀惧湪褰撳墠甯ф暟鎹悗鏂圭殑绌轰綑 `camera_date_buf` 鍖哄煙銆?  - 姣忚疆鍏堟寜姝ｅ父 RGB565 瑙ｆ瀽涓績 ROI 骞惰皟鐢?`payment_qr_process_frame()`銆?  - 鑻ヤ粛涓?`PAYMENT_QR_SCANNING`锛屽啀鎸?RGB565 楂樹綆瀛楄妭浜ゆ崲鍚庨噸鏂扮敓鎴愮伆搴?ROI锛屽啀璋冪敤涓€娆?`payment_qr_process_frame()`銆?  - 鎵弿鏃ュ織浼氭樉绀猴細
    - `QR scanning... tried swapped RGB565`
  - 鐏板害璋冭瘯鏃ュ織浼氭樉绀鸿浆鎹㈡ā寮忥細
    - `QR debug: mode=rgb565, roi=(...), gray min=..., max=...`
    - 鍚庣画鑻ラ棿闅旀弧瓒筹紝涔熷彲鑳界湅鍒?`mode=rgb565_swap`銆?- 鐩殑锛氬揩閫熷垽鏂?DCMIPP/OV2640 杈撳嚭鍒板唴瀛樺悗鐨?RGB565 瀛楄妭搴忔槸鍚︿笌褰撳墠鐏板害杞崲鍋囪涓嶄竴鑷淬€?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婇獙璇侊細
  - 瑙傚療鏄惁鑳芥墦鍗?`QR detected: ...`銆?  - 濡傛灉浠嶅彧鎵弿锛岃褰?`QR debug` 鐨?`gray min/max`銆?  - 濡傛灉 `gray min/max` 瀵规瘮鏄庢樉浣嗚瘑鍒け璐ワ紝缁х画妫€鏌ヤ簩缁寸爜灏哄銆佺劍璺濄€佸浘鍍忔棆杞?闀滃儚鍜?ATKQR 杈撳叆灏哄闄愬埗銆?
2026-05-06 20:52 鏇存柊锛歱ayment_qr 娴嬭瘯妯″紡鏀逛负涓績 ROI 璇嗗埆骞跺鍔犵伆搴﹁皟璇曟棩蹇?
- 閽堝鏉夸笂鐜拌薄锛?  - `atk_qr_decode res=3`
  - `QR scanning...`
  鎸佺画鍑虹幇锛岃鏄?ATKQR 鍒濆鍖栧拰璋冪敤閾捐矾宸查€氾紝浣嗗綋鍓嶈緭鍏ュ浘鍍忔湭琚瘑鍒负浜岀淮鐮併€?- 璋冩暣 `User\app\payment_qr_test.c`锛?  - 涓嶅啀鎶婂畬鏁?`480x360` 鐢婚潰鐩存帴浼犵粰 ATKQR銆?  - 鏀逛负鎴彇鎽勫儚澶寸敾闈腑蹇冩柟褰?ROI锛屽綋鍓嶆渶澶?`320x320`銆?  - 灏嗕腑蹇?ROI 浠?RGB565 杞负 8 浣嶇伆搴﹀浘鍚庝紶缁?`payment_qr_process_frame()`銆?  - 姣忕害 3 绉掕緭鍑轰竴娆¤皟璇曚俊鎭細
    - `QR debug: roi=(x,y 320x320), gray min=..., max=...`
- 杩欐牱鍙互闄嶄綆浜岀淮鐮佸湪鍏ㄥ箙鐢婚潰涓崰姣旇繃灏忓鑷磋瘑鍒け璐ョ殑姒傜巼锛屼篃鏂逛究鍒ゆ柇鎽勫儚澶寸湅鍒扮殑涓績鍖哄煙鏄惁鏈夎冻澶熼粦鐧藉姣斻€?- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 涓嬩竴姝ユ澘涓婂垽鎹細
  - 濡傛灉 `gray min` 鍜?`gray max` 宸窛寰堝皬锛岃鏄庝腑蹇冪敾闈㈠姣斾笉瓒炽€佷簩缁寸爜涓嶅湪涓績鎴栫敾闈㈣繃鏇?杩囨殫銆?  - 濡傛灉 `gray min` 鎺ヨ繎 0 涓?`gray max` 鎺ヨ繎 255 浣嗕粛 `res=3`锛岀户缁鏌?RGB565 瀛楄妭搴忋€佷簩缁寸爜灏哄銆佺劍璺濄€侀暅鍍?鏃嬭浆鍜?ATKQR 瀵硅緭鍏ュ昂瀵哥殑瑕佹眰銆?  - 娴嬭瘯鏃惰浜岀淮鐮佸敖閲忎綅浜庢憚鍍忓ご鐢婚潰涓績锛屽苟鍗犱腑蹇?ROI 杈冨ぇ姣斾緥銆?
2026-05-06 20:48 鏇存柊锛氳皟鏁?payment_qr 娴嬭瘯妯″紡涓洪€氱敤浜岀淮鐮佸唴瀹规墦鍗?
- 璋冩暣 `User\app\payment_qr_test.c` 鐨勪复鏃舵祴璇曟祦绋嬨€?- `PAYMENT_QR_TEST_MODE` 寮€鍚悗锛屼笉鍐嶈姹備簩缁寸爜鍐呭蹇呴』鍖归厤 `PAY_OK_SELF_CHECKOUT_001`銆?- 娴嬭瘯妯″紡鍒濆鍖栧悗姣忛殧绾?1 绉掗噰闆嗕竴甯с€佽浆鎹负 8 浣嶇伆搴﹀浘骞惰皟鐢?`payment_qr_process_frame()`銆?- 鍙 ATKQR 璇嗗埆鍒颁换鎰忎簩缁寸爜鍐呭锛屼覆鍙ｈ緭鍑猴細
  - `QR detected: <浜岀淮鐮佸唴瀹?`
- 鏈瘑鍒埌浜岀淮鐮佹椂锛屾瘡闅旂害 1 绉掕緭鍑猴細
  - `QR scanning...`
- 鏈涓嶆帴鎵爜鏀粯 UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- `payment_qr_process_frame()` 鍐呴儴浠嶄繚鐣欏浐瀹氱洰鏍囦覆鍖归厤鐘舵€侊紝鏂逛究鍚庣画姝ｅ紡鏀粯娴佺▼澶嶇敤锛涙湰娆″彧鏄祴璇曞叆鍙ｄ笉鍐嶆寜 valid/invalid 鎵撳嵃鏀粯缁撴灉銆?- 宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鏉夸笂楠岃瘉锛氬鍑嗕换鎰忎簩缁寸爜鏃朵覆鍙ｆ槸鍚︽瘡绉掕緭鍑鸿瘑鍒唴瀹癸紱鑻ヤ粛鍙緭鍑?`QR scanning...`锛岀户缁鏌ヤ簩缁寸爜澶у皬銆佸鐒︺€佸厜鐓с€丷GB565 瀛楄妭搴忓拰鐏板害杞崲鏁堟灉銆?
2026-05-06 20:35 鏇存柊锛氭柊澧?payment_qr 鐙珛浜岀淮鐮佽瘑鍒祴璇曞叆鍙?
- 鏂板涓存椂娴嬭瘯妯″潡锛?  - `User\app\payment_qr_test.c`
  - `User\app\payment_qr_test.h`
- 涓や釜 Keil 宸ョ▼鍧囧凡鍔犲叆 `payment_qr_test.c`锛?  - `Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx`
  - `Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx`
- `User\app\payment_qr.h` 鏂板瀹忓紑鍏筹細
  - `PAYMENT_QR_TEST_MODE`
  - 榛樿鍊间负 `0U`锛屽嵆鍏抽棴娴嬭瘯妯″紡锛屼繚鎸佸師鑷姪缁撹处娴佺▼銆?  - 涓存椂娴嬭瘯 ATKQR 鏃跺彲鏀逛负 `1U`锛岄噸鏂版瀯寤哄苟涓嬭浇銆?- `User\main.c` 鍦?`PAYMENT_QR_TEST_MODE != 0U` 鏃惰皟鐢細
  - `payment_qr_test_start(&camera_state)`
  - 涓诲惊鐜腑璋冪敤 `payment_qr_test_loop(&camera_state)`
- 娴嬭瘯妯″紡琛屼负锛?  - 鍒濆鍖?`payment_qr`锛屼覆鍙ｈ緭鍑?`QR init ok`銆?  - 寰幆璋冪敤褰撳墠椤圭洰宸叉湁 `dcmipp_start()` 閲囬泦 OV2640 RGB565 鍗曞抚銆?  - 鍦ㄦ祴璇曟ā寮忓唴灏?RGB565 甯у師鍦拌浆鎹负 8 浣嶇伆搴﹀浘銆?  - 璋冪敤 `payment_qr_process_frame()`锛屽唴閮ㄧ户缁皟鐢?`atk_qr_decode()`銆?  - 鍛ㄦ湡杈撳嚭 `QR scanning...`銆?  - 璇嗗埆鍒板浐瀹氬唴瀹?`PAY_OK_SELF_CHECKOUT_001` 鏃惰緭鍑猴細
    - `QR detected: PAY_OK_SELF_CHECKOUT_001`
    - `QR valid: PAY_OK_SELF_CHECKOUT_001`
    - `PAYMENT QR VALID`
  - 璇嗗埆鍒板叾浠栦簩缁寸爜鏃惰緭鍑猴細
    - `QR detected: ...`
    - `QR invalid: ...`
    - `PAYMENT QR INVALID`
  - 鎹曡幏澶辫触鎴?ATKQR 閿欒鏃惰緭鍑?`QR decode error: ...`銆?- 鏈鍙仛 ATKQR 鐙珛璇嗗埆娴嬭瘯鍏ュ彛锛屼笉鎺ユ壂鐮佹敮浠?UI銆佷笉鏀圭粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉杩涘叆鏀粯鎴愬姛椤点€?- 涓烘柟渚挎祴璇曞眰杈撳嚭閿欒鐮侊紝`payment_qr.c/.h` 鏂板锛?  - `payment_qr_get_last_decode_result()`
- 褰撳墠鎽勫儚澶村抚浠嶆槸 RGB565锛屼笉鑳界洿鎺ヤ紶缁?ATKQR锛涙祴璇曟ā鍧楀凡缁忓仛涓存椂 RGB565 -> 8 浣嶇伆搴﹁浆鎹€傚悗缁寮忔帴鎵爜鏀粯娴佺▼鏃朵粛闇€璇勪及鐏板害缂撳啿浣嶇疆銆佽浆鎹㈣€楁椂鍜屾槸鍚﹀奖鍝?UI 棰勮銆?- 鏈満宸查噸鏂版瀯寤?full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 鏉夸笂娴嬭瘯鍥?P7 鐨勪娇鐢ㄦ柟寮忥細
  - 灏嗗浐瀹氫簩缁寸爜鍥?P7 鏄剧ず鍦ㄦ墜鏈烘垨鐢佃剳灞忓箷涓娿€?  - 浜岀淮鐮佸唴瀹瑰繀椤讳负 `PAY_OK_SELF_CHECKOUT_001`銆?  - 鎽勫儚澶村鍑嗕簩缁寸爜锛岃皟鏁磋窛绂汇€佽搴﹀拰鍏夌収锛岄伩鍏嶅睆骞曞弽鍏夊拰杩囨洕銆?  - 瑙傚療涓插彛鏄惁浠?`QR scanning...` 鍙樹负 `PAYMENT QR VALID`銆?- 浠嶉渶鏉夸笂楠岃瘉锛?  - ATKQR 鍒濆鍖栨槸鍚︾ǔ瀹氭垚鍔熴€?  - 褰撳墠 48KB LVGL 鍐呭瓨姹犳槸鍚﹁冻澶?ATKQR 瑙ｇ爜杩囩▼浣跨敤銆?  - RGB565 鍘熷湴鐏板害杞崲鍚庣殑鍥惧儚鏄惁鑳借 `atk_qr_decode()` 姝ｇ‘璇嗗埆銆?  - 涓嶅悓璺濈銆佷寒搴︺€佸睆骞曞埛鏂扮汗鐞嗕笅璇嗗埆鑰楁椂鍜屾垚鍔熺巼鏄惁婊¤冻鍚庣画鎵爜鏀粯鍦烘櫙銆?
2026-05-06 20:06 鏇存柊锛氬皝瑁呬簩缁寸爜鏀粯璇嗗埆妯″潡

- 鍦?`User\app\payment_qr.c/.h` 涓皝瑁?ATKQR 璋冪敤鎺ュ彛锛屾湰娆″彧鍋氭ā鍧楀皝瑁咃紝涓嶆帴鎵爜鏀粯 UI銆佷笉鎺ョ粨璐︽寜閽€佷笉娓呯┖璐墿杞︺€佷笉閲嶆柊鍒濆鍖栨憚鍍忓ご銆?- 鏂板鍏紑鎺ュ彛锛?  - `payment_qr_init()`锛氬唴閮ㄨ皟鐢?`atk_qr_init()`锛岃繑鍥?`PAYMENT_QR_INIT_OK` 鎴?`PAYMENT_QR_INIT_FAIL`銆?  - `payment_qr_reset()`锛氭竻绌轰笂涓€娆¤瘑鍒枃鏈拰鐘舵€侊紝鐢ㄤ簬閲嶆柊鎵爜銆?  - `payment_qr_process_frame(const uint8_t *gray_image, uint16_t width, uint16_t height)`锛氳緭鍏?8 浣嶇伆搴﹀浘锛屽唴閮ㄨ皟鐢?`atk_qr_decode()`锛屾寜缁撴灉杩斿洖 `PAYMENT_QR_SCANNING`銆乣PAYMENT_QR_INVALID`銆乣PAYMENT_QR_VALID` 鎴栭敊璇姸鎬併€?  - `payment_qr_get_last_text()`锛氳繑鍥炴渶杩戜竴娆¤瘑鍒埌鐨勪簩缁寸爜鍐呭銆?  - `payment_qr_get_status()`锛氳繑鍥炲綋鍓嶇姸鎬併€?  - `payment_qr_deinit()`锛氬唴閮ㄨ皟鐢?`atk_qr_destroy()` 骞舵竻绌轰笂涓嬫枃銆?- 瀹氫箟鍥哄畾鐩爣瀛楃涓诧細
  - `PAY_OK_SELF_CHECKOUT_001`
- 瀹氫箟鐘舵€佹灇涓撅細
  - `PAYMENT_QR_IDLE`
  - `PAYMENT_QR_INIT_OK`
  - `PAYMENT_QR_INIT_FAIL`
  - `PAYMENT_QR_SCANNING`
  - `PAYMENT_QR_DETECTED`
  - `PAYMENT_QR_VALID`
  - `PAYMENT_QR_INVALID`
  - `PAYMENT_QR_ERROR`
- `payment_qr_process_frame()` 褰撳墠鏄庣‘瑕佹眰浼犲叆 8 浣嶇伆搴﹀浘锛屾瘡鍍忕礌 1 瀛楄妭銆備笉鑳界洿鎺ヤ紶褰撳墠鎽勫儚澶?RGB565 甯э紱鍚庣画鎺ュ叆鍓嶄粛闇€瀹炵幇 RGB565 鍒扮伆搴﹀浘杞崲銆?- 鍦?`payment_qr.c` 鍐呮彁渚?ATKQR 鎵€闇€鐨勭Щ妞嶅嚱鏁帮細
  - `atk_qr_memset`
  - `atk_qr_malloc`
  - `atk_qr_realloc`
  - `atk_qr_free`
  - `atk_qr_memcpy`
  - `atk_qr_convert`
  褰撳墠鍐呭瓨鐢宠/閲婃斁鏆傛椂澶嶇敤 LVGL 鐨?`lv_mem_alloc/lv_mem_realloc/lv_mem_free`锛宍atk_qr_convert()` 鏆傛椂鍘熸牱杩斿洖 Unicode 缂栫爜锛涚洰鏍囨敮浠樹覆涓?ASCII锛屼笉渚濊禆涓枃杞爜銆?- 宸插鍔犱覆鍙ｈ皟璇曟棩蹇楋細
  - 鍒濆鍖栨槸鍚︽垚鍔熴€?  - `atk_qr_decode()` 杩斿洖鍊笺€?  - 璇嗗埆鑰楁椂锛屽崟浣?ms銆?  - 璇嗗埆鍒扮殑瀛楃涓层€?  - 鏄惁鍖归厤 `PAY_OK_SELF_CHECKOUT_001`銆?- 涓洪伩鍏?ATKQR 澶存枃浠朵腑瑁?`#include "sys.h"` 涓庡綋鍓?include path 涓嶅尮閰嶏紝鏈娌℃湁鍦?`payment_qr.c` 鐩存帴鍖呭惈 `atk_qrdecode.h`锛岃€屾槸鍦ㄦā鍧楀唴閮ㄥ０鏄庡皯閲?ATKQR 鍑芥暟鍘熷瀷鍜岄敊璇爜锛涚涓夋柟澶存枃浠朵粛淇濈暀鍦?`Middlewares\ATKQR\Inc` 渚涘悗缁弬鑰冦€?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鍚庣画鏉夸笂楠岃瘉锛欰TKQR 鍒濆鍖栨槸鍚﹁兘浠?LVGL 鍐呭瓨姹犳垚鍔熺敵璇峰唴瀛樸€?8KB LVGL 鍐呭瓨姹犳槸鍚﹁冻澶熶簩缁寸爜璇嗗埆銆佺伆搴﹀浘杞崲鍚?`atk_qr_decode()` 璇嗗埆鑰楁椂鍜屽噯纭巼鏄惁婊¤冻缁撹处鍦烘櫙銆?
2026-05-06 19:48 鏇存柊锛氫簩缁寸爜搴撶Щ妞嶅墠宸ョ▼鍑嗗

- 鏍规嵁 ATKQR README 鍋氫簩缁寸爜璇嗗埆搴撶Щ妞嶅墠宸ョ▼鍑嗗锛屾湰娆′笉鎺ユ壂鐮佹敮浠?UI銆佷笉鎺ョ粨璐︽寜閽€佷笉璋冪敤 `atk_qr_decode`銆?- 宸叉鏌ュ惎鍔ㄦ枃浠讹細
  - `Drivers\CMSIS\Device\ST\STM32H7RSxx\Source\Templates\arm\startup_stm32h7r3xx.s`
  - `Stack_Size EQU 0x00001000`
  褰撳墠宸叉弧瓒?ATKQR 瑕佹眰鐨?`0x1000` 鎴栦互涓婏紝鏈鏈慨鏀瑰惎鍔ㄦ枃浠躲€?- 宸插鍒?ATKQR 蹇呴渶搴撴枃浠讹細
  - `Middlewares\ATKQR\Inc\atk_qrdecode.h`
  - `Middlewares\ATKQR\Lib\ATK_QR_V1.4.lib`
- 鏈娌℃湁澶嶅埗 ATKQR 鐨?`main.c`锛屼篃娌℃湁澶嶅埗 ATKQR 渚嬬▼涓殑 LCD銆佹憚鍍忓ご銆佸欢鏃躲€佷覆鍙ｃ€丅SP 绛夊簳灞傛枃浠讹紝閬垮厤鍜屽綋鍓嶈嚜鍔╃粨璐﹀伐绋嬬殑鎽勫儚澶淬€丩VGL銆丩CD銆丅SP 鍒濆鍖栭摼璺啿绐併€?- 鏂板浜岀淮鐮佹敮浠樻ā鍧楀崰浣嶆枃浠讹細
  - `User\app\payment_qr.c`
  - `User\app\payment_qr.h`
  褰撳墠鍙繚瀛?娓呯悊璇嗗埆缁撴灉鐘舵€侊紝浣滀负鍚庣画鎺ュ叆鐐癸紝涓嶆敼鍙樼幇鏈変笟鍔℃祦绋嬨€?- 涓や釜 Keil 宸ョ▼宸插姞鍏ワ細
  - `User\app\payment_qr.c`
  - `Middlewares\ATKQR\Lib\ATK_QR_V1.4.lib`
  - include path 鏂板 `..\..\Middlewares\ATKQR\Inc`
- 鏈浠嶆湭杩佺Щ/瀹炵幇 `atk_qrdecode.c` 閫傞厤灞傦紝鍥犱负鍘熸枃浠朵緷璧?`mymalloc/myfree/myrealloc/mymemset/mymemcpy`銆乣SRAMIN` 鍜?`ff_convert`锛岄渶瑕佷笅涓€闃舵缁撳悎褰撳墠宸ョ▼鍐呭瓨姹犮€丷GB565 杞?8 浣嶇伆搴﹀浘銆佷腑鏂囪浆鐮佺瓥鐣ュ悗鍐嶅仛銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 浠嶉渶鍚庣画瀹炵幇骞舵澘涓婇獙璇侊細ATKQR 鍐呭瓨閫傞厤銆丷GB565 鍒?8 浣嶇伆搴﹁浆鎹€乣atk_qr_init/decode/destroy` 璋冪敤鏃舵満銆佹壂鐮佽€楁椂銆佽瘑鍒粨鏋滃瓧绗︿覆缂栫爜鍜屼笌缁撹处娴佺▼鐨勮仈鍔ㄣ€?

1. 椤圭洰姒傚喌

鏈」鐩槸杩愯鍦?STM32H7R3 涓婄殑鑷姪缁撹处婕旂ず椤圭洰銆?宸查泦鎴愮殑涓昏鍔熻兘锛?
- OV2640 鎽勫儚澶撮噰鍥?- X-CUBE-AI 鎺ㄧ悊
- HX711 绉伴噸
- LVGL 绔栧睆鐣岄潰
- 璐墿杞﹂€昏緫
- 浠锋牸璁＄畻
- 閲嶉噺绋冲畾鍚庤嚜鍔ㄨЕ鍙戣瘑鍒?
褰撳墠鏀寔鐨勫晢鍝侊細

- 鑻规灉 / Apple
- 棣欒晧 / Banana
- 姗欏瓙 / Orange
- 鍐滃か灞辨硥 / NFSQ
- 鎬″疂 / Yibao

璁′环瑙勫垯锛?
- 鑻规灉 / 棣欒晧 / 姗欏瓙锛氭寜閲嶉噺璁′环锛屽崟浣嶆樉绀轰负 鍏?鍗冨厠
- 鍐滃か灞辨硥 / 鎬″疂锛氭寜閲嶉噺浼扮畻鐡舵暟锛屽崟浣嶆樉绀轰负 鍏?鐡?

2. 褰撳墠 Keil 宸ョ▼

褰撳墠涓诲伐绋嬶細

- E:\H7R3\鑷姪缁撹处\Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx

褰撳墠蹇笅鏉垮伐绋嬶細

- E:\H7R3\鑷姪缁撹处\Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx

鏃х殑 `atk_h7r3_full.uvprojx` 宸ョ▼鐩告満鍒濆鍖栦笉姝ｅ父锛屽凡缁忓垹闄ゃ€?`camrun` 宸ョ▼鏄綋鍓嶄富鍩虹嚎锛屽洜涓哄畠淇濈暀浜嗗鎽勫儚澶磋繍琛屾洿鍙嬪ソ鐨勫唴瀛樺竷灞€銆?
浣跨敤鏂瑰紡锛?
- 绗竴娆＄儳褰曘€丄I 妯″瀷鍙樺寲銆丄I 鏉冮噸鍙樺寲銆佸閮?Flash 琚摝闄ゅ悗锛屼娇鐢?`atk_h7r3_full_camrun.uvprojx` 鍏ㄩ噺涓嬭浇銆?- 骞虫椂鍙敼 UI銆佷笟鍔￠€昏緫銆佹櫘閫氬浐浠朵唬鐮佹椂锛屼娇鐢?`atk_h7r3_full_camrun_fast.uvprojx` 蹇€熶笅鏉裤€?- 濡傛灉浣跨敤 fast 宸ョ▼鍚庤瘑鍒紓甯革紝鍏堢‘璁ゅ閮?Flash 涓殑 AI 鏉冮噸鏄惁宸茬粡鐢?full 宸ョ▼姝ｇ‘鍐欏叆銆?
闄ら潪鏈夋槑纭繀瑕佸苟涓旀湁纭欢楠岃瘉鏃堕棿锛屽惁鍒欎笉瑕佸悎骞舵垨閲嶅啓杩欎袱涓?Keil 宸ョ▼澹炽€?

3. 閲嶈 Scatter 鏂囦欢

褰撳墠 scatter 鏂囦欢锛?
- E:\H7R3\鑷姪缁撹处\User\Script\ATK-DNH7R3_flash_ROMxspi1.sct
- E:\H7R3\鑷姪缁撹处\User\Script\ATK-DNH7R3_flash_ROMxspi1_fast.sct
- E:\H7R3\鑷姪缁撹处\User\Script\ATK-DNH7R3_flash_ROMxspi1_camrun.sct
- E:\H7R3\鑷姪缁撹处\User\Script\ATK-DNH7R3_flash_ROMxspi1_camrun_fast.sct

鍚箟锛?
- `ATK-DNH7R3_flash_ROMxspi1_camrun.sct`
  褰撳墠涓诲伐绋嬩娇鐢ㄧ殑 scatter銆?
- `ATK-DNH7R3_flash_ROMxspi1_camrun_fast.sct`
  褰撳墠蹇笅鏉垮伐绋嬩娇鐢ㄧ殑 scatter锛岀敤浜庨伩鍏嶉噸澶嶄笅杞藉ぇ鐨?AI 鏉冮噸娈点€?
- `ATK-DNH7R3_flash_ROMxspi1.sct` 鍜?`ATK-DNH7R3_flash_ROMxspi1_fast.sct`
  浣滀负鍘嗗彶鍜屽鐓ф枃浠朵繚鐣欍€傚垹闄ゅ墠蹇呴』纭娌℃湁寮曠敤锛屽苟璇勪及纭欢褰卞搷銆?
scatter 鏂囦欢灞炰簬楂橀闄╂枃浠躲€備箣鍓嶆憚鍍忓ご绋冲畾鎬у拰杩愯鏃跺唴瀛樺竷灞€寮虹浉鍏炽€?

4. Keil 鍘嗗彶闂锛欼llegal start vector

鏇剧粡閬囧埌杩?Keil 闂锛?
`Illegal start vector!`

鐜拌薄鏄墦寮€ `Options for Target` 浼氬脊鍑鸿閿欒銆?
杈冨彲鑳界殑鏍瑰洜鏄?Keil 宸ョ▼ XML 涓煇浜涚湅璧锋潵涓虹┖銆佸疄闄呭甫绌虹櫧瀛楃鐨勫瓧娈佃 Keil 褰撴垚浜嗛潪娉曞€艰В鏋愩€?鍚庢潵娓呯悊浜嗗伐绋?XML 瀛楁锛屼娇 Keil Options 鑳芥甯告墦寮€銆?
娉ㄦ剰锛?
- 涓嶈闅忔剰閲嶅啓 `uvprojx`銆乣uvoptx` 鎴?scatter銆?- 濡傛灉 Keil Options 鍐嶆鎶ラ敊锛屼紭鍏堟妸宸ョ▼ XML 瀛楁鍜屽彲姝ｅ父鎵撳紑鐨?ALIENTEK LVGL 渚嬬▼瀵规瘮銆?- 濡傛灉鏀瑰伐绋嬫枃浠跺悗鎽勫儚澶村紓甯革紝鍏堟煡 scatter 鍜屽伐绋嬪３宸紓锛屽啀鏌?OV2640 椹卞姩浠ｇ爜銆?

5. 鎽勫儚澶翠笌璇嗗埆娴佺▼

鏍稿績鏂囦欢锛?
- E:\H7R3\鑷姪缁撹处\User\app\camera_workflow.c
- E:\H7R3\鑷姪缁撹处\Drivers\BSP\OV2640\ov2640.c
- E:\H7R3\鑷姪缁撹处\Drivers\BSP\OV2640\sccb.c
- E:\H7R3\鑷姪缁撹处\Drivers\BSP\HX711\hx711.c

鐘舵€佹満娴佺▼锛?
`EMPTY -> DETECT_PLACEMENT -> WAIT_STABLE -> RECOGNIZING -> WAIT_REMOVE`

娴佺▼璇存槑锛?
- 绛夊緟鍟嗗搧鏀句笂绉伴噸鍖哄煙
- 绛夊緟閲嶉噺绋冲畾
- 鎷嶇収
- 杩愯 AI 鍒嗙被
- 灏?AI 鏍囩鏄犲皠鎴愬晢鍝?- 鍔犲叆璐墿杞?- 绛夊緟鍟嗗搧鍙栬蛋鍚庤繘鍏ヤ笅涓€杞?
閲嶈琛屼负棰勬湡锛?
- 鍟嗗搧璇嗗埆瀹屾垚骞跺彇璧板悗锛岄噸閲忓洖闆舵椂璇嗗埆缁撴灉鍖哄煙搴旀竻绌恒€?- 璐墿杞︽潯鐩偣鍑昏涓哄簲涓猴細绗竴娆＄偣鍑婚€変腑锛屽悓涓€涓潯鐩浜屾鐐瑰嚮鍙栨秷閫変腑銆?- 璇嗗埆澶辫触鏃跺簲寮瑰嚭涓枃澶辫触鎻愮ず锛屽苟鍦ㄥ彇璧板晢鍝佸悗鍏佽涓嬩竴娆￠噸璇曘€?
鎽勫儚澶存椂搴忋€侀噸閲忕ǔ瀹氭€с€丄I 璇嗗埆鍜?UI 鍒锋柊鏈€缁堥兘蹇呴』浠ユ澘涓婄幇璞′负鍑嗐€?

6. LVGL UI 褰撳墠鐘舵€?
2026-04-28 22:37 鏇存柊锛?
- 鎸夊浘3瀹炵幇鈥滆瘑鍒け璐ュ脊绐椻€濄€傛湰娆″彧琛ヨ瘑鍒け璐ュ脊绐楀唴瀹癸紝涓嶆敼鍏朵粬纭寮圭獥銆佺粨璐﹀畬鎴愬脊绐楁垨涓婚〉闈㈠竷灞€銆?- 璇嗗埆澶辫触寮圭獥缁х画澶嶇敤 `checkout_ui` 鐜版湁鍏ㄥ睆妯℃€侀伄缃╋細
  - 閬僵瀵硅薄锛歚g_checkout_ui_pages.popup_mask`
  - 瑕嗙洊 `480x800`
  - 榛戣壊鍗婇€忔槑锛屽脊绐楁樉绀烘椂搴曞眰椤甸潰涓嶅彲鎿嶄綔銆?- `User\ui\checkout_ui.c` 鏂板璇嗗埆澶辫触寮圭獥瀵硅薄鍙ユ焺锛?  - `g_checkout_ui_recog_fail.panel`
  - `g_checkout_ui_recog_fail.icon_bg`
  - `g_checkout_ui_recog_fail.icon_label`
  - `g_checkout_ui_recog_fail.title_label`
  - `g_checkout_ui_recog_fail.body_label`
  - `g_checkout_ui_recog_fail.ok_button`
- 寮圭獥鏈綋锛?  - 浣跨敤宸叉湁 `g_checkout_ui_pages.recog_fail_popup`
  - 灏哄 `310x190`
  - 鐧借壊鑳屾櫙锛屽渾瑙?`12`
  - 灞呬腑鏄剧ず銆?- 寮圭獥鍐呭锛?  - 椤堕儴绾㈣壊鍦嗗舰鍥炬爣锛岀櫧鑹?`X` 瀛楃鍗犱綅銆?  - 鏍囬锛歚璇嗗埆澶辫触`
  - 姝ｆ枃锛歚璇峰彇璧板晢鍝佸悗閲嶆柊鏀剧疆`
  - 搴曢儴钃濊壊 `纭畾` 鎸夐挳锛屽昂瀵?`220x44`銆?- 鏄剧ず/鍏抽棴鎺ュ彛锛?  - 鏄剧ず锛歚checkout_ui_show_recog_fail_popup()`
  - 闅愯棌锛氬鐢?`checkout_ui_hide_popup()`
  - 鐐瑰嚮纭畾鎸夐挳璋冪敤 `checkout_ui_popup_cancel_event_cb()`锛屽彧鍏抽棴寮圭獥骞跺洖鍒板綋鍓嶄富椤甸潰鐘舵€併€?- 鏈涓嶆敼璇嗗埆鐘舵€佹満銆佷笉鏀?AI銆佺О閲嶃€佹憚鍍忓ご銆佽喘鐗╄溅搴曞眰銆並eil 宸ョ▼澹虫垨 scatter銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 褰撳墠浜х墿锛?  - `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/28 22:37:22
  - `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/28 22:37:50
- 浠嶉渶鏉夸笂楠岃瘉锛氳瘑鍒け璐ユ椂寮圭獥鏄惁鏄剧ず銆侀伄缃╂槸鍚﹂樆姝㈠簳灞傛搷浣溿€佹枃瀛楁槸鍚﹀眳涓笖鏃犵己瀛?婧㈠嚭銆佺‘瀹氭寜閽槸鍚﹀彧鍏抽棴寮圭獥涓斾笉褰卞搷涓婚〉闈㈠綋鍓嶇姸鎬併€?
2026-04-28 22:30 鏇存柊锛?
- 瀹屾垚褰撳墠 GUI 闃舵鏈€鍚庝竴姝ワ細瀹炵幇鈥滅粨璐﹀畬鎴愬脊绐椻€濓紙鍥?锛夛紝骞惰ˉ榻愪富璇嗗埆椤点€佽喘鐗╄溅椤典笌鍥涗釜寮圭獥涔嬮棿鐨勫熀纭€鑱斿姩銆傛湰娆′笉閲嶆柊璁捐宸叉湁椤甸潰甯冨眬銆?- `User\ui\checkout_ui.c/.h` 鏂板/璋冩暣鎺ュ彛锛?  - `checkout_ui_show_checkout_done_popup(uint32_t total_price_cent)`
  - `checkout_ui_refresh_main_summary(const cart_context_t *cart)`
  - `checkout_ui_refresh_cart_page(const cart_context_t *cart)`
  - `checkout_ui_refresh_all(const cart_context_t *cart)`
  - `checkout_ui_set_cart_action_callbacks(...)` 鎵╁睍涓烘敞鍐屽垹闄ゃ€佹竻绌恒€佺粨璐﹀畬鎴愬拰鑾峰彇璐墿杞﹀洖璋冦€?- 缁撹处瀹屾垚寮圭獥瀵硅薄鍙ユ焺锛?  - `g_checkout_ui_checkout_done.panel`
  - `g_checkout_ui_checkout_done.icon_bg`
  - `g_checkout_ui_checkout_done.icon_label`
  - `g_checkout_ui_checkout_done.title_label`
  - `g_checkout_ui_checkout_done.body_label`
  - `g_checkout_ui_checkout_done.amount_prefix_label`
  - `g_checkout_ui_checkout_done.amount_value_label`
  - `g_checkout_ui_checkout_done.amount_unit_label`
  - `g_checkout_ui_checkout_done.ok_button`
- 缁撹处瀹屾垚寮圭獥鏍峰紡锛?  - 閬僵澶嶇敤 `popup_mask` 鍏ㄥ睆鍗婇€忔槑榛戣壊銆?  - 寮圭獥鏈綋 `320x210`锛岀櫧搴曪紝鍦嗚 `12`锛屽眳涓€?  - 椤堕儴缁胯壊鍦嗗舰鎴愬姛鍥炬爣锛屽綋鍓嶇敤鐧借壊 `V` 浣滀负瀵瑰嬀鍗犱綅銆?  - 鏍囬 `缁撹处瀹屾垚`銆?  - 姝ｆ枃 `鎰熻阿浣跨敤鏈郴缁焋銆?  - 閲戦琛?`鏈鎬婚噾棰濓細xx.xx 鍏僠锛岄噾棰濆拰鍗曚綅涓虹孩鑹层€?  - 搴曢儴钃濊壊 `纭畾` 鎸夐挳锛屽昂瀵?`220x44`銆?- 椤甸潰鍒囨崲鍜屾寜閽粦瀹氾細
  - 涓昏瘑鍒〉鈥滄煡鐪嬭喘鐗╄溅鈥?-> `checkout_ui_show_cart_page()`
  - 璐墿杞﹂〉杩斿洖鎸夐挳 -> `checkout_ui_show_main_page()`
  - 涓昏瘑鍒〉鈥滅粨璐︹€?-> 鑻ヨ喘鐗╄溅闈炵┖锛屾樉绀虹粨璐﹀畬鎴愬脊绐楋紱鑻ヨ喘鐗╄溅涓虹┖锛岀洿鎺ュ拷鐣ャ€?  - 璐墿杞﹂〉鈥滃幓缁撹处鈥?-> 鑻ヨ喘鐗╄溅闈炵┖锛屾樉绀虹粨璐﹀畬鎴愬脊绐楋紱鑻ヨ喘鐗╄溅涓虹┖锛岀洿鎺ュ拷鐣ャ€?  - 缁撹处瀹屾垚寮圭獥鈥滅‘瀹氣€?-> 鍏抽棴寮圭獥銆佽皟鐢ㄧ粨璐﹀畬鎴愪笟鍔″洖璋冦€佹竻绌哄綋鍓嶈瘑鍒粨鏋滄樉绀恒€佹€讳环褰掗浂銆佸埛鏂颁富椤甸潰鍜岃喘鐗╄溅椤点€佽繑鍥炰富璇嗗埆椤碉紝骞跺皢 UI 鐘舵€佹樉绀轰负 `璇锋斁缃晢鍝乣銆?  - 涓昏瘑鍒〉鈥滄竻绌鸿喘鐗╄溅鈥濄€佽喘鐗╄溅椤甸《閮ㄢ€滄竻绌衡€濄€佽喘鐗╄溅椤靛簳閮ㄢ€滄竻绌鸿喘鐗╄溅鈥?-> 娓呯┖纭寮圭獥銆?  - 璐墿杞﹂〉鈥滃垹闄ら€変腑椤光€?-> 鍒犻櫎纭寮圭獥銆?  - 涓昏瘑鍒〉鈥滅户缁喘鐗┾€濆綋鍓嶄粛涓虹┖澹充簨浠讹紝浣滀负鍚庣画涓氬姟鎵╁睍鍏ュ彛淇濈暀銆?- `User\main.c` 淇濇寔鍙彁渚涗笟鍔￠€傞厤鍥炶皟锛?  - `main_delete_selected_cart_item()`
  - `main_clear_cart()`
  - `main_complete_checkout()`
  - `main_get_cart()`
  UI 寮圭獥甯冨眬鍜屾寜閽樉绀洪€昏緫浠嶄繚鐣欏湪 `checkout_ui.c`銆?- 鏈涓嶆敼璐墿杞﹀簳灞傘€佷笉鏀硅瘑鍒姸鎬佹満銆佷笉鏀?AI銆佺О閲嶃€佹憚鍍忓ご銆乻catter 鎴?Keil 宸ョ▼澹炽€?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 褰撳墠浜х墿锛?  - `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/28 22:29:57
  - `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/28 22:30:25
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 缁撹处瀹屾垚寮圭獥甯冨眬鏄惁璐磋繎鍥?锛岄噾棰濈孩鑹叉槸鍚︽甯搞€?  - 涓昏瘑鍒〉鍜岃喘鐗╄溅椤甸兘鑳借Е鍙戠粨璐﹀畬鎴愬脊绐椼€?  - 缁撹处瀹屾垚鍚庤喘鐗╄溅娓呯┖銆佸綋鍓嶈瘑鍒粨鏋滄竻绌恒€佷袱涓〉闈㈡€讳环褰掗浂銆佽繑鍥炰富璇嗗埆椤点€?  - 绌鸿喘鐗╄溅鐐瑰嚮缁撹处鏄惁鏃犲紓甯搞€?  - 鍒犻櫎/娓呯┖鍚庝富椤甸潰鎽樿鍜岃喘鐗╄溅椤垫槸鍚﹀悓姝ュ埛鏂般€?
2026-04-28 22:17 鏇存柊锛?
- 鏍规嵁鏈€鏂颁覆鍙ｏ細
  - `[FLOW] AI classify start`
  - `[AI] Live preprocess start`
  涔嬪悗鐤戜技绋嬪簭璺戦锛岃鏄庡崱鐐瑰凡缁忓畾浣嶅埌 RGB565 ROI 棰勫鐞嗗紑濮嬪悗銆佸皻鏈畬鎴愯緭鍏?tensor 鍑嗗銆?- 杩涗竴姝ユ帓鏌ュ彂鐜?`Drivers\BSP\DCMIPP\dcmipp.c` 鍦ㄦ媿鐓у墠鍚庝娇鐢ㄦ暣鐗?cache 鎿嶄綔锛?  - 鎷嶇収鍓嶏細`SCB_CleanInvalidateDCache()`
  - 鎷嶇収鍚庯細`SCB_InvalidateDCache()`
- 鏍瑰洜鍒ゆ柇锛氭暣鐗?D-cache invalidate 鍦?DMA 鎷嶇収鍚庡彲鑳戒涪寮?CPU 灏氭湭鍐欏洖鐨勬爤銆丄I 鍙ユ焺銆佺姸鎬佸彉閲忔垨 UI 鏁版嵁缂撳瓨琛岋紝瀵艰嚧 `Capture done` 鍚庤繘鍏?AI 棰勫鐞嗘椂鍑虹幇璺戦/寮傚父銆傝椋庨櫓涓庘€滄媿鐓у畬鎴愬悗鍒氳繘鍏?AI 棰勫鐞嗗氨鏃犲悗缁棩蹇椻€濈殑鐜拌薄鍚诲悎銆?- 鍋氭渶灏忎慨澶嶏細灏?DCMIPP 鎷嶇収鍓嶅悗 cache 鎿嶄綔鏀剁獎涓哄彧 invalidate 鎽勫儚澶村抚缂撳啿鍖?`camera_date_buf`锛?  - 鏂板 `dcmipp_invalidate_camera_buffer_cache()`
  - 浣跨敤 `SCB_InvalidateDCache_by_Addr()`锛屾寜 32 瀛楄妭 cache line 瀵归綈鍦板潃鍜岄暱搴︺€?  - `dcmipp_start()` 鎷嶇収鍓嶅悗鍧囪皟鐢ㄨ鍑芥暟銆?  - `dcmipp_invalidate_frame_cache()` 涔熸敼涓哄彧澶勭悊鎽勫儚澶村抚缂撳啿鍖恒€?- 鏈涓嶆敼 DCMIPP 寮曡剼銆佹椂搴忋€佸抚缂撳啿鍦板潃銆佹憚鍍忓ご閰嶇疆銆丄I 妯″瀷銆丄I 鏉冮噸鎴?UI 甯冨眬銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 褰撳墠浜х墿锛?  - `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/28 22:17:26
  - `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/28 22:17:43
- 浠嶉渶鏉夸笂楠岃瘉锛?  - 涓嬭浇鍚庣‘璁?`[AI] Live preprocess done` 鏄惁鍑虹幇銆?  - 鑻ヨ兘缁х画鍒?`[AI] Live network run start`锛屽啀鍒ゆ柇鏄惁杩涘叆/鍗″湪 X-CUBE-AI 鎺ㄧ悊銆?  - 鑻ヤ粛璺戦锛岃褰?`[FAULT]` 杈撳嚭涓殑 CFSR/HFSR/MMFAR/BFAR銆?  - 濡傛灉 fast 宸ョ▼浠嶈繘鍏?AI 鍚庡紓甯革紝鍏堢敤 full camrun 鍏ㄩ噺鐑у綍涓€娆★紝纭澶栭儴 AI 鏉冮噸宸叉仮澶嶃€?
2026-04-28 22:12 鏇存柊锛?
- 閽堝鏉夸笂鍙嶉鈥滄斁涓婄墿鍝佸悗涓€鐩村崱鍦ㄧ瓑寰呴噸閲忕ǔ瀹氾紝涓嶅嚭鍥句篃涓嶅嚭缁撴灉鈥濊繘琛岃皟璇曘€?- 鍚庣画涓插彛琛ュ厖鏄剧ず瀹為檯娴佺▼宸蹭粠 `绛夊緟閲嶉噺绋冲畾` 杩涘叆 `閲嶉噺绋冲畾锛岃瘑鍒腑`锛屽苟瀹屾垚锛?  - `Capture start`
  - `Capture done`
  - `AI classify start`
- 鍥犳褰撳墠鍒ゆ柇锛氬師濮嬬幇璞′笉鍐嶆槸鍗″湪绉伴噸绋冲畾鐘舵€佹満锛岃€屾槸鍗″湪 `APP_AI_ClassifyRgb565Roi()` 鍐呴儴锛屽緟杩涗竴姝ュ尯鍒嗘槸 RGB565 ROI 棰勫鐞嗐€乣ai_network_run()` 鎺ㄧ悊锛岃繕鏄悗澶勭悊銆?- 涓洪檷浣?HX711 鎶栧姩瀵艰嚧绋冲畾鍒ゅ畾鍙嶅閲嶇疆鐨勬鐜囷紝`User\app\camera_workflow.c` 鍋氭渶灏忛槇鍊艰皟鏁达細
  - `CAMERA_STABLE_DELTA_G`锛歚10U` -> `30U`
  - `CAMERA_STABLE_HOLD_MS`锛歚50U` -> `300U`
- 涓哄畾浣?AI 鍐呴儴鍗＄偣锛宍User\app_x-cube-ai.c` 鍦ㄥ疄鏃?ROI 鎺ㄧ悊璺緞鏂板闃舵鏃ュ織锛屼笉鏀瑰彉杈撳叆銆佹ā鍨嬨€佹潈閲嶆垨鎺ㄧ悊娴佺▼锛?  - `[AI] Live preprocess start`
  - `[AI] Live preprocess done`
  - `[AI] Live network run start`
  - `[AI] Live network run done, res=...`
  - `[AI] Live postprocess start`
  - `[AI] Live postprocess done, res=...`
- 涓嬩竴姝ユ澘涓婂垽鎹細
  - 濡傛灉鍋滃湪 `[AI] Live preprocess start` 鍚庯紝浼樺厛鏌?ROI 鍙傛暟銆佸抚缂撳啿鍦板潃鍜?RGB565 璁块棶銆?  - 濡傛灉鍋滃湪 `[AI] Live network run start` 鍚庯紝浼樺厛鏌?AI 鏉冮噸鏄惁宸茬敱 full camrun 姝ｇ‘鍐欏叆澶栭儴 Flash銆乣0x90000000` 鏉冮噸婧愭暟鎹€乣0x70400000` HyperRAM 鏉冮噸鍓湰鍜?X-CUBE-AI 杩愯鏃跺唴瀛樸€?  - 濡傛灉鍑虹幇 `Live network run done, res=-1`锛屼紭鍏堢湅闅忓悗 `[AI] Error ...` 鐨?type/code銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 褰撳墠浜х墿锛?  - `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/28 22:11:44
  - `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/28 22:12:05
- 浠嶉渶鏉夸笂楠岃瘉锛氶噸鏂颁笅杞藉悗鎶撳彇浠?`[FLOW] AI classify start` 寮€濮嬬殑瀹屾暣涓插彛鏃ュ織锛屽挨鍏舵槸鏂板 `[AI] Live ...` 闃舵鏃ュ織锛涘鏋滀娇鐢?fast 宸ョ▼浠嶅崱鍦?AI network run锛屽厛鐢?full camrun 鍏ㄩ噺鐑у綍涓€娆℃仮澶嶅閮?AI 鏉冮噸銆?
2026-04-28 21:56 鏇存柊锛?
- 鎸夊浘4銆佸浘5瀹炵幇涓や釜纭绫诲脊绐楋細鍒犻櫎纭寮圭獥鍜屾竻绌虹‘璁ゅ脊绐椼€傛湰娆′笉瀹炵幇銆佷笉淇敼缁撹处瀹屾垚寮圭獥銆?- 涓や釜寮圭獥缁х画澶嶇敤 `checkout_ui` 鐨勫叏灞忓崐閫忔槑閬僵 `popup_mask`锛屽脊绐楁湰浣撲娇鐢ㄥ眳涓櫧鑹查潰鏉匡紝灏哄 `320x200`锛屽渾瑙?`12`銆?- `User\ui\checkout_ui.c/.h` 鏂板璐墿杞﹀姩浣滃洖璋冩敞鍐屾帴鍙ｏ細
  - `checkout_ui_set_cart_action_callbacks(delete_selected_cb, clear_cart_cb, user_data)`
- `checkout_ui.c` 鏂板纭寮圭獥鍐呴儴瀵硅薄鍙ユ焺锛?  - `g_checkout_ui_delete_confirm`
  - `g_checkout_ui_clear_confirm`
  涓よ€呭潎鍖呭惈闈㈡澘銆佹鑹查棶鍙峰浘鏍囥€佹爣棰樸€佹鏂囥€佸彇娑堟寜閽€佺‘瀹氭寜閽€?- 鍒犻櫎纭寮圭獥鍐呭锛?  - 鏍囬锛歚鍒犻櫎鍟嗗搧`
  - 姝ｆ枃锛歚纭鍒犻櫎褰撳墠閫変腑鍟嗗搧锛焋
  - 鍙栨秷鎸夐挳鍏抽棴寮圭獥銆?  - 纭畾鎸夐挳璋冪敤宸叉敞鍐屽垹闄ゅ洖璋冨悗鍏抽棴寮圭獥銆?- 娓呯┖纭寮圭獥鍐呭锛?  - 鏍囬锛歚娓呯┖璐墿杞
  - 姝ｆ枃锛歚纭娓呯┖鍏ㄩ儴鍟嗗搧锛焋
  - 鍙栨秷鎸夐挳鍏抽棴寮圭獥銆?  - 纭畾鎸夐挳璋冪敤宸叉敞鍐屾竻绌哄洖璋冨悗鍏抽棴寮圭獥銆?- 璐墿杞﹂〉涓師鏈夋寜閽粦瀹氫繚鎸佷负锛?  - 椤堕儴鈥滄竻绌衡€濆拰搴曢儴鈥滄竻绌鸿喘鐗╄溅鈥?-> `checkout_ui_show_clear_confirm_popup()`
  - 搴曢儴鈥滃垹闄ら€変腑椤光€?-> `checkout_ui_show_delete_confirm_popup()`
- `User\main.c` 娉ㄥ唽寮圭獥纭鍚庣殑涓氬姟鍥炶皟锛?  - 鍒犻櫎纭锛氬厛鎶?`checkout_ui_cart_get_selected_index()` 鍚屾鍒?`camera_workflow_select_cart_item()`锛屽啀璋冪敤 `camera_workflow_remove_selected_item()`锛岄殢鍚庡埛鏂颁富椤甸潰璐墿杞︽憳瑕佸拰璐墿杞﹂〉銆?  - 娓呯┖纭锛氳皟鐢?`camera_workflow_clear_cart()`锛岄殢鍚庡埛鏂颁富椤甸潰璐墿杞︽憳瑕佸拰璐墿杞﹂〉銆?- 鏈鍙敼纭寮圭獥鍜岃喘鐗╄溅鍒犻櫎/娓呯┖纭閾捐矾锛屼笉鏀?AI銆佺О閲嶃€佹憚鍍忓ご銆乻catter銆並eil 宸ョ▼澹虫垨缁撹处瀹屾垚寮圭獥銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)锛?  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun`
  - `F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast`
- 褰撳墠浜х墿锛?  - `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/28 21:56:29
  - `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/28 21:56:49
- 浠嶉渶鏉夸笂楠岃瘉锛氬垹闄ょ‘璁ゅ脊绐楀竷灞€涓庡浘4鏄惁涓€鑷淬€佹竻绌虹‘璁ゅ脊绐楀竷灞€涓庡浘5鏄惁涓€鑷淬€佸彇娑堟寜閽槸鍚﹀彧鍏抽棴寮圭獥銆佺‘瀹氬垹闄ゆ槸鍚﹀垹闄ゅ綋鍓嶉€変腑椤瑰苟鍒锋柊鍒楄〃/鎬讳环銆佺‘瀹氭竻绌烘槸鍚︽竻绌哄叏閮ㄥ晢鍝佸苟鍒锋柊涓婚〉闈笌璐墿杞﹂〉銆佹枃瀛楁槸鍚︽湁缂哄瓧鎴栨孩鍑恒€?
2026-04-25 22:48 鏇存柊锛?
- 鍩轰簬 `checkout_ui` 鍏叡妗嗘灦瀹炵幇鈥滆喘鐗╄溅绠＄悊椤碘€濓紙鍥?锛夛紝鏈涓嶅疄鐜板脊绐楀唴瀹癸紝涓嶉噸鍐欎富璇嗗埆椤垫暣浣撳竷灞€銆?- `User\ui\checkout_ui.c/.h` 鏂板璐墿杞﹂〉瀵硅薄鍜屾帴鍙ｏ細
  - `checkout_ui_cart_refresh(const cart_context_t *cart)`
  - `checkout_ui_cart_get_selected_index(void)`
- 璐墿杞﹂〉缁撴瀯锛?  - 椤堕儴鏍囬鏍忥細杩斿洖鎸夐挳銆佸眳涓€滆喘鐗╄溅鈥濄€佸彸渚р€滄竻绌衡€濇寜閽€?  - 缁熻鍗＄墖锛氬乏渚ц喘鐗╄溅鍥炬爣鍜屸€滃叡 N 绫诲晢鍝佲€濓紝鍙充晶鎬讳环閲戦锛屼繚鐣欎腑闂寸珫绾裤€?  - 鍒楄〃鍗＄墖锛氭敮鎸佺旱鍚戞粴鍔紝绌鸿喘鐗╄溅鏄剧ず鈥滆喘鐗╄溅涓虹┖鈥濓紱鏉＄洰鍖呭惈閫変腑鍦嗙偣銆佺缉鐣ュ浘鍗犱綅銆佸晢鍝佸悕銆佸崟浠枫€侀噸閲?鏁伴噺銆佸皬璁°€?  - 搴曢儴鎸夐挳锛氬垹闄ら€変腑椤广€佹竻绌鸿喘鐗╄溅銆佸幓缁撹处銆?- 浜や簰锛?  - 涓昏瘑鍒〉鈥滄煡鐪嬭喘鐗╄溅鈥濇寜閽繘鍏ヨ喘鐗╄溅椤碉紝浣嗕笉鏀瑰彉涓昏瘑鍒〉鏁翠綋甯冨眬銆?  - 杩斿洖鎸夐挳璋冪敤 `checkout_ui_show_main_page()`銆?  - 鐐瑰嚮鏉＄洰鍙仛 UI 灞傞€変腑/鍙栨秷閫変腑鍜屾祬钃濋珮浜紝涓嶇洿鎺ュ垹闄よ喘鐗╄溅鏁版嵁銆?  - 鍒犻櫎閫変腑椤硅皟鐢?`checkout_ui_show_delete_confirm_popup()` 绌哄３銆?  - 娓呯┖璐墿杞﹁皟鐢?`checkout_ui_show_clear_confirm_popup()` 绌哄３銆?  - 鍘荤粨璐﹁皟鐢?`checkout_ui_show_checkout_done_popup()` 绌哄３銆?- `User\main.c` 鍦ㄨ喘鐗╄溅鏁伴噺鎴栨€讳环鍙樺寲鏃惰皟鐢?`checkout_ui_cart_refresh(&state->cart)`锛屽彧鍚屾鏄剧ず鏁版嵁锛屼笉淇敼 AI銆佺О閲嶃€佽喘鐗╄溅涓氬姟閫昏緫銆?- 鏈浣跨敤宸叉湁 `checkout_icon_cart_summary_36` 鍜?`checkout_icon_basket_placeholder_72` 浣滀负缁熻/缂╃暐鍥惧崰浣嶏紝鏆傛湭鎺ュ叆鐪熷疄鍟嗗搧缂╃暐鍥俱€?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)銆?- 浠嶉渶鏉夸笂楠岃瘉锛氳喘鐗╄溅椤垫粴鍔ㄣ€佹潯鐩€変腑/鍙栨秷閫変腑楂樹寒銆佽繑鍥炰富椤甸潰銆佷笁涓簳閮ㄦ寜閽脊绐楃┖澹宠Е鍙戙€佹枃瀛?閲戦鏄惁婧㈠嚭銆?
2026-04-25 22:35 鏇存柊锛?
- 淇涓昏瘑鍒〉搴曢儴鍥涗釜鍥炬爣鎸夐挳涓枃瀛楀亸涓婄殑闂銆?- 鏍瑰洜鍒ゆ柇锛氬浘鏍囨寜閽唴閮ㄦ枃瀛?label 姝ゅ墠鍥哄畾涓?`126x28`锛孡VGL label 鏂囨湰鍦ㄨ鍥哄畾楂樺害鍐呬笉鏄瀭鐩村眳涓樉绀猴紝瀵艰嚧瑙嗚涓婂亸涓娿€?- `User\ui\checkout_ui.c` 鐨?`checkout_ui_create_icon_button()` 鏀逛负鍙浐瀹氭枃瀛?label 瀹藉害锛屼笉鍥哄畾楂樺害锛岃 label 浣跨敤瀛椾綋鑷韩楂樺害鍚庡啀閫氳繃 `LV_ALIGN_LEFT_MID` 鎸夋寜閽腑绾垮榻愩€?- 鏈鍙皟鏁存寜閽唴閮ㄦ枃瀛楀竷灞€锛屼笉淇敼鎸夐挳浣嶇疆銆佸昂瀵搞€佷簨浠跺洖璋冦€丄I 璇嗗埆銆佺О閲嶆垨璐墿杞﹂€昏緫銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)銆?- 浠嶉渶鏉夸笂楠岃瘉锛氬洓涓寜閽枃瀛楁槸鍚﹀凡涓婁笅灞呬腑锛屼笖闀挎枃瀛椻€滄煡鐪嬭喘鐗╄溅鈥濃€滄竻绌鸿喘鐗╄溅鈥濇湭琚浘鏍囨尋鍑烘垨瑁佸垏銆?
2026-04-25 22:25 鏇存柊锛?
- 灏嗗凡楠岃瘉鍙敤鐨?LVGL v8 鍥炬爣璧勬簮鎺ュ叆鏂?`checkout_ui` 涓昏瘑鍒粨璐﹂〉銆?- 浠?`E:\H7R3\iconfont\lvgl_v8_out\c_array` 澶嶅埗鍒板伐绋嬶細
  - `User\ui\checkout_icons.c`
  - `User\ui\checkout_icons.h`
- 涓や釜 Keil 宸ョ▼ `User/UI` 鍒嗙粍鍧囨柊澧炵紪璇?`User\ui\checkout_icons.c`锛?  - `Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx`
  - `Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx`
- `User\ui\checkout_ui.c/.h` 灞€閮ㄦ帴鍏ュ浘鏍囷紝涓嶄慨鏀?AI 璇嗗埆銆佺О閲嶃€佽喘鐗╄溅涓氬姟閫昏緫锛屼篃涓嶉噸鍐欎富椤甸潰缁撴瀯銆?- 鍥炬爣瀵瑰簲鍏崇郴锛?  - 椤堕儴鐘舵€佹爮鍙充晶锛歚checkout_icon_scale_header_32`
  - 褰撳墠閲嶉噺淇℃伅锛歚checkout_icon_scale_info_36`
  - 璇嗗埆鑰楁椂淇℃伅锛歚checkout_icon_time_info_36`
  - 鍥惧儚鏄剧ず鍗犱綅锛歚checkout_icon_basket_placeholder_72`
  - 璐墿杞︽憳瑕佸渾褰㈠浘鏍囷細`checkout_icon_cart_summary_36`
  - 鏌ョ湅璐墿杞︽寜閽細`checkout_icon_cart_button_34`
  - 缁撹处鎸夐挳锛歚checkout_icon_money_button_34`
  - 娓呯┖璐墿杞︽寜閽細`checkout_icon_trash_button_34`
  - 缁х画璐墿鎸夐挳锛歚checkout_icon_refresh_button_34`
- 鎸夐挳鍐呴儴鏀逛负鍥哄畾鐨勨€滃乏渚у師濮嬪昂瀵稿浘鏍?+ 鍙充晶鏂囧瓧鈥濆竷灞€锛岄伩鍏嶅浘鏍囨妸鏂囧瓧鎸ゅ嚭 215x60 鎸夐挳銆?- 鍥惧儚鏄剧ず鍖哄煙浠嶄繚鐣欌€滃浘鍍忔樉绀哄尯鍩熲€濆崰浣嶆枃瀛楋紝鍙湪鍏朵笂鏂瑰鍔犵瀛愬浘鏍囷紱璇嗗埆鎷嶇収鍚庝粛鐢?`preview_image` 鏄剧ず鐩告満甯с€?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)銆?- 浠嶉渶鏉夸笂楠岃瘉锛氫富椤甸潰鍥炬爣棰滆壊/閫忔槑搴︽槸鍚︾鍚堥鏈燂紝鍥涗釜鎸夐挳鏂囧瓧鏄惁瀹屽叏鏄剧ず锛屽浘鍍忓崰浣嶅浘鏍囧拰鎷嶇収棰勮鍒囨崲鏄惁姝ｅ父銆?
2026-04-25 17:26 鏇存柊锛?
- 閽堝鏉夸笂鍙嶉鈥滆瘑鍒垚鍔熷悗鍥剧墖鏄剧ず鍖哄煙骞舵病鏈夊浘鍍忊€濓紝瀵规柊 `checkout_ui` 涓昏瘑鍒〉鍋氭渶灏?UI 鍒锋柊淇銆?- 鏍瑰洜鍒ゆ柇锛氭媿鐓у畬鎴愬悗鐨?`capture_counter`銆佸抚缂撳啿鍜屽昂瀵稿凡缁忎粠 `User\main.c` 浼犲叆 `checkout_ui_main_set_image_source()`锛屼絾鏂?UI 鍦ㄨ缃?`lv_img` 婧愬悗鍙垏鎹㈤殣钘忕姸鎬侊紝缂哄皯瀵瑰浘鐗囧璞″墠鍚庡眰绾у拰鍥惧儚妗嗛噸缁樼殑鏄惧紡澶勭悊锛屽彲鑳藉鑷村崰浣嶅眰/鏃х粯鍒跺尯鍩熸病鏈夎鍒锋柊鍑虹浉鏈哄抚銆?- `User\ui\checkout_ui.c` 鐨?`checkout_ui_main_set_image_source()` 鍦ㄨ缃浘鐗囨簮銆佺缉鏀惧拰灞呬腑鍚庯紝鏂板锛?  - `lv_obj_move_foreground(g_checkout_ui_main.preview_image)`
  - `lv_obj_invalidate(g_checkout_ui_main.image_box)`
- 鏈涓嶄慨鏀规憚鍍忓ご閲囬泦銆丏CMIPP銆乧ache銆乻catter銆並eil 宸ョ▼鍜?AI 閫昏緫锛屽彧澶勭悊 LVGL 鍥剧墖鏄剧ず鍒锋柊璺緞銆?- 宸插湪鏈満閲嶆柊鏋勫缓 full camrun 鍜?camrun_fast锛屽潎涓?0 Error(s), 0 Warning(s)銆?- 浠嶉渶鏉夸笂楠岃瘉锛氳瘑鍒垚鍔熷悗鍥惧儚妗嗘槸鍚︽樉绀烘湰娆℃媿鐓х敾闈紱鍟嗗搧鍙栬蛋鍥為浂鍚庯紝璇嗗埆缁撴灉鍖哄煙鏄惁浠嶆寜棰勬湡娓呯┖銆?
2026-04-25 16:49 鏇存柊锛?
- 淇涓昏瘑鍒〉鈥滃浘鍍忔樉绀哄尯鍩熸病鏄剧ず鐩告満鍥惧儚鈥濈殑闂銆?- 鏍瑰洜锛氭柊 `checkout_ui` 涓婚〉闈㈠彧鏈夊浘鍍忓崰浣嶆鍜屽崰浣嶆枃瀛楋紝娌℃湁鍒涘缓 `lv_img`锛屼篃娌℃湁鎶?`camera_workflow_get_frame_buffer()` 鎺ュ埌鏂?UI銆?- `checkout_ui_main_page_handles_t` 鏂板 `preview_image` 鍙ユ焺銆?- `checkout_ui.c/.h` 鏂板锛?  - `checkout_ui_main_set_image_source(width, height, pixels)`
  - `checkout_ui_main_show_image(visible)`
- `User\main.c` 鐨?`main_refresh_checkout_ui()` 鏂板 `capture_counter` 缂撳瓨锛歚capture_counter == 0` 鏃舵樉绀哄崰浣嶆枃瀛楋紱鎷嶇収瀹屾垚骞朵笖 `capture_counter` 鍙樺寲鍚庯紝浣跨敤 `camera_workflow_get_frame_buffer()` 鍒锋柊 LVGL 鍥剧墖銆?- 鍥剧墖缂╂斁娌跨敤鏃?UI 鎬濊矾锛屾寜鍥惧儚妗嗗昂瀵歌绠?zoom锛屼繚鎸佸畬鏁村浘鍍忔樉绀哄湪鍥惧儚妗嗗唴銆?
2026-04-25 16:43 鏇存柊锛?
- 淇鏂颁富璇嗗埆椤甸噸閲忎竴鐩存樉绀?0 鐨勯棶棰樸€?- 鏍瑰洜锛氫笂涓€杞彧瀹屾垚浜嗛潤鎬佷富椤甸潰锛宍main` 涓诲惊鐜腑娌℃湁鎶?`camera_workflow_process()` 鏇存柊鍚庣殑 `camera_state` 鍒锋柊鍒版柊 `checkout_ui`銆?- `User\main.c` 鏂板 `main_refresh_checkout_ui()`锛屽彧鍋?UI 鏄剧ず妗ユ帴锛屼笉淇敼璇嗗埆鐘舵€佹満銆佺О閲嶉€昏緫鍜岃喘鐗╄溅閫昏緫銆?- 褰撳墠鍒锋柊鍐呭锛?  - 椤堕儴鐘舵€佹爮锛氭潵鑷?`camera_state.status_text`
  - 褰撳墠閲嶉噺锛氭潵鑷?`camera_state.weight_gram`
  - 璇嗗埆鑰楁椂锛氭潵鑷?`camera_state.recognition_time_valid / recognition_time_ms_x10`
  - 鏈璇嗗埆缁撴灉锛氭潵鑷?`camera_state.last_cart_add`
  - 璐墿杞︽憳瑕侊細鏉ヨ嚜 `camera_state.cart.item_count`
  - 鎬讳环锛氭潵鑷?`camera_state.cart.total_price_cent`
- 鍒锋柊鍑芥暟鍐呴儴鍋氫簡缂撳瓨锛屽彧鏈夋暟鎹彉鍖栨椂鎵嶈皟鐢?LVGL label 鏇存柊锛岄伩鍏?10ms 涓诲惊鐜腑鍙嶅鍒嗛厤/鍒锋柊閫犳垚涓嶇ǔ瀹氥€?- 涓洪伩鍏嶇姸鎬佹爮缂哄瓧锛屽凡鎶?`妫€娴嬪埌鐗╁搧`銆乣璇嗗埆鎴愬姛锛岃鍙栬蛋鍟嗗搧`銆乣璇嗗埆澶辫触锛岃鍙栬蛋閲嶈瘯`銆乣璇嗗埆瀹屾垚` 鍔犲叆 `checkout_ui_font_texts.txt` 骞堕噸鏂扮敓鎴?`Font20.c/.h`銆?- 鐢ㄦ埛鍙嶉鈥滃啀娆′笅鏉挎姤閿欑枒浼肩▼搴忚窇椋炩€濓紝褰撳墠灏氭湭鎷垮埌 Keil 鍘熷 Load/Debug 鎶ラ敊銆傝嫢浠嶅鐜帮紝涓嬩竴姝ュ繀椤诲厛璁板綍瀹屾暣鎶ラ敊锛屽啀鍖哄垎鏄儳褰曡繛鎺ラ棶棰樸€佽繍琛?HardFault銆佸閮?Flash/AI 鏉冮噸闂锛岃繕鏄?UI/LVGL 杩愯闂銆?
2026-04-24 18:47 鏇存柊锛?
- 鍩轰簬鏂扮殑 `checkout_ui` 鍏叡妗嗘灦锛屽凡瀹屾垚鍥?鈥滀富璇嗗埆缁撹处椤碘€濈殑闈欐€佸竷灞€銆?- 鏈鍙疄鐜颁富璇嗗埆缁撹处椤碉紝娌℃湁瀹炵幇璐墿杞︾鐞嗛〉鍜屽叾浠栧脊绐楀唴瀹广€?- 涓婚〉闈㈡寜 480x800 绔栧睆鍒嗕负 6 涓尯鍩燂細椤堕儴鐘舵€佹爮銆侀噸閲?璇嗗埆鑰楁椂鍗＄墖銆佸浘鍍忔樉绀哄尯鍩熴€佹湰娆¤瘑鍒粨鏋滃崱鐗囥€佽喘鐗╄溅鎽樿/鎬讳环鍗＄墖銆佸簳閮ㄥ洓鎸夐挳鍖哄煙銆?- 搴曢儴鎸夐挳宸插垱寤猴細鏌ョ湅璐墿杞︺€佺粨璐︺€佹竻绌鸿喘鐗╄溅銆佺户缁喘鐗╋紱浜嬩欢鐩墠鍙帴绌哄３鍑芥暟锛屾湭鑱斿姩涓氬姟銆?- `checkout_ui.h` 鏂板涓婚〉闈㈠璞″彞鏌?`checkout_ui_main_page_handles_t`銆?- 淇濈暀鐨勪富椤甸潰鍒锋柊鎺ュ彛锛?  - `checkout_ui_main_set_status`
  - `checkout_ui_main_set_weight_gram`
  - `checkout_ui_main_set_recognition_time`
  - `checkout_ui_main_set_result_state`
  - `checkout_ui_main_set_result`
  - `checkout_ui_main_clear_result`
  - `checkout_ui_main_set_cart_summary`
  - `checkout_ui_main_set_total_price`
- 涓洪伩鍏?`kg` 鍜?`s` 缂哄瓧锛屽凡鎶?`0.000 kg`銆乣--.-- s` 鍔犲叆 `checkout_ui_font_texts.txt` 骞堕噸鏂扮敓鎴?`Font20.c/.h`銆?
2026-04-24 18:40 鏇存柊锛?
- 褰撳墠鍚姩鍏ュ彛宸茬粡浠庢棫 `ui_checkout_main` 椤甸潰鍒囨崲鍒版柊鐨?`checkout_ui` 鍏叡妗嗘灦銆?- `User\main.c` 鐜板湪鍖呭惈 `ui/checkout_ui.h`锛屽垵濮嬪寲 LVGL 鍚庤皟鐢?`checkout_ui_framework_init()`銆?- 涓诲惊鐜腑涓嶅啀璋冪敤鏃х殑 `ui_checkout_main_refresh()`銆?- 涓や釜 Keil 宸ョ▼褰撳墠鍙紪璇?`User\ui\checkout_ui.c`锛屼笉鍐嶇紪璇戞棫鐨?`ui_checkout_main.c` 鍜?`ui_dialog.c`銆?- 鏃ф簮鐮佹枃浠舵殏鏃朵繚鐣欏湪鐩綍涓紝浣滀负杩佺Щ鍙傝€冿紱褰撳墠涓嶅弬涓庣紪璇戙€?- 鏂版鏋剁洰鍓嶅彧寤虹珛 480x800 绔栧睆鏍瑰璞°€佷富璇嗗埆椤点€佽喘鐗╄溅椤点€佸脊绐楅伄缃╁拰 4 涓脊绐楀簳鏉裤€?- 鏂版鏋跺厛鍙斁鍩虹鏍囬鏍忓拰鍗犱綅鍗＄墖锛屼笉鍋氳瘑鍒€佽喘鐗╄溅銆佺姸鎬佹満涓氬姟鑱斿姩銆?
鏃?UI 鏇剧粡宸叉敼涓轰腑鏂囩晫闈紝浣嗗綋鍓嶄笉鍐嶄綔涓哄惎鍔ㄩ〉闈€?
涓昏 UI 鏂囦欢锛?
- E:\H7R3\鑷姪缁撹处\User\ui\checkout_ui.c
- E:\H7R3\鑷姪缁撹处\User\ui\checkout_ui.h
- E:\H7R3\鑷姪缁撹处\User\ui\ui_checkout_main.c
- E:\H7R3\鑷姪缁撹处\User\ui\ui_checkout_main.h
- E:\H7R3\鑷姪缁撹处\User\ui\ui_dialog.c
- E:\H7R3\鑷姪缁撹处\User\ui\lvgl_port.c

褰撳墠 UI 鐗圭偣锛?
- 480x800 绔栧睆甯冨眬
- 鐘舵€併€佽瘑鍒粨鏋溿€佽喘鐗╄溅銆佹寜閽€佸脊绐椼€佸晢鍝佸悕鍧囦负涓枃
- 鍗曚綅鏄剧ず涓轰腑鏂囬鏍硷紝渚嬪 鍗冨厠銆佺銆佸厓/鍗冨厠銆佸厓/鐡?- 寮圭獥鏂囧瓧鍖呮嫭 璇嗗埆澶辫触銆佺粨璐﹀畬鎴愩€佺‘瀹?绛?
娉ㄦ剰锛氫笂闈腑鏂囧寲鍐呭涓昏鏉ヨ嚜鏃?UI 鏂囦欢锛屽悗缁柊椤甸潰搴斾紭鍏堝湪 `checkout_ui.c/.h` 鍩虹涓婇€愭閲嶅缓锛屼笉瑕佺洿鎺ユ仮澶嶆棫椤甸潰鍏ュ彛銆?
瀛椾綋鐘舵€侊細

- 涓枃 LVGL 鏍囩褰撳墠浣跨敤 `Font20`
- UI 浠ｇ爜涓娇鐢?`LV_FONT_DECLARE(Font20)`
- `Font20.c` 鐢变互涓嬫枃浠剁敓鎴愶細
  - E:\H7R3\鑷姪缁撹处\Middlewares\LVGL\Font\checkout_ui_font_texts.txt
  - E:\H7R3\鑷姪缁撹处\tools\generate_checkout_ui_font.py
- 褰撳墠鐢熸垚杈撳嚭锛?  - E:\H7R3\鑷姪缁撹处\Middlewares\LVGL\Font\Font20.c
  - E:\H7R3\鑷姪缁撹处\Middlewares\LVGL\Font\Font20.h

褰撳墠鍋氭硶鍙傝€冧簡 ALIENTEK LVGL 缁煎悎渚嬬▼锛氫腑鏂囨帶浠剁洿鎺ユ寕 `Font20`銆?
瀛椾綋娉ㄦ剰浜嬮」锛?
- 鏃ф枃浠?`checkout_ui_font_20.c` 鍜?`checkout_ui_font_20.h` 鍙兘浠嶅湪纾佺洏涓婏紝浣嗗綋鍓?Keil 宸ョ▼涓嶅啀缂栬瘧瀹冧滑銆?- 褰撳墠 Keil 宸ョ▼缂栬瘧鐨勬槸 `Font20.c`銆?- 濡傛灉鏂板涓枃 UI 鏂囨锛屽繀椤诲厛鏇存柊 `checkout_ui_font_texts.txt`锛屽啀閲嶆柊鐢熸垚 `Font20.c`锛岀劧鍚庨噸缂栦袱涓?Keil 宸ョ▼锛屾渶鍚庢洿鏂版湰 readme銆?
鏈€杩戠殑瀛椾綋鏄剧ず闂锛?
- `鍒犻櫎閫変腑椤筦 鏇惧嚭鐜板嚑涓瓧涓婁笅涓嶉綈銆?- 瀛椾綋鐢熸垚鑴氭湰宸叉敼鎴愬弬鑰?LVGL 渚嬬▼鐨勮楂樺拰鍩虹嚎椋庢牸锛歚line_height = 20`锛宍base_line = 3`銆?- 璇ユ樉绀烘晥鏋滀粛闇€瑕佷笅杞藉埌鏉垮瓙鍚庣‘璁ゃ€?

7. 瀛椾綋鐢熸垚

瀛椾綋鐢熸垚鑴氭湰锛?
- E:\H7R3\鑷姪缁撹处\tools\generate_checkout_ui_font.py

榛樿鐢熸垚鍛戒护锛?
```text
python tools\generate_checkout_ui_font.py
```

榛樿杈撳嚭锛?
- `Middlewares\LVGL\Font\Font20.c`
- `Middlewares\LVGL\Font\Font20.h`

褰撳墠鑴氭湰涓殑婧愬瓧浣擄細

- `C:\Windows\Fonts\simhei.ttf`

濡傛灉鏉夸笂涓枃瑙嗚瀵归綈浠嶇劧涓嶇悊鎯筹紝涓嬩竴姝ュ彲浠ュ皾璇曟洿鎹㈡簮瀛椾綋锛屼緥濡傦細

- `simsunb.ttf`
- `simsun.ttc`
- `NotoSansSC-VF.ttf`

鏇存崲婧愬瓧浣撳悗闇€瑕侀噸鏂扮敓鎴愩€侀噸鏂版瀯寤猴紝骞跺湪鏉垮瓙涓婂姣旀樉绀烘晥鏋溿€?

8. 蹇笅鏉夸笌 AI 鏉冮噸

AI 鏉冮噸寰堝ぇ锛屽瓨鏀惧湪澶栭儴 Flash 涓€?
鐩稿叧鏂囦欢锛?
- E:\H7R3\鑷姪缁撹处\network_data.bin
- E:\H7R3\鑷姪缁撹处\User\network_data.c
- E:\H7R3\鑷姪缁撹处\User\network_data_params.c
- E:\H7R3\鑷姪缁撹处\User\network_weights_bin_fast.s

蹇笅鏉垮師鐞嗭細

- full 宸ョ▼浼氭妸 AI 鏉冮噸鍐欏叆澶栭儴 Flash銆?- fast 宸ョ▼鐢ㄤ竴涓緢灏忕殑 stub 瀵硅薄浠ｆ浛澶ф潈閲嶅璞★紝閬垮厤姣忔閲嶅涓嬭浇 AI 鏉冮噸銆?- 鎺ㄧ悊閫熷害鐞嗚涓婁笉鍙橈紝鍥犱负杩愯鏃朵粛浠庡厛鍓嶅啓鍏ョ殑澶栭儴 Flash 鏉冮噸鍦板潃璇诲彇銆?
浣跨敤瑙勫垯锛?
- AI 妯″瀷鎴栨潈閲嶅彉鍖栧悗锛屽厛鐢?full camrun 鍏ㄩ噺鐑у綍涓€娆°€?- 涔嬪悗鏅€氬浐浠惰凯浠ｅ彲浠ョ敤 camrun_fast銆?- 濡傛灉 fast 宸ョ▼涓嬫澘鍚庤瘑鍒け璐ワ紝浼樺厛纭澶栭儴 Flash 涓槸鍚﹀凡缁忔湁姝ｇ‘ AI 鏉冮噸銆?

9. 鏂囦欢娓呯悊鍘嗗彶

涔嬪墠宸插垹闄わ細

- 鏃х殑 `atk_h7r3_full.uvprojx`
- 鏃х殑 `atk_h7r3_full.uvoptx`
- 鏃х殑 `atk_h7r3_full.uvguix.allyh`
- `Middlewares\LVGL\Image` 鏁翠釜鐩綍
- `Middlewares\LVGL\Font\Font60.c`

鏈夋剰淇濈暀锛?
- `Drivers\BSP\KEY\key.c`锛屽悗缁彲鑳戒娇鐢ㄣ€?- LVGL QR 鐩稿叧鏂囦欢锛屽悗缁彲鑳藉仛浜岀淮鐮佽瘑鍒垨浜岀淮鐮佺晫闈€?- 鍘嗗彶 scatter 鏂囦欢锛屼綔涓烘帓闅滃拰瀵圭収鏉愭枡銆?
涓嶈鍦ㄦ病鏈夌‘璁ゅ紩鐢ㄥ叧绯诲拰鍚庣画鍔熻兘鎰忓浘鐨勬儏鍐典笅缁х画鍒犻櫎鏂囦欢銆?

10. 鏋勫缓楠岃瘉璁板綍

杩戞湡浣跨敤杩囩殑 Keil 鍛戒护琛屾瀯寤烘柟寮忥細

```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
```

杩戞湡瑙傚療鍒扮殑鏋勫缓浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`
- `Output\atk_h7r3_full_camrun_fast.axf`

2026-04-24 18:40 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/24 18:40:25
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/24 18:40:25

2026-04-24 18:47 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/24 18:47:17
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/24 18:47:17

2026-04-25 16:43 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/25 16:43:08
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/25 16:43:08

2026-04-25 16:49 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/25 16:49:27
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/25 16:49:27

2026-04-25 17:26 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/25 17:26:41
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/25 17:26:46

2026-04-25 22:25 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/25 22:25:35
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/25 22:25:35

2026-04-25 22:35 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/25 22:35:26
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/25 22:35:26

2026-04-25 22:48 楠岃瘉锛?
```text
F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun
缁撴灉锛? Error(s), 0 Warning(s)

F:\Keil5\UV4\UV4.exe -j0 -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast
缁撴灉锛? Error(s), 0 Warning(s)
```

褰撳墠浜х墿锛?
- `Output\atk_h7r3_full_camrun.axf`锛屾椂闂?2026/4/25 22:48:08
- `Output\atk_h7r3_full_camrun_fast.axf`锛屾椂闂?2026/4/25 22:48:08

娉ㄦ剰锛?
- 浠?PowerShell 璋?Keil GUI 绋嬪簭鏃讹紝杩涚▼杩斿洖琛屼负鍙兘姣旇緝濂囨€€傞獙璇佸懡浠よ鏋勫缓鏃讹紝浼樺厛浣跨敤 `Start-Process -Wait`銆?- PC 涓婃瀯寤洪€氳繃涓嶇瓑浜庣‖浠跺姛鑳藉叏閮ㄦ甯搞€傛憚鍍忓ご銆佽Е鎽搞€丩VGL 鏃跺簭銆佺О閲嶇ǔ瀹氥€丄I 鏉冮噸鍐呭閮介渶瑕佹澘涓婇獙璇併€?

11. 鏂?AI 瀵硅瘽浼樺厛闃呰鏂囦欢

鏂板璇濆缓璁厛璇伙細

- E:\H7R3\鑷姪缁撹处\AGENTS.md
- E:\H7R3\鑷姪缁撹处\.agents\skills\embedded-project\SKILL.md
- E:\H7R3\鑷姪缁撹处\readme.txt
- E:\H7R3\鑷姪缁撹处\Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx
- E:\H7R3\鑷姪缁撹处\Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx
- E:\H7R3\鑷姪缁撹处\User\Script\ATK-DNH7R3_flash_ROMxspi1_camrun.sct
- E:\H7R3\鑷姪缁撹处\User\Script\ATK-DNH7R3_flash_ROMxspi1_camrun_fast.sct
- E:\H7R3\鑷姪缁撹处\User\app\camera_workflow.c
- E:\H7R3\鑷姪缁撹处\User\app\cart_service.c
- E:\H7R3\鑷姪缁撹处\User\app\product_catalog.c
- E:\H7R3\鑷姪缁撹处\User\ui\ui_checkout_main.c
- E:\H7R3\鑷姪缁撹处\User\ui\ui_dialog.c
- E:\H7R3\鑷姪缁撹处\User\ui\lvgl_port.c
- E:\H7R3\鑷姪缁撹处\Middlewares\LVGL\Font\checkout_ui_font_texts.txt
- E:\H7R3\鑷姪缁撹处\tools\generate_checkout_ui_font.py

濡傛灉鍚庣画閲嶆柊璋冩憚鍍忓ご锛屽彲缁х画鍙傝€冩棫鐨勫凡鐭ュ鐓у伐绋嬶細

- E:\H7R3\鑷姪缁撹处3


12. 寤鸿涓嬩竴姝?
褰撳墠寤鸿浼樺厛妫€鏌ワ細

1. 涓嬭浇鏈€鏂?`camrun_fast` 鍒版澘瀛愪笂锛屾鏌ヤ腑鏂?UI 鏄剧ず锛屽挨鍏舵槸 `鍒犻櫎閫変腑椤筦銆?2. 楠岃瘉鎽勫儚澶存媿鐓с€佽瘑鍒€侀噸閲忚Е鍙戙€佸姞鍏ヨ喘鐗╄溅銆侀€変腑/鍙栨秷閫変腑銆佸垹闄ら€変腑椤广€佹竻绌鸿喘鐗╄溅鍜岀粨璐﹀脊绐椼€?3. 濡傛灉涓枃瀛椾綋浠嶇劧涓嶉綈锛屽皾璇曟洿鎹㈡簮瀛椾綋閲嶆柊鐢熸垚 `Font20.c` 骞舵澘涓婂姣斻€?4. 濡傛灉 fast 宸ョ▼璇嗗埆澶辫触锛屽厛鐢?full `camrun` 宸ョ▼鍏ㄩ噺鐑у綍涓€娆★紝鎭㈠澶栭儴 AI 鏉冮噸銆?

13. 鏂板璇濆彲鐢ㄦ彁绀鸿瘝

鏂板紑 AI 瀵硅瘽鏃跺彲浠ョ洿鎺ヨ锛?
```text
璇峰厛闃呰 E:\H7R3\鑷姪缁撹处\AGENTS.md 鍜?E:\H7R3\鑷姪缁撹处\readme.txt銆?閬靛畧 .agents\skills\embedded-project 瑙勫垯銆?缁х画褰撳墠 STM32H7R3 鑷姪缁撹处椤圭洰锛屽綋鍓嶄富绾挎槸 camrun / camrun_fast銆?浠ュ悗姣忔鏈夊疄璐ㄦ敼鍔紝閮借鍦ㄧ粨鏉熷墠鏇存柊 readme.txt銆?璇风壒鍒皬蹇?Keil 宸ョ▼鏂囦欢銆乻catter銆佹憚鍍忓ご鍐呭瓨甯冨眬銆丩VGL Font20 涓枃 UI銆丄I 鏉冮噸鍜岀‖浠堕獙璇併€?```

2026-05-16 鏇存柊锛氬洖閫€ LVGL 鍐呭瓨姹犲ぇ灏忔帓鏌?LCD 涓嶄寒

- 鐢ㄦ埛鍙嶉锛氳繘鍏?Keil Debug 鍚庤繍琛屾寜閽伆銆侀殢鍚庨渶瑕侀獙璇?LCD 涓嶄寒鏄惁涓庝笂涓€娆?`LV_MEM_SIZE` 璋冩暣鏈夊叧銆?
- 鏈浠呰皟鏁?`Middlewares\LVGL\GUI\lvgl\lv_conf.h`锛?
  - 灏?`LV_MEM_SIZE` 浠?`(96U * 1024U)` 鏀瑰洖鍘熸潵鐨?`(48U * 1024U)`銆?
- 鐩殑锛氬厛鎭㈠鍒板師濮?LVGL 鍐呭瓨姹犻厤缃紝鎺掗櫎鍐呭瓨姹犳墿澶у鑷寸殑鍚姩/鏄剧ず寮傚父銆?
- 鏈涓嶆敼鏀粯娴佺▼銆佷笉鏀规憚鍍忓ご鍒濆鍖栥€佷笉鏀?scatter銆佷笉鏀?Keil 宸ョ▼閰嶇疆銆?
- 鍚庣画鏉夸笂楠岃瘉锛?
  - 閲嶆柊缂栬瘧骞朵笅杞藉悗锛屽厛纭 LCD 鑳屽厜鏄惁鎭㈠銆佷覆鍙ｆ槸鍚﹁緭鍑哄惎鍔ㄦ棩蹇椼€?
  - 濡傛灉 LCD 鑳戒寒浣嗗啀娆″崱鍦?UI 鍒濆鍖栵紝鍐嶇户缁畾浣?LVGL 瀵硅薄鍒涘缓/鍐呭瓨涓嶈冻闂銆?
  - 濡傛灉 LCD 浠嶄笉浜笖涓插彛鏃犺緭鍑猴紝浼樺厛妫€鏌?Keil Debug/Download 鐘舵€併€丅OOT 寮曡剼銆佷緵鐢靛拰 ST-Link 杩炴帴銆?

2026-05-16 鏇存柊锛歀CD 鍗″湪鎽勫儚澶村垵濮嬪寲鐢婚潰鐨勬洿绋冲Ε淇鏂规

- 鐢ㄦ埛鏉夸笂鍙嶉锛歚LV_MEM_SIZE` 鏀瑰洖 48K 鍚庯紝涓插彛鎭㈠鍒?`[FLOW] -> 璇锋斁缃晢鍝乣 鍜?`CTP ID:911`锛孡CD 鑳屽厜涔熸仮澶嶏紝浣嗙晫闈粛鍋滅暀鍦ㄦ憚鍍忓ご鍒濆鍖栫敾闈€?- 鏈淇濈暀 `Middlewares\LVGL\GUI\lvgl\lv_conf.h` 涓?`LV_MEM_SIZE = (48U * 1024U)`锛屼笉鍐嶆墿澶?LVGL 鍐呭瓨姹狅紝閬垮厤鍐嶆褰卞搷鍚姩/璋冭瘯琛屼负銆?- 璋冩暣 `User\ui\checkout_ui.c`锛?  - `checkout_ui_framework_init()` 鍚姩鏃跺彧鍒涘缓 root銆佷富璇嗗埆椤点€侀〉闈㈠鍣ㄥ拰 popup mask銆?  - 璐墿杞﹂〉銆佹壂鐮佹敮浠橀〉銆佹敮浠樻垚鍔熼〉銆佽瘑鍒け璐ュ脊绐椼€佹棤鏁堜粯娆剧爜寮圭獥銆佹敮浠樿秴鏃跺脊绐椼€佸垹闄?娓呯┖纭寮圭獥銆佹棫缁撹处瀹屾垚寮圭獥鏀逛负鎸夐渶鍒涘缓銆?  - `checkout_ui_show_cart_page()`銆乣checkout_ui_show_payment_page()`銆乣checkout_ui_show_payment_success_page()` 浠ュ強鍚勫脊绐楁樉绀哄叆鍙ｄ細鍏?ensure 瀵瑰簲椤甸潰/寮圭獥宸插垱寤恒€?- 鐩殑锛氬噺灏戝惎鍔ㄩ樁娈典竴娆℃€у垱寤哄ぇ閲?LVGL 瀵硅薄瀵艰嚧鐨?48K 鍐呭瓨姹犲帇鍔涳紝閬垮厤 `checkout_ui_framework_init()` 涓€斿垎閰嶅け璐ュ悗涓荤晫闈㈡病鏈夌湡姝ｅ姞杞斤紝琛ㄧ幇涓?LCD 鍋滃湪鎽勫儚澶村垵濮嬪寲鐢婚潰銆?- 鍚屾淇浜?`checkout_ui.c` 涓嫢骞茬敤鎴峰彲瑙佷腑鏂囧瓧绗︿覆鐨勪贡鐮佹畫鐣欙紝閬垮厤璐墿杞︺€佹壂鐮佹敮浠橀〉銆佹敮浠樻垚鍔熼〉鍜屾棫寮圭獥鏄剧ず涔辩爜銆?- 鏈涓嶆敼鎽勫儚澶村垵濮嬪寲銆丏CMIPP銆丠yperRAM銆乻catter銆並eil 宸ョ▼閰嶇疆銆丄I 鏉冮噸鍜屾敮浠樹笟鍔＄姸鎬佹満銆?- 缂栬瘧楠岃瘉锛?  - `F:\Keil5\UV4\UV4.exe -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun -j0`
  - 鏃ュ織锛歚Projects\MDK-ARM\checkout_ui_lazy_build_final.log`
  - 缁撴灉锛? Error(s), 0 Warning(s)銆?- 闇€瑕佹澘涓婇獙璇侊細
  - 涓嬭浇鍚庡惎鍔紝涓插彛鍒?`CTP ID:911` 鍚庯紝LCD 搴斾粠鎽勫儚澶村垵濮嬪寲鐢婚潰鍒囧埌涓昏瘑鍒粨璐﹂〉銆?  - 涓昏瘑鍒〉鐘舵€佸簲鏄剧ず鈥滆鏀剧疆鍟嗗搧鈥濄€?  - 绌鸿喘鐗╄溅鐐瑰嚮缁撹处搴斿脊鍑烘棤娉曠粨璐︽彁绀恒€?  - 鍔犲晢鍝佸悗杩涘叆璐墿杞﹂〉銆佹壂鐮佹敮浠橀〉銆佹敮浠樻垚鍔熼〉锛岀‘璁よ繖浜涢〉闈㈤娆℃墦寮€鏃惰兘姝ｅ父鍒涘缓鍜屾樉绀恒€?  - 鏀粯鍙栨秷杩斿洖鍚庤喘鐗╄溅涓嶆竻绌猴紱鏀粯鎴愬姛杩斿洖棣栭〉鍚庤喘鐗╄溅娓呯┖銆佹€讳环褰掗浂銆佺姸鎬佷负鈥滆鏀剧疆鍟嗗搧鈥濄€?

2026-05-16 鏇存柊锛氭壂鐮佹敮浠橀〉鏄剧ず鎽勫儚澶寸敾闈㈠苟淇鏈夋晥鐮佸悗鐨勮秴鏃?
- 鐢ㄦ埛鏉夸笂鍙嶉锛氭壂鐮佹敮浠橀〉鑳借繘鍏?QR 妯″紡锛屼絾澶氭鍑虹幇 `QR decode ret = 3`锛涢噸鏂版壂鐮佸悗鏇惧湪 `mode=hmirror` 涓嬭瘑鍒埌 `PAY_OK_SELF_CHECKOUT_001`锛岄殢鍚庡張鎵撳嵃 `[PAY] payment timeout`锛岄〉闈㈠崱鍦ㄦ壂鐮佹祦绋嬨€?- 鍒ゆ柇锛?  - `mode=hmirror` 鎴愬姛璇存槑褰撳墠鎽勫儚澶寸敾闈?浜岀淮鐮佹柟鍚戠‘瀹炲彲鑳藉瓨鍦ㄦ按骞抽暅鍍忛棶棰橈紝鐜版湁 QR 瑙ｇ爜淇濈暀 normal + hmirror 涓よ矾灏濊瘯銆?  - 鎴愬姛璇嗗埆 1 娆″悗浠?timeout锛屾槸鍥犱负褰撳墠鏀粯鎴愬姛瑕佹眰杩炵画璇嗗埆 2 娆★紝浣?30 绉掕秴鏃惰鏃舵病鏈夊湪绗?1 娆℃湁鏁堣瘑鍒悗缁湡銆?  - 娌℃湁瀹炴椂鐢婚潰鏃讹紝鏃犳硶鐩磋鐪嬪埌浜岀淮鐮佹槸鍚﹀湪 ROI 涓€佹槸鍚﹁繃灏?杩囧ぇ銆佹槸鍚︾劍铏氥€佹洕鍏夋槸鍚﹀悎閫傘€?- 璋冩暣 `User\ui\checkout_ui.h/.c`锛?  - 鏂板 `checkout_ui_payment_set_camera_source(width, height, pixels)`銆?  - 鎵爜鏀粯椤典腑闂?260x260 妗嗘柊澧?`lv_img` 棰勮瀵硅薄銆?  - 棣栨鏀跺埌鎽勫儚澶村抚鍚庨殣钘忓師鏉ョ殑妯℃嫙浜岀淮鐮佸潡锛屽湪涓棿妗嗗唴鏄剧ず褰撳墠 RGB565 鎽勫儚澶寸敾闈紝渚夸簬瑙傚療浜岀淮鐮佷綅缃€佸ぇ灏忋€佺劍璺濄€佹洕鍏夊拰闀滃儚鏂瑰悜銆?- 璋冩暣 `User\main.c`锛?  - 鍦ㄦ敮浠?QR 姣忓抚瑙ｇ爜鍓嶏紝鍏?`dcmipp_invalidate_frame_cache()`锛屽啀璋冪敤 `checkout_ui_payment_set_camera_source()` 鍒锋柊鎵爜妗嗛瑙堛€?  - 鍒锋柊棰勮鍜屸€滆瘑鍒腑鈥濈姸鎬佸悗涓诲姩璋冪敤涓€娆?`lv_timer_handler()`锛岄伩鍏?QR 瑙ｇ爜鑰楁椂杈冮暱鏃?UI 涓€鐩寸瓑鍒拌В鐮佺粨鏉熸墠鍒锋柊銆?  - 绗?1 娆¤瘑鍒埌鏈夋晥浜岀淮鐮佸悗閲嶇疆 `g_payment_start_tick`锛岀粰绗?2 娆¤繛缁‘璁ょ暀鍑烘柊鐨勮秴鏃舵椂闂淬€?  - 瓒呮椂鍒ゆ柇鍙湪 `g_payment_valid_count == 0` 鏃惰Е鍙戯紝閬垮厤宸茶瘑鍒埌鏈夋晥浜岀淮鐮佸悗琚秴鏃跺脊绐楁墦鏂€?- 鏈涓嶆敼 QR 瑙ｇ爜搴撱€佷笉鏀?ROI 灏哄搴忓垪銆佷笉鏀规憚鍍忓ご鍒濆鍖栥€佷笉鏀?DCMIPP 閰嶇疆銆佷笉鏀规敮浠樻垚鍔熻繛缁瘑鍒鏁般€?- 缂栬瘧楠岃瘉锛?  - `F:\Keil5\UV4\UV4.exe -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun -j0`
  - 鏃ュ織锛歚Projects\MDK-ARM\payment_preview_build_final.log`
  - 缁撴灉锛? Error(s), 0 Warning(s)銆?- 闇€瑕佹澘涓婇獙璇侊細
  - 杩涘叆鎵爜鏀粯椤靛悗锛屼腑闂存壂鐮佹搴旀樉绀烘憚鍍忓ご瀹炴椂鐢婚潰锛岃€屼笉鏄彧鏄剧ず妯℃嫙浜岀淮鐮併€?  - 灏嗗浐瀹氫簩缁寸爜鏀惧埌妗嗕腑澶紝瑙傚療鐢婚潰鏄惁姘村钩闀滃儚銆佹槸鍚︽竻鏅般€佸ぇ灏忔槸鍚︽帴杩?160~280 鍍忕礌 ROI 鑼冨洿銆?  - 鑻ヤ覆鍙ｄ粛澶ч噺 `QR decode error 3`锛屼紭鍏堟寜鐢婚潰妫€鏌?ROI銆佷簩缁寸爜澶у皬銆佺劍璺濄€佹洕鍏夊拰鐏板害瀵规瘮搴︺€?  - 褰撲覆鍙ｅ嚭鐜扮 1 娆?`PAYMENT QR VALID` 鍚庯紝涓嶅簲绔嬪埢杩涘叆鏀粯瓒呮椂寮圭獥锛涘簲缁х画灏濊瘯绗?2 娆＄‘璁わ紝鎴愬姛鍚庤繘鍏ユ敮浠樻垚鍔熼〉銆?

2026-05-16 更新：新增 full/fast 快速下板方案说明

- 根据用户要求，新增 `fast.txt`，用于把当前工程的 full + fast 分离下载方案复刻到其他工程。
- 文档内容包括：
  - full 工程携带 `network_data.bin` 并写入外部 Flash 权重区的原理。
  - fast 工程使用空 stub 权重文件、跳过重复下载大权重的原理。
  - 当前工程关键地址：`0x90000000` 外部 Flash 权重源地址、`0x70400000` 运行时权重 RAM 地址、`0x90A00000` 普通 RO 区。
  - `network_weights_bin.s` 与 `network_weights_bin_fast.s` 的差异。
  - full / fast scatter 文件差异。
  - Keil target 中 scatter、权重文件、linker misc 的配置差异。
  - 复刻步骤、使用规则、常见问题和检查清单。
- 本次只新增文档，不修改源码、Keil 工程、scatter 或构建产物。

2026-05-16 更新：底层调整摄像头左右镜像方向

- 根据用户要求“将摄像头采集的地方，底层将图像左右翻转一下”，在 OV2640/MC2640 驱动层新增水平镜像控制。
- 调整 `Drivers\BSP\OV2640\ov2640.h/.c`：
  - 新增 `ov2640_hmirror(uint8_t enable)`。
  - 函数切到 Sensor bank，读取 `ATK_MC2640_REG_SENSOR_REG04`，按 bit7 控制水平镜像。
  - `enable != 0` 时置位 `0x80`；`enable == 0` 时清除 `0x80`。
  - 串口打印 `[CAM] OV2640 horizontal mirror on/off, REG04=0x..` 便于板上确认寄存器最终值。
- 调整 `User\app\camera_workflow.c`：
  - 在 `ov2640_rgb565_mode()` 后调用 `ov2640_hmirror(0U)`。
  - 当前初始化表中 Sensor `REG04` 原本写入 `0xA8`，即水平镜像位为 1；本次清除该位，使底层采集缓冲区中的图像相对当前方向左右翻转。
- 本次不改 DCMIPP、帧缓冲地址、ROI、QR 灰度转换、AI 预处理、scatter 或 Keil 工程。
- 编译验证：
  - full：`Projects\MDK-ARM\camera_hmirror_final_build.log`，结果 0 Error(s), 0 Warning(s)。
  - fast：`Projects\MDK-ARM\camera_hmirror_fast_final_build.log`，结果 0 Error(s), 0 Warning(s)。
- 需要板上验证：
  - 启动日志应出现 `[CAM] OV2640 horizontal mirror off, REG04=0x..`。
  - 主识别页拍照预览和扫码支付页中间摄像头画面应整体左右翻转。
  - 固定二维码扫码时观察串口 `QR detected mode` 是否从 `hmirror` 更倾向于 `normal`。
  - 若方向反了，可把 `camera_workflow_init()` 中的 `ov2640_hmirror(0U)` 改为 `ov2640_hmirror(1U)` 后重新编译验证。

2026-05-18 更新：首页按钮文字缺失修复

- 用户板上反馈：LVGL 界面第一页有文字缺失，底部四个按钮中有一个没有文字显示。
- 定位结果：
  - 当前首页由 `User\ui\checkout_ui.c` 创建。
  - 首页右下角按钮源码中残留乱码字符串 `缁х画璐墿`，而当前 `Font20` 字库清单中包含的是正常文案 `继续购物`，因此板上可能显示为空或缺字。
  - 首页图片占位文案也残留乱码 `鍥惧儚鏄剧ず鍖哄煙`。
- 本次仅调整 `User\ui\checkout_ui.c`：
  - 将首页图片占位文案恢复为 `图像显示区域`。
  - 将首页右下角按钮文案恢复为 `继续购物`。
- 本次不改 LVGL 配置、不改 Font20 字库、不改 Keil 工程、不改业务流程；上述两个正常文案已存在于 `Middlewares\LVGL\Font\checkout_ui_font_texts.txt`，因此无需重新生成字体。
- 编译验证：
  - `F:\Keil5\UV4\UV4.exe -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun -j0`
    - `Output\atk_h7r3_full_camrun.build_log.htm`：0 Error(s), 0 Warning(s)。
  - `F:\Keil5\UV4\UV4.exe -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast -j0`
    - `Output\atk_h7r3_full_camrun_fast.build_log.htm`：0 Error(s), 0 Warning(s)。
- 需要板上验证：
  - 下载最新 `camrun_fast` 后，首页底部四个按钮应分别显示 `查看购物车`、`结 账`、`清空购物车`、`继续购物`。
  - 首页摄像头占位区域在无预览图时应显示 `图像显示区域`。

2026-05-21 更新：修复扫码支付成功后偶发 MemManage 的 cache 失效范围风险

- 用户板上反馈：扫码支付中多次 `QR decode ret = 3` 后，重新扫码识别到 `PAY_OK_SELF_CHECKOUT_001`，随后偶发：
  - `[FAULT] MemManage`
  - `CFSR=0x00000082`
  - `MMFAR=0xDEF7F505`
- 关键判断：
  - `CFSR=0x82` 表示 MemManage 数据访问违规，且 MMFAR 有效。
  - 成功识别那次日志中 `elapsed=604302770 ms` 明显异常，说明 `atk_qr_decode()` 返回前后已经存在内存/栈或计时变量被破坏的迹象。
  - `camera_date_buf` 总大小 2MB，布局实际被复用为：
    - 前 1MB：DCMIPP 摄像头帧缓冲。
    - `0x702F4000` 起：QR 灰度缓冲。
    - 灰度缓冲之后：ATKQR 私有堆。
  - 原来的 `dcmipp_invalidate_frame_cache()` 会 invalidate 整个 `camera_date_buf`，可能把 ATKQR 私有堆或 QR 灰度缓冲的脏 cache 行丢掉，导致 allocator/库内部状态损坏。
- 本次调整 `Drivers\BSP\DCMIPP\dcmipp.c`：
  - 新增 `DCMIPP_CAPTURE_CACHE_BYTES = 1024UL * 1024UL`。
  - 新增内部 `dcmipp_invalidate_camera_buffer_cache()`，只对摄像头帧前 1MB 做 cache invalidate。
  - `dcmipp_start()`、`dcmipp_start_continuous()`、`dcmipp_invalidate_frame_cache()` 改为调用该局部失效函数。
- 本次不改 DCMIPP 启动模式、不改 QR ROI、不改 ATKQR 库、不改支付状态机。
- 编译验证：
  - full：`F:\Keil5\UV4\UV4.exe -b Projects\MDK-ARM\atk_h7r3_full_camrun.uvprojx -t atk_h7r3_full_camrun -j0`，0 Error(s), 0 Warning(s)。
  - fast：`F:\Keil5\UV4\UV4.exe -b Projects\MDK-ARM\atk_h7r3_full_camrun_fast.uvprojx -t atk_h7r3_full_camrun_fast -j0`，0 Error(s), 0 Warning(s)。
- 需要板上验证：
  - 下载最新 `camrun_fast` 后，重复扫码支付流程。
  - 观察识别成功时 `elapsed` 是否恢复为合理数值。
  - 确认 `PAYMENT QR VALID` 后不再进入 MemManage。
  - 若仍出现 fault，下一步应增加 stacked PC/LR 打印，定位 fault 精确返回地址。
