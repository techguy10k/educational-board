#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
#include "encoder.h"

#define PWMA   TIM1->CCR1  //PA8
#define AIN2   PBout(15)
#define AIN1   PBout(14)
#define BIN1   PBout(13)
#define BIN2   PBout(12)
#define PWMB   TIM1->CCR4  //PA11

#define PWMAs   TIM2->CCR3  //PB10
#define AIN2s   PBout(3)
#define AIN1s  	PBout(2)
#define BIN1s   PAout(4)
#define BIN2s   PAout(5)
#define PWMBs   TIM2->CCR4  //PB11

void PWM_Init(u16 arr,u16 psc);
void Motor_Init(void);
void PWM_Init2(u16 arr,u16 psc);
void Motor_Init2(void);
int Position_PIDA (int Encoder,int Target);
int Position_PIDB (int Encoder,int Target);
void Xianfu_Pwm(void);
void Set_Pwm(int moto1,int moto2);
void Set_Pwm2(int moto1,int moto2);
void Stop_motor(int num);
int myabs(int a);
void Key(void);

#endif
