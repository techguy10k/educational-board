#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
 
#define KEY0   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//��ȡ������

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��	
u8 click_N_Double (u8 time,u8 KEY);
u8 Long_Press(u8 KEY);

#endif
