#ifndef __LCD12864_H
#define __LCD12864_H	 
#include "sys.h"

#define cs_gpio GPIO_Pin_2
#define reset_gpio GPIO_Pin_7
#define rs_gpio GPIO_Pin_15
#define sid_gpio GPIO_Pin_14
#define sclk_gpio GPIO_Pin_13

#define cs1 PAout(2)	//PC0
#define reset PAout(7)	//PC1
#define rs PCout(15)	//PC15
#define sid PCout(14)	//PC14
#define sclk PCout(13)	//PC13

void transfer_command(int data1);
void transfer_data(int data1);
void LCD12864_Init(void);
void initial_lcd(void);
void lcd_address(unsigned char page,unsigned char column);
void clear_screen(void);
void disp_blink_16x16(unsigned char page,unsigned char column,unsigned char blink,unsigned char reverse,unsigned char *dp);
void disp_underline_16x16(unsigned char page,unsigned char column,unsigned char under,unsigned char reverse,unsigned char *dp);
void disp_cursor_16x16(unsigned char page,unsigned char column,unsigned char cursor,unsigned char reverse,unsigned char *dp);
void LCD12864_P6x8Str(unsigned char y,unsigned char x,unsigned char ch[]);
void LCD12864_P8x16Str(unsigned char y,unsigned char x,unsigned char ch[]);
void disp_blink_40x40(unsigned char page,unsigned char column,unsigned char blink,unsigned char reverse,unsigned char *dp);

#endif

