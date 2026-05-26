/**
 ****************************************************************************************************
 * @file        lcd.c
 * @author      姝ｇ偣鍘熷瓙鍥㈤槦(ALIENTEK)
 * @version     V1.0
 * @date        2024-05-21
 * @brief       LCD椹卞姩浠ｇ爜
 * @license     Copyright (c) 2020-2032, 骞垮窞甯傛槦缈肩數瀛愮鎶€鏈夐檺鍏徃
 ****************************************************************************************************
 * @attention
 * 
 * 瀹為獙骞冲彴:姝ｇ偣鍘熷瓙 H7R3寮€鍙戞澘
 * 鍦ㄧ嚎瑙嗛:www.yuanzige.com
 * 鎶€鏈鍧?www.openedv.com
 * 鍏徃缃戝潃:www.alientek.com
 * 璐拱鍦板潃:openedv.taobao.com
 * 
 ****************************************************************************************************
 */

#include "./BSP/LCD/lcd.h"
#include "./BSP/LCD/lcdfont.h"
#include "./BSP/LCD/lcd_ex.c"
#include "./SYSTEM/delay/delay.h"

SRAM_HandleTypeDef g_sram_handle = {0};   /* SRAM鍙ユ焺 */

/* 缁樺埗LCD鏃剁殑鑳屾櫙鑹?*/
uint32_t g_back_color = 0xFFFF;

/* LCD閲嶈鍙傛暟 */
_lcd_dev lcddev;

/**
 * @brief   LCD鍐欐暟鎹?
 * @param   data: 瑕佸啓鍏ョ殑鏁版嵁
 * @retval  鏃?
 */
void lcd_wr_data(volatile uint16_t data)
{
    data = data;
    LCD->LCD_RAM = data;
}

/**
 * @brief   LCD鍐欏瘎瀛樺櫒缂栧彿鎴栧湴鍧€
 * @param   regno: 瀵勫瓨鍣ㄧ紪鍙锋垨鍦板潃
 * @retval  鏃?
 */
void lcd_wr_regno(volatile uint16_t regno)
{
    regno = regno;
    LCD->LCD_REG = regno;
}

/**
 * @brief   LCD鍐欏瘎瀛樺櫒
 * @param   regno: 瀵勫瓨鍣ㄧ紪鍙?
 * @param   data : 瑕佸啓鍏ョ殑鏁版嵁
 * @retval  鏃?
 */
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_REG = regno;
    LCD->LCD_RAM = data;
}

/**
 * @brief   LCD璇绘暟鎹?
 * @param   鏃?
 * @retval  璇诲彇鍒扮殑鏁版嵁
 */
static uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;
    
    ram = LCD->LCD_RAM;
    
    return ram;
}

/**
 * @brief   LCD寤舵椂鍑芥暟
 * @note    浠呯敤浜庨儴鍒嗗湪-O1鏃堕棿浼樺寲鏃堕渶瑕佽缃殑鍦版柟
 * @param   t: 寤舵椂鐨勬暟鍊?
 * @retval  鏃?
 */
static void lcd_opt_delay(uint32_t i)
{
    /* 浣跨敤AC6鏃剁┖寰幆鍙兘琚紭鍖栵紝鍙娇鐢╳hile(1) __asm volatile(""); */
    while (i--);
}

/**
 * @brief   鍑嗗鍐橤RAM
 * @param   鏃?
 * @retval  鏃?
 */
void lcd_write_ram_prepare(void)
{
    LCD->LCD_REG = lcddev.wramcmd;
}

/**
 * @brief   璇诲彇涓煇鐐圭殑棰滆壊鍊?
 * @param   x: 鎸囧畾鐐圭殑X鍧愭爣
 * @param   y: 鎸囧畾鐐圭殑Y鍧愭爣
 * @retval  鎸囧畾鐐圭殑棰滆壊锛?2浣嶉鑹诧紝鏂逛究鍏煎LTDC锛?
 */
uint32_t lcd_read_point(uint16_t x, uint16_t y)
{
    uint16_t r;
    uint16_t g;
    uint16_t b;
    
    if ((x >= lcddev.width) || (y >= (lcddev.height)))          /* 鍒ゆ柇鐐圭殑鍧愭爣鏄惁鍚堟硶 */
    {
        return 0;
    }
    
    lcd_set_cursor(x, y);                                       /* 璁剧疆鍏夋爣 */
    
    if (lcddev.id == 0x5510)                                    /* 5510 */
    {
        lcd_wr_regno(0x2E00);
    }
    else
    {
        lcd_wr_regno(0x2E);                                     /* 9341/5310/1963/7789/7796/9806 */
    }
    
    r = lcd_rd_data();                                          /* 鏃犳晥鏁版嵁锛堥櫎1963锛?*/
    if (lcddev.id == 0x1963)                                    /* 1963 */
    {
        return r;                                               /* 1963鐩存帴璇诲嚭鏁版嵁鍗冲彲 */
    }
    
    lcd_opt_delay(2);
    r = lcd_rd_data();
    if (lcddev.id == 0x7796)                                    /* 7796涓€娆¤鍙栦竴涓儚绱犲€?*/
    {
        return r;
    }

    lcd_opt_delay(2);
    b = lcd_rd_data();                                          /* 5510/9341/5310/7789绗竴娆¤鍑篟G鏁版嵁锛岀浜屾璇诲彇BR锛圧鏄笅涓€涓儚绱犵殑棰滆壊鏁版嵁锛夋暟鎹?*/
    g = (r & 0xFF) << 8;
    
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));  /* RGB565 */
}

/**
 * @brief   LCD寮€鍚樉绀?
 * @param   鏃?
 * @retval  鏃?
 */
void lcd_display_on(void)
{
    if (lcddev.id == 0x5510)    /* 5510 */
    {
        lcd_wr_regno(0x2900);
    }
    else                        /* 9341/5310/1963/7789/7796/9806 */
    {
        lcd_wr_regno(0x29);
    }
}

/**
 * @brief   LCD鍏抽棴鏄剧ず
 * @param   鏃?
 * @retval  鏃?
 */
void lcd_display_off(void)
{
    if (lcddev.id == 0x5510)    /* 5510 */
    {
        lcd_wr_regno(0x2800);
    }
    else                        /* 9341/5310/1963/7789/7796/9806 */
    {
        lcd_wr_regno(0x28);
    }
}

/**
 * @brief   璁剧疆鍏夋爣浣嶇疆锛堝RGB灞忔棤鏁堬級
 * @param   x: 鍏夋爣鐨刋鍧愭爣
 * @param   y: 鍏夋爣鐨刌鍧愭爣
 * @retval  鏃?
 */
void lcd_set_cursor(uint16_t x, uint16_t y)
{
    if (lcddev.id == 0x1963)        /* 1963 */
    {
        if (lcddev.dir == 0)        /* 绔栧睆妯″紡闇€瑕佸彉鎹鍧愭爣 */
        {
            x = lcddev.width - 1 - x;
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(0);
            lcd_wr_data(0);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0xFF);
        }
        else                        /* 妯睆妯″紡 */
        {
            lcd_wr_regno(lcddev.setxcmd);
            lcd_wr_data(x >> 8);
            lcd_wr_data(x & 0xFF);
            lcd_wr_data((lcddev.width - 1) >> 8);
            lcd_wr_data((lcddev.width - 1) & 0xFF);
        }
        
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0xFF);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_data((lcddev.height - 1) & 0xFF);
    }
    else if (lcddev.id == 0x5510)   /* 5510 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(x & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(y & 0xFF);
    }
    else                            /* 9341/5310/7789/7796/9806 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(x >> 8);
        lcd_wr_data(x & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(y >> 8);
        lcd_wr_data(y & 0xFF);
    }
}

/**
 * @brief   璁剧疆LCD鐨勮嚜鍔ㄦ壂鎻忔柟鍚戯紙瀵筊GB灞忔棤鏁堬級
 * @note    9341/5310/5510/1963/7789/7796/9806绛塈C宸茬粡瀹為檯娴嬭瘯
 *          娉ㄦ剰:鍏朵粬鍑芥暟鍙兘浼氬彈鍒版鍑芥暟璁剧疆鐨勫奖鍝嶏紙灏ゅ叾鏄?341锛夛紝
 *          鎵€浠ワ紝涓€鑸缃负L2R_U2D鍗冲彲锛屽鏋滆缃负鍏朵粬鎵弿鏂瑰紡锛屽彲鑳藉鑷存樉绀轰笉姝ｅ父銆?
 * @param   dir: LCD鎵弿鏂瑰悜
 *   @arg   L2R_U2D: 浠庡乏鍒板彸锛屼粠涓婂埌涓?
 *   @arg   L2R_D2U: 浠庡乏鍒板彸锛屼粠涓嬪埌涓?
 *   @arg   R2L_U2D: 浠庡彸鍒板乏锛屼粠涓婂埌涓?
 *   @arg   R2L_D2U: 浠庡彸鍒板乏锛屼粠涓嬪埌涓?
 *   @arg   U2D_L2R: 浠庝笂鍒颁笅锛屼粠宸﹀埌鍙?
 *   @arg   U2D_R2L: 浠庝笂鍒颁笅锛屼粠鍙冲埌宸?
 *   @arg   D2U_L2R: 浠庝笅鍒颁笂锛屼粠宸﹀埌鍙?
 *   @arg   D2U_R2L: 浠庝笅鍒颁笂锛屼粠鍙冲埌宸?
 * @retval  鏃?
 */
void lcd_scan_dir(uint8_t dir)
{
    uint16_t regval = 0;
    uint16_t dirreg;
    uint16_t temp;
    
    /* 妯睆鏃讹紝1963涓嶆敼鍙樻壂鎻忔柟鍚戯紝鍏朵粬IC鏀瑰彉鎵弿鏂瑰悜
     * 绔栧睆鏃讹紝1963鏀瑰彉鎵弿鏂瑰悜锛屽叾浠朓C涓嶆敼鍙樻壂鎻忔柟鍚?
     */
    if (((lcddev.dir == 1) && (lcddev.id != 0x1963)) || ((lcddev.dir == 0) && (lcddev.id == 0x1963)))
    {
        switch (dir)   /* 鏂瑰悜杞崲 */
        {
            case L2R_U2D:
            {
                dir = D2U_L2R;
                break;
            }
            case L2R_D2U:
            {
                dir = D2U_R2L;
                break;
            }
            case R2L_U2D:
            {
                dir = U2D_L2R;
                break;
            }
            case R2L_D2U:
            {
                dir = U2D_R2L;
                break;
            }
            case U2D_L2R:
            {
                dir = L2R_D2U;
                break;
            }
            case U2D_R2L:
            {
                dir = L2R_U2D;
                break;
            }
            case D2U_L2R:
            {
                dir = R2L_D2U;
                break;
            }
            case D2U_R2L:
            {
                dir = R2L_U2D;
                break;
            }
        }
    }
    
    /* 鏍规嵁鎵弿鏂瑰悜璁剧疆0x36鎴?x3600瀵勫瓨鍣╞it5~7浣嶇殑鍊?*/
    switch (dir)
    {
        case L2R_U2D:
        {
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;
        }
        case L2R_D2U:
        {
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;
        }
        case R2L_U2D:
        {
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;
        }
        case R2L_D2U:
        {
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;
        }
        case U2D_L2R:
        {
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;
        }
        case U2D_R2L:
        {
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;
        }
        case D2U_L2R:
        {
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;
        }
        case D2U_R2L:
        {
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
        }
    }
    
    if (lcddev.id == 0x5510)                    /* 0x5510 */
    {
        dirreg = 0x3600;
    }
    else                                        /* 9341/5310/1963/7789 */
    {
        dirreg = 0x36;
    }
    
    /* 9341銆?789鍜?796瑕佽缃瓸GR浣?*/
    if ((lcddev.id == 0x9341) || (lcddev.id == 0x7789) || (lcddev.id == 0x7796))
    {
        regval |= 0x08;
    }
    
    lcd_write_reg(dirreg, regval);
    
    if (lcddev.id != 0x1963)                    /* 1963涓嶇敤鍋氬潗鏍囧鐞?*/
    {
        if (regval & 0x20)
        {
            if (lcddev.width < lcddev.height)   /* 浜ゆ崲X鍜孻 */
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
        else
        {
            if (lcddev.width > lcddev.height)   /* 浜ゆ崲X鍜孻 */
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
    }
    
    /* 璁剧疆鏄剧ず鍖哄煙锛堝紑绐楋級澶у皬 */
    if (lcddev.id == 0x5510)                    /* 0x5510 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setxcmd + 2);
        lcd_wr_data((lcddev.width - 1) >> 8);
        lcd_wr_regno(lcddev.setxcmd + 3);
        lcd_wr_data((lcddev.width - 1) & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(0);
        lcd_wr_regno(lcddev.setycmd + 2);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_regno(lcddev.setycmd + 3);
        lcd_wr_data((lcddev.height - 1) & 0xFF);
    }
    else                                        /* 9341/5310/1963/7789 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(0);
        lcd_wr_data(0);
        lcd_wr_data((lcddev.width - 1) >> 8);
        lcd_wr_data((lcddev.width - 1) & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(0);
        lcd_wr_data(0);
        lcd_wr_data((lcddev.height - 1) >> 8);
        lcd_wr_data((lcddev.height - 1) & 0xFF);
    }
}

/**
 * @brief   鐢荤偣
 * @param   x: 鐐圭殑X鍧愭爣
 * @param   y: 鐐圭殑Y鍧愭爣
 * @param   color: 鐐圭殑棰滆壊锛?2浣嶉鑹诧紝鏂逛究鍏煎LTDC锛?
 * @retval  鏃?
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint32_t color)
{
    lcd_set_cursor(x, y);       /* 璁剧疆鍏夋爣浣嶇疆 */
    lcd_write_ram_prepare();    /* 寮€濮嬪啓鍏RAM */
    LCD->LCD_RAM = color;
}

/**
 * @brief   璁剧疆SSD1963鑳屽厜浜害
 * @param   pwm: 鑳屽厜绛夌骇锛岃寖鍥?~100锛堟暟鍊艰秺澶ц秺浜級
 * @retval  鏃?
 */
void lcd_ssd_backlight_set(uint8_t pwm)
{
    lcd_wr_regno(0xBE);         /* 閰嶇疆PWM杈撳嚭 */
    lcd_wr_data(0x05);          /* 璁剧疆PWM棰戠巼 */
    lcd_wr_data(pwm * 2.55);    /* 璁剧疆PWM鍗犵┖姣?*/
    lcd_wr_data(0x01);          /* 璁剧疆C */
    lcd_wr_data(0xFF);          /* 璁剧疆D */
    lcd_wr_data(0x00);          /* 璁剧疆E */
    lcd_wr_data(0x00);          /* 璁剧疆F */
}

/**
 * @brief   璁剧疆LCD鏄剧ず鏂瑰悜
 * @param   dir: LCD鏄剧ず鏂瑰悜
 *   @arg   0: 绔栧睆
 *   @arg   1: 妯睆
 * @retval  鏃?
 */
void lcd_display_dir(uint8_t dir)
{
    lcddev.dir = dir;
    
    if (dir == 0)                                                   /* 绔栧睆 */
    {
        lcddev.width = 240;
        lcddev.height = 320;
        
        if (lcddev.id == 0x5510)                                    /* 5510 */
        {
            lcddev.wramcmd = 0x2C00;                                /* 寮€濮嬪啓GRAM鎸囦护 */
            lcddev.setxcmd = 0x2A00;                                /* 璁剧疆X鍧愭爣鎸囦护 */
            lcddev.setycmd = 0x2B00;                                /* 璁剧疆Y鍧愭爣鎸囦护 */
            lcddev.width = 480;                                     /* LCD瀹藉害 */
            lcddev.height = 800;                                    /* LCD楂樺害 */
        }
        else if (lcddev.id == 0x1963)                               /* 1963 */
        {
            lcddev.wramcmd = 0x2C;
            lcddev.setxcmd = 0x2B;
            lcddev.setycmd = 0x2A;
            lcddev.width = 480;
            lcddev.height = 800;
        }
        else                                                        /* 9341/5310/7789/7796/9806 */
        {
            lcddev.wramcmd = 0x2C;
            lcddev.setxcmd = 0x2A;
            lcddev.setycmd = 0x2B;
        }
        
        if ((lcddev.id == 0x5310) || (lcddev.id == 0x7796))         /* 5310/7796 */
        {
            lcddev.width = 320;
            lcddev.height = 480;
        }

        if (lcddev.id == 0x9806)
        {
            lcddev.width = 480;
            lcddev.height = 800;
        }
    }
    else                                                            /* 妯睆 */
    {
        lcddev.width = 320;
        lcddev.height = 240;
        
        if (lcddev.id == 0x5510)                                    /* 5510 */
        {
            lcddev.wramcmd = 0x2C00;
            lcddev.setxcmd = 0x2A00;
            lcddev.setycmd = 0x2B00;
            lcddev.width = 800;
            lcddev.height = 480;
        }
        else if ((lcddev.id == 0x1963) || (lcddev.id == 0x9806))    /* 1963/9806 */
        {
            lcddev.wramcmd = 0x2C;
            lcddev.setxcmd = 0x2A;
            lcddev.setycmd = 0x2B;
            lcddev.width = 800;
            lcddev.height = 480;
        }
        else                                                        /* 9341/5310/7789/7796 */
        {
            lcddev.wramcmd = 0x2C;
            lcddev.setxcmd = 0x2A;
            lcddev.setycmd = 0x2B;
        }
        
        if ((lcddev.id == 0x5310) || (lcddev.id == 0x7796))         /* 5310/7796 */
        {
            lcddev.width = 480;
            lcddev.height = 320;
        }
    }
    
    lcd_scan_dir(DFT_SCAN_DIR);         /* 璁剧疆LCD涓洪粯璁ゆ壂鎻忔柟鍚?*/
}

/**
 * @brief   璁剧疆绐楀彛锛堝RGB灞忔棤鏁堬級
 * @note    浼氳嚜鍔ㄨ缃敾鐐瑰潗鏍囧埌绐楀彛宸︿笂瑙?sx,sy)
 * @param   sx    : 绐楀彛璧峰X鍧愭爣
 * @param   sy    : 绐楀彛璧峰Y鍧愭爣
 * @param   width : 绐楀彛瀹藉害锛岄渶澶т簬0
 * @param   height: 绐楀彛楂樺害锛岄渶澶т簬0
 *  @note   绐楀彛澶у皬 = width * height
 * @retval  鏃?
 */
void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth;
    uint16_t theight;
    
    twidth = sx + width - 1;
    theight = sy + height - 1;
    
    if ((lcddev.id == 0x1963) && (lcddev.dir != 1)) /* 1963锛堢珫灞忥級 */
    {
        sx = lcddev.width - width - sx;
        height = sy + height - 1;
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx & 0xFF);
        lcd_wr_data((sx + width - 1) >> 8);
        lcd_wr_data((sx + width - 1) & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy & 0xFF);
        lcd_wr_data(height >> 8);
        lcd_wr_data(height & 0xFF);
    }
    else if (lcddev.id == 0x5510)                   /* 5510 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_regno(lcddev.setxcmd + 1);
        lcd_wr_data(sx & 0xFF);
        lcd_wr_regno(lcddev.setxcmd + 2);
        lcd_wr_data(twidth >> 8);
        lcd_wr_regno(lcddev.setxcmd + 3);
        lcd_wr_data(twidth & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_regno(lcddev.setycmd + 1);
        lcd_wr_data(sy & 0xFF);
        lcd_wr_regno(lcddev.setycmd + 2);
        lcd_wr_data(theight >> 8);
        lcd_wr_regno(lcddev.setycmd + 3);
        lcd_wr_data(theight & 0xFF);
    }
    else                                            /* 9341/5310/1963锛堟í灞忥級/7789 */
    {
        lcd_wr_regno(lcddev.setxcmd);
        lcd_wr_data(sx >> 8);
        lcd_wr_data(sx & 0xFF);
        lcd_wr_data(twidth >> 8);
        lcd_wr_data(twidth & 0xFF);
        lcd_wr_regno(lcddev.setycmd);
        lcd_wr_data(sy >> 8);
        lcd_wr_data(sy & 0xFF);
        lcd_wr_data(theight >> 8);
        lcd_wr_data(theight & 0xFF);
    }
}

/**
 * @brief   娓呭睆
 * @param   color: 娓呭睆鐨勯鑹?
 * @retval  鏃?
 */
void lcd_clear(uint16_t color)
{
    uint32_t index;
    uint32_t totalpoint;
    
    totalpoint = lcddev.width * lcddev.height;  /* 计算总像素数量 */
    lcd_set_cursor(0x00, 0x0000);               /* 设置光标位置 */
    lcd_write_ram_prepare();                    /* 开始写入GRAM */
    for (index=0; index<totalpoint; index++)
    {
        LCD->LCD_RAM = color;
    }
}

/**
 * @brief   鍦ㄦ寚瀹氬尯鍩熷唴濉厖鍗曚釜棰滆壊
 * @param   sx    : 鎸囧畾鍖哄煙鐨勮捣濮媂鍧愭爣
 * @param   sy    : 鎸囧畾鍖哄煙鐨勮捣濮媃鍧愭爣
 * @param   ex    : 鎸囧畾鍖哄煙鐨勭粨鏉焁鍧愭爣
 * @param   ey    : 鎸囧畾鍖哄煙鐨勭粨鏉焂鍧愭爣
 * @param   color : 瑕佸～鍏呯殑棰滆壊锛?2浣嶉鑹诧紝鏂逛究鍏煎LTDC锛?
 *  @note   鎸囧畾鍖哄煙鐨勫ぇ灏?= (ex - sx + 1) * (ey - sy + 1)
 * @retval  鏃?
 */
void lcd_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint32_t total;
    uint16_t width, height;

    width = ex - sx + 1U;
    height = ey - sy + 1U;
    total = (uint32_t)width * (uint32_t)height;
    
    lcd_set_window(sx, sy, width, height);
    lcd_write_ram_prepare();
    while (total--)
    {
        LCD->LCD_RAM = color;
    }
}

/**
 * @brief   鍦ㄦ寚瀹氬尯鍩熷唴濉厖鎸囧畾棰滆壊鍧?
 * @param   sx    : 鎸囧畾鍖哄煙鐨勮捣濮媂鍧愭爣
 * @param   sy    : 鎸囧畾鍖哄煙鐨勮捣濮媃鍧愭爣
 * @param   ex    : 鎸囧畾鍖哄煙鐨勭粨鏉焁鍧愭爣
 * @param   ey    : 鎸囧畾鍖哄煙鐨勭粨鏉焂鍧愭爣
 * @param   color : 鎸囧畾棰滆壊鏁扮粍鐨勯鍦板潃
 *  @note   鎸囧畾鍖哄煙鐨勫ぇ灏?= (ex - sx + 1) * (ey - sy + 1)
 * @retval  鏃?
 */
void lcd_color_fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint32_t total;

    total = (uint32_t)(ex - sx + 1U) * (uint32_t)(ey - sy + 1U);
    lcd_set_window(sx, sy, ex - sx + 1U, ey - sy + 1U);
    lcd_write_ram_prepare();
    while (total--)
    {
        LCD->LCD_RAM = *color++;
    }
}

/**
 * @brief   鐢荤嚎
 * @param   x1   : 绾跨殑璧峰X鍧愭爣
 * @param   y1   : 绾跨殑璧峰Y鍧愭爣
 * @param   x2   : 绾跨殑缁撴潫X鍧愭爣
 * @param   y2   : 绾跨殑缁撴潫Y鍧愭爣
 * @param   color: 绾跨殑棰滆壊
 * @retval  鏃?
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0;
    int yerr = 0;
    int delta_x;
    int delta_y;
    int distance;
    int incx;
    int incy;
    int row;
    int col;
    
    /* 璁＄畻鍧愭爣澧為噺 */
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    
    row = x1;
    col = y1;
    
    /* 璁剧疆X鍗曟鏂瑰悜 */
    if (delta_x > 0)
    {
        incx = 1;
    }
    else if (delta_x == 0)
    {
        incx = 0;
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    
    /* 璁剧疆Y鍗曟鏂瑰悜 */
    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
    /* 閫夊彇鍩烘湰澧為噺鍧愭爣杞?*/
    if (delta_x > delta_y)
    {
        distance = delta_x;
    }
    else
    {
        distance = delta_y;
    }
    
    for (t=0; t<=(distance+1); t++)
    {
        lcd_draw_point(row, col, color);
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief   鐢绘按骞崇嚎
 * @param   x    : 绾跨殑璧峰X鍧愭爣
 * @param   y    : 绾跨殑璧峰Y鍧愭爣
 * @param   len  : 绾跨殑闀垮害
 * @param   color: 绾跨殑棰滆壊
 * @retval  鏃?
 */
void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > lcddev.width) || (y > lcddev.height))
    {
        return;
    }
    
    lcd_fill(x, y, x + len - 1, y, color);
}

/**
 * @brief   鐢荤煩褰?
 * @param   x1   : 鐭╁舰宸︿笂瑙扻鍧愭爣
 * @param   y1   : 鐭╁舰宸︿笂瑙扽鍧愭爣
 * @param   x2   : 鐭╁舰鍙充笅瑙扻鍧愭爣
 * @param   y2   : 鐭╁舰鍙充笅瑙扽鍧愭爣
 * @param   color: 鐭╁舰鐨勯鑹?
 * @retval  鏃?
 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1, color);
    lcd_draw_line(x1, y1, x1, y2, color);
    lcd_draw_line(x1, y2, x2, y2, color);
    lcd_draw_line(x2, y1, x2, y2, color);
}

/**
 * @brief   鐢诲渾
 * @param   x0   : 鍦嗗績鐨刋鍧愭爣
 * @param   y0   : 鍦嗗績鐨刌鍧愭爣
 * @param   r    : 鍦嗙殑鍗婂緞
 * @param   color: 鍦嗙殑棰滆壊
 * @retval  鏃?
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a;
    int b;
    int di;
    
    a = 0;
    b = r;
    di = 3 - (r << 1);  /* 鍒ゆ柇涓嬩釜鐐逛綅缃殑鏍囧織 */
    
    while (a <= b)      /* 浣跨敤Bresenham绠楁硶鐢诲渾 */
    {
        lcd_draw_point(x0 + a, y0 - b, color);
        lcd_draw_point(x0 + b, y0 - a, color);
        lcd_draw_point(x0 + b, y0 + a, color);
        lcd_draw_point(x0 + a, y0 + b, color);
        lcd_draw_point(x0 - a, y0 + b, color);
        lcd_draw_point(x0 - b, y0 + a, color);
        lcd_draw_point(x0 - a, y0 - b, color);
        lcd_draw_point(x0 - b, y0 - a, color);
        a++;
        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

/**
 * @brief   鐢诲疄蹇冨渾
 * @param   x    : 鍦嗗績鐨刋鍧愭爣
 * @param   y    : 鍦嗗績鐨刌鍧愭爣
 * @param   r    : 鍦嗙殑鍗婂緞
 * @param   color: 鍦嗙殑棰滆壊
 * @retval  鏃?
 */
void lcd_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    uint32_t i;
    uint32_t imax;
    uint32_t sqmax;
    uint32_t xr;
    
    imax = ((uint32_t)r * 707) / 1000 + 1;
    sqmax = (uint32_t)r * (uint32_t)r + (uint32_t)r / 2;
    xr = r;
    
    lcd_draw_hline(x - r, y, 2 * r, color);
    for (i=1; i<=imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            if (xr > imax)
            {
                lcd_draw_hline (x - i + 1, y + xr, 2 * (i - 1), color);
                lcd_draw_hline (x - i + 1, y - xr, 2 * (i - 1), color);
            }
            xr--;
        }
        lcd_draw_hline(x - xr, y + i, 2 * xr, color);
        lcd_draw_hline(x - xr, y - i, 2 * xr, color);
    }
}

/**
 * @brief   鍦ㄦ寚瀹氫綅缃樉绀轰竴涓瓧绗?
 * @param   x    : 鎸囧畾浣嶇疆鐨刋鍧愭爣
 * @param   y    : 鎸囧畾浣嶇疆鐨刌鍧愭爣
 * @param   chr  : 瑕佹樉绀虹殑瀛楃锛岃寖鍥达細' '~'~'
 * @param   size : 瀛椾綋
 *   @arg   12: 12*12 ASCII瀛楃
 *   @arg   16: 16*16 ASCII瀛楃
 *   @arg   24: 24*24 ASCII瀛楃
 *   @arg   32: 32*32 ASCII瀛楃
 * @param   mode : 鏄剧ず妯″紡
 *   @arg   0: 闈炲彔鍔犳柟寮?
 *   @arg   1: 鍙犲姞鏂瑰紡
 * @param   color: 瀛楃鐨勯鑹?
 * @retval  鏃?
 */
void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t csize;
    uint8_t *pfont;
    uint16_t y0;
    uint8_t t;
    uint8_t t1;
    uint8_t temp;
    
    csize = ((size >> 3) + (((size & 0x7) != 0) ? 1 : 0)) * (size >> 1);    /* 璁＄畻鎵€閫夊瓧浣撳搴斾竴涓瓧绗︽墍鍗犵殑瀛楄妭鏁?*/
    chr -= ' ';                                                             /* 璁＄畻鍋忕Щ鍚庣殑鍊硷紝鍥犱负瀛楀簱鏄粠绌烘牸寮€濮嬬殑 */
    
    switch (size)
    {
        case 12:                                                            /* 12*12 ASCII瀛楃 */
        {
            pfont = (uint8_t *)asc2_1206[chr];
            break;
        }
        case 16:                                                            /* 16*16 ASCII瀛楃 */
        {
            pfont = (uint8_t *)asc2_1608[chr];
            break;
        }
        case 24:                                                            /* 24*24 ASCII瀛楃 */
        {
            pfont = (uint8_t *)asc2_2412[chr];
            break;
        }
        case 32:                                                            /* 32*32 ASCII瀛楃 */
        {
            pfont = (uint8_t *)asc2_3216[chr];
            break;
        }
        default:
        {
            return;
        }
    }
    
    y0 = y;
    for (t=0; t<csize; t++)
    {
        temp = pfont[t];                                                    /* 鑾峰彇瀛楃鐨勭偣闃垫暟鎹?*/
        for (t1=0; t1<8; t1++)                                              /* 閬嶅巻涓€涓瓧鑺傜殑8涓綅 */
        {
            if ((temp & 0x80) != 0)                                         /* 闇€瑕佹樉绀虹殑鏈夋晥鐐?*/
            {
                lcd_draw_point(x, y, color);                                /* 浠ュ瓧绗﹂鑹茬粯鍒惰繖涓偣 */
            }
            else if (mode == 0)                                             /* 涓嶉渶瑕佹樉绀虹殑鏃犳晥鐐?*/
            {
                lcd_draw_point(x, y, g_back_color);                         /* 缁樺埗鑳屾櫙鑹?*/
            }
            
            temp <<= 1;                                                     /* 绉讳綅鑷充笅涓€涓綅 */
            y++;
            if (y >= lcddev.height)                                         /* 鍒ゆ柇Y鍧愭爣鏄惁瓒呭嚭鏄剧ず鍖哄煙 */
            {
                return;
            }
            if ((y - y0) == size)                                           /* 鏈缁樺埗瀹屾垚 */
            {
                y = y0;                                                     /* Y鍧愭爣澶嶄綅 */
                x++;                                                        /* 涓嬩竴琛?*/
                if (x >= lcddev.width)                                      /* 鍒ゆ柇X鍧愭爣鏄惁瓒呭嚭鏄剧ず鍖哄煙 */
                {
                    return;
                }
                break;
            }
        }
    }
}

/**
 * @brief   骞虫柟鍑芥暟
 * @param   m: 搴曟暟
 * @param   n: 鎸囨暟
 * @retval  m^n
 */
static uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    
    while (n--)
    {
        result *= m;
    }
    
    return result;
}

/**
 * @brief   鏄剧ずlen涓暟瀛?
 * @param   x    : 璧峰X鍧愭爣
 * @param   y    : 璧峰Y鍧愭爣
 * @param   num  : 鏁板€硷紝鑼冨洿锛?~2^32
 * @param   len  : 鏄剧ず鏁板瓧鐨勪綅鏁?
 * @param   size : 瀛椾綋
 *   @arg   12: 12*12 ASCII瀛楃
 *   @arg   16: 16*16 ASCII瀛楃
 *   @arg   24: 24*24 ASCII瀛楃
 *   @arg   32: 32*32 ASCII瀛楃
 * @param   color: 鏁板瓧鐨勯鑹?
 * @retval  鏃?
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t;
    uint8_t temp;
    uint8_t enshow = 0;
    
    for (t=0; t<len; t++)                                                   /* 鎸夋€绘樉绀轰綅鏁板惊鐜?*/
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;                       /* 鑾峰彇瀵瑰簲浣嶇殑鏁板瓧 */
        if((enshow == 0) && (t < (len - 1)))                                /* 娌℃湁浣胯兘鏄剧ず锛屼笖杩樻湁浣嶈鏄剧ず */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size >> 1) * t, y, ' ', size, 0, color); /* 鏄剧ず绌烘牸锛屽崰浣?*/
                continue;                                                   /* 缁х画涓嬩竴涓綅 */
            }
            else
            {
                enshow = 1;                                                 /* 浣胯兘鏄剧ず */
            }
        }
        
        lcd_show_char(x + (size >> 1) * t, y, temp + '0', size, 0, color);  /* 鏄剧ず瀛楃 */
    }
}

/**
 * @brief   鎵╁睍鏄剧ずlen涓暟瀛楋紙鏄剧ず楂樹綅0锛?
 * @param   x    : 璧峰X鍧愭爣
 * @param   y    : 璧峰Y鍧愭爣
 * @param   num  : 鏁板€硷紝鑼冨洿锛?~2^32
 * @param   len  : 鏄剧ず鏁板瓧鐨勪綅鏁?
 * @param   size : 瀛椾綋
 *   @arg   12: 12*12 ASCII瀛楃
 *   @arg   16: 16*16 ASCII瀛楃
 *   @arg   24: 24*24 ASCII瀛楃
 *   @arg   32: 32*32 ASCII瀛楃
 * @param   mode : 鏄剧ず妯″紡
 *   @arg   0: 闈炲彔鍔犳柟寮?
 *   @arg   1: 鍙犲姞鏂瑰紡
 * @param   color: 鏁板瓧鐨勯鑹?
 * @retval  鏃?
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t;
    uint8_t temp;
    uint8_t enshow = 0;
    
    for (t=0; t<len; t++)                                                                   /* 鎸夋€绘樉绀轰綅鏁板惊鐜?*/
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;                                       /* 鑾峰彇瀵瑰簲浣嶇殑鏁板瓧 */
        if((enshow == 0) && (t < (len - 1)))                                                /* 娌℃湁浣胯兘鏄剧ず锛屼笖杩樻湁浣嶈鏄剧ず */
        {
            if (temp == 0)
            {
                if ((mode & 0x80) != 0)                                                     /* 楂樹綅闇€瑕佸～鍏? */
                {
                    lcd_show_char(x + (size >> 1) * t, y, '0', size, mode & 0x01, color);   /* 鏄剧ず0锛屽崰浣?*/
                }
                else
                {
                    lcd_show_char(x + (size >> 1) * t, y, ' ', size, mode & 0x01, color);   /* 鏄剧ず绌烘牸锛屽崰浣?*/
                }
                continue;                                                                   /* 缁х画涓嬩竴涓綅 */
            }
            else
            {
                enshow = 1;                                                                 /* 浣胯兘鏄剧ず */
            }
        }
        
        lcd_show_char(x + (size >> 1) * t, y, temp + '0', size, mode & 0x01, color);        /* 鏄剧ず瀛楃 */
    }
}

/**
 * @brief   鏄剧ず瀛楃涓?
 * @param   x     : 璧峰X鍧愭爣
 * @param   y     : 璧峰Y鍧愭爣
 * @param   width : 鏄剧ず鍖哄煙瀹藉害
 * @param   height: 鏄剧ず鍖哄煙楂樺害
 * @param   size  : 瀛椾綋
 *   @arg   12: 12*12 ASCII瀛楃
 *   @arg   16: 16*16 ASCII瀛楃
 *   @arg   24: 24*24 ASCII瀛楃
 *   @arg   32: 32*32 ASCII瀛楃
 * @param   *p    : 瀛楃涓叉寚閽?
 * @param   color : 瀛楃涓茬殑棰滆壊
 * @retval  鏃?
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0;
    
    x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' '))              /* 鍒ゆ柇鏄惁涓洪潪娉曞瓧绗?*/
    {
        if (x >= width)                             /* 瀹藉害瓒婄晫 */
        {
            x = x0;                                 /* 鎹㈣ */
            y += size;
        }
        
        if (y >= height)                            /* 楂樺害瓒婄晫 */
        {
            break;                                  /* 閫€鍑?*/
        }
        
        lcd_show_char(x, y, *p, size, 0, color);    /* 鏄剧ず涓€涓瓧绗?*/
        x += (size >> 1);                           /* ASCII瀛楃瀹藉害涓洪珮搴︾殑涓€鍗?*/
        p++;
    }
}

/**
 * @brief   鍒濆鍖朙CD
 * @param   鏃?
 * @retval  鏃?
 */
void lcd_init(void)
{
    RCC_PeriphCLKInitTypeDef rcc_periph_clk_init_struct = {0};
    GPIO_InitTypeDef gpio_init_struct = {0};
    FMC_NORSRAM_TimingTypeDef fmc_read_timing_struct = {0};
    FMC_NORSRAM_TimingTypeDef fmc_write_timing_struct = {0};
    
    /* 閰嶇疆鏃堕挓婧?*/
    rcc_periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
    rcc_periph_clk_init_struct.FmcClockSelection = RCC_FMCCLKSOURCE_HCLK;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periph_clk_init_struct);
    
    /* 浣胯兘鏃堕挓 */
    LCD_BL_GPIO_CLK_ENABLE();
    
    /* 閰嶇疆LCD BL寮曡剼 */
    gpio_init_struct.Pin = LCD_BL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LCD_BL_GPIO_PORT, &gpio_init_struct);
    
    /* 閰嶇疆FMC璇绘椂搴?*/
    fmc_read_timing_struct.AddressSetupTime = 15;
    fmc_read_timing_struct.AddressHoldTime = 0;
    fmc_read_timing_struct.DataSetupTime = 107;
    fmc_read_timing_struct.AccessMode = FMC_ACCESS_MODE_A;
    
    /* 閰嶇疆FMC鍐欐椂搴?*/
    fmc_write_timing_struct.AddressSetupTime = 15;
    fmc_write_timing_struct.AddressHoldTime = 0;
    fmc_write_timing_struct.DataSetupTime = 20;
    fmc_write_timing_struct.AccessMode = FMC_ACCESS_MODE_A;
    
    /* 閰嶇疆FMC */
    g_sram_handle.Instance = FMC_NORSRAM_DEVICE;
    g_sram_handle.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    g_sram_handle.Init.NSBank = FMC_NORSRAM_BANK1;
    g_sram_handle.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
    g_sram_handle.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
    g_sram_handle.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    g_sram_handle.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    g_sram_handle.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    g_sram_handle.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    g_sram_handle.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
    g_sram_handle.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    g_sram_handle.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
    g_sram_handle.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    g_sram_handle.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    g_sram_handle.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    g_sram_handle.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
    g_sram_handle.Init.PageSize = FMC_PAGE_SIZE_NONE;
    
    HAL_SRAM_Init(&g_sram_handle, &fmc_read_timing_struct, &fmc_write_timing_struct);
    delay_ms(50);
    
    /* 灏濊瘯璇诲彇ILI9341鐨処D */
    lcd_wr_regno(0xD3);
    lcddev.id = lcd_rd_data();                                                              /* 鏃犳晥鏁版嵁 */
    lcddev.id = lcd_rd_data();                                                              /* 璇诲彇鍒?x00 */
    lcddev.id = lcd_rd_data() << 8;                                                         /* 璇诲彇鍒?x93 */
    lcddev.id |= lcd_rd_data();                                                             /* 璇诲彇鍒?x41 */
    if (lcddev.id != 0x9341)                                                                /* 灏濊瘯璇诲彇ST7789鐨処D */
    {
        lcd_wr_regno(0x04);
        lcddev.id = lcd_rd_data();                                                          /* 鏃犳晥鏁版嵁 */
        lcddev.id = lcd_rd_data();                                                          /* 璇诲彇鍒?x85 */
        lcddev.id = lcd_rd_data() << 8;                                                     /* 璇诲彇鍒?x85 */
        lcddev.id |= lcd_rd_data();                                                         /* 璇诲彇鍒?x52 */
        if (lcddev.id == 0x8552)                                                            /* 灏哠T7789鐨処D璁颁负0x7789 */
        {
            lcddev.id = 0x7789;
        }
        if (lcddev.id != 0x7789)                                                            /* 灏濊瘯璇诲彇NT35310鐨処D */
        {
            lcd_wr_regno(0xD4);
            lcddev.id = lcd_rd_data();                                                      /* 鏃犳晥鏁版嵁 */
            lcddev.id = lcd_rd_data();                                                      /* 璇诲彇鍒?x01 */
            lcddev.id = lcd_rd_data() << 8;                                                 /* 璇诲彇鍒?x53 */
            lcddev.id |= lcd_rd_data();                                                     /* 璇诲彇鍒?x10 */
            if (lcddev.id != 0x5310)                                                        /* 灏濊瘯璇诲彇ST7796鐨処D */
            {
                lcd_wr_regno(0XD3);
                lcddev.id = lcd_rd_data();
                lcddev.id = lcd_rd_data();
                lcddev.id = lcd_rd_data();
                lcddev.id <<= 8;
                lcddev.id |= lcd_rd_data();
                if (lcddev.id != 0x7796)                                                    /* 灏濊瘯璇诲彇NT35510鐨処D */
                {
                    /* 鍙戦€佸瘑閽ワ紙鍘傚鎻愪緵锛?*/
                    lcd_write_reg(0xF000, 0x0055);
                    lcd_write_reg(0xF001, 0x00AA);
                    lcd_write_reg(0xF002, 0x0052);
                    lcd_write_reg(0xF003, 0x0008);
                    lcd_write_reg(0xF004, 0x0001);
                    
                    lcd_wr_regno(0xC500);
                    lcddev.id = lcd_rd_data() << 8;                                         /* 璇诲彇鍒?x80 */
                    lcd_wr_regno(0xC501);
                    lcddev.id |= lcd_rd_data();                                             /* 璇诲彇鍒?x00 */
                    delay_ms(5);                                                            /* 鍥犱负姝T5510鐨勬寚浠ゆ伆濂戒负SSD1963鐨勮蒋浠跺浣嶆寚浠?*/
                    if (lcddev.id != 0x5510)                                                /* 灏濊瘯璇诲彇ILI9806鐨処D */
                    {
                        lcd_wr_regno(0XD3);
                        lcddev.id = lcd_rd_data();
                        lcddev.id = lcd_rd_data();
                        lcddev.id = lcd_rd_data();
                        lcddev.id <<= 8;
                        lcddev.id |= lcd_rd_data();
                        if (lcddev.id != 0x9806)                                            /* 灏濊瘯璇诲彇SSD1963鐨処D */
                        {
                            lcd_wr_regno(0xA1);
                            lcddev.id = lcd_rd_data();
                            lcddev.id = lcd_rd_data() << 8;                                 /* 璇诲彇鍒?x57 */
                            lcddev.id |= lcd_rd_data();                                     /* 璇诲彇鍒?x61 */
                            if (lcddev.id == 0x5761)                                        /* 灏哠SD1963鐨処D璁颁负0x1963 */
                            {
                                lcddev.id = 0x1963;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (lcddev.id == 0x7789)
    {
        lcd_ex_st7789_reginit();                                                            /* 鎵цST7789鍒濆鍖?*/
    }
    else if (lcddev.id == 0x9341)
    {
        lcd_ex_ili9341_reginit();                                                           /* 鎵цILI9341鍒濆鍖?*/
    }
    else if (lcddev.id == 0x5310)
    {
        lcd_ex_nt35310_reginit();                                                           /* 鎵цNT35310鍒濆鍖?*/
    }
    else if (lcddev.id == 0x7796)
    {
        lcd_ex_st7796_reginit();                                                            /* 鎵цST7796鍒濆鍖?*/
    }
    else if (lcddev.id == 0x5510)
    {
        lcd_ex_nt35510_reginit();                                                           /* 鎵цNT35510鍒濆鍖?*/
    }
    else if (lcddev.id == 0x9806)
    {
        lcd_ex_ili9806_reginit();                                                           /* 鎵цILI9806鍒濆鍖?*/
    }
    else if (lcddev.id == 0x1963)
    {
        lcd_ex_ssd1963_reginit();                                                           /* 鎵цSSD1963鍒濆鍖?*/
        lcd_ssd_backlight_set(100);                                                         /* 鑳屽厜璁剧疆涓烘渶浜?*/
    }
    
    /* 鍒濆鍖栧畬鎴愬悗锛屾彁閫?*/
    if ((lcddev.id == 0x9341) || (lcddev.id == 0x1963) || (lcddev.id == 0x7789))            /* 9341/1963/7789 */
    {
        fmc_write_timing_struct.AddressSetupTime = 3;
        fmc_write_timing_struct.DataSetupTime = 3;
        FMC_NORSRAM_Extended_Timing_Init(g_sram_handle.Extended, &fmc_write_timing_struct, g_sram_handle.Init.NSBank, g_sram_handle.Init.ExtendedMode);
    }
    else if ((lcddev.id == 0x5310) ||                                                       /* 5310/7796/5510/9806 */
             (lcddev.id == 0x7796) ||
             (lcddev.id == 0x5510) ||
             (lcddev.id == 0x9806))
    {
        fmc_write_timing_struct.AddressSetupTime = 3;
        fmc_write_timing_struct.DataSetupTime = 3;
        FMC_NORSRAM_Extended_Timing_Init(g_sram_handle.Extended, &fmc_write_timing_struct, g_sram_handle.Init.NSBank, g_sram_handle.Init.ExtendedMode);
    }
    
    lcd_display_dir(0);                                                                     /* 榛樿璁剧疆涓虹珫灞?*/
    LCD_BL(1);                                                                              /* 鐐逛寒鑳屽厜 */
    lcd_clear(WHITE);                                                                       /* 娓呭睆 */
}

/**
 * @brief   HAL搴揝RAM鍒濆鍖朚SP鍑芥暟
 * @param   鏃?
 * @retval  鏃?
 */
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    if (hsram->Instance == FMC_NORSRAM_DEVICE)
    {
        /* 浣胯兘鏃堕挓 */
        __HAL_RCC_FMC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        LCD_WR_GPIO_CLK_ENABLE();
        LCD_RD_GPIO_CLK_ENABLE();
        LCD_CS_GPIO_CLK_ENABLE();
        LCD_RS_GPIO_CLK_ENABLE();
        
        /* 閰嶇疆FMC寮曡剼 */
        gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = GPIO_AF12_FMC;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
        
        gpio_init_struct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
        
        gpio_init_struct.Pin = LCD_WR_GPIO_PIN;
        gpio_init_struct.Alternate = LCD_WR_GPIO_AF;
        HAL_GPIO_Init(LCD_WR_GPIO_PORT, &gpio_init_struct);
        
        gpio_init_struct.Pin = LCD_RD_GPIO_PIN;
        gpio_init_struct.Alternate = LCD_RD_GPIO_AF;
        HAL_GPIO_Init(LCD_RD_GPIO_PORT, &gpio_init_struct);
        
        gpio_init_struct.Pin = LCD_CS_GPIO_PIN;
        gpio_init_struct.Alternate = LCD_CS_GPIO_AF;
        HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpio_init_struct);
        
        gpio_init_struct.Pin = LCD_RS_GPIO_PIN;
        gpio_init_struct.Alternate = LCD_RS_GPIO_AF;
        HAL_GPIO_Init(LCD_RS_GPIO_PORT, &gpio_init_struct);
    }
}











