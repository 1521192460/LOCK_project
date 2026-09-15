#ifndef __LCD_H__
#define __LCD_H__
#include "main.h"
//宏定义
extern const unsigned char gImage_001[115208];
//函数声明
void lcd_clear(u16 xs,u16 ys,u16 xe,u16 ye,u16 color);
void lcd_set_position(u16 xs,u16 ys,u16 xe,u16 ye);
void lcd_init(void);
void lcd_show_image(u8 *image,u8 x,u8 y);
void lcd_show_eng(u8 *font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg);
void lcd_show_chinese(u8 *font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg);

/*********************字库显示******************************/
void lcd_show_zk_font(u8 *font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg);
void lcd_show_zk_char(u8 font,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg);
void lcd_show_zk_str(u8 *str,u8 x,u8 y,u8 size,u16 font_color,u16 font_bg);
#endif