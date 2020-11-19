#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(12)
#define Power_button PAout(3)
#define backlight PAout(15)

void LED_Init(void);//≥ı ºªØ
void Power_off_button_Init(void);
void backlight_Init(void);

#endif
