#include "motor.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "usart.h"	

extern int Target_Position, Position_Amplitude;															//===Ŀ�� ����
float Position_KP = 1.5, Position_KI=0, Position_KD=3;									//===PID����
extern int Flag_Foreward, Flag_Reversal;																		//===��ת ��ת��־λ
extern int tmp;		
extern int Moto1, Moto2;                           //=====���PWM���� Ӧ��Motor�� ��Moto�¾�	

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
}

void Motor_Init2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOA 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
}

void PWM_Init(u16 arr,u16 psc)
{		 		
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	Motor_Init();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
   //���ø�����Ϊ�����������,���TIM1 CH1 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM_CH1 //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
//	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
 
} 

void PWM_Init2(u16 arr,u16 psc)
{		 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	Motor_Init2();	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);     //��ӳ��PA2��PB10
	
   //���ø�����Ϊ�����������,���TIM1 CH1 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //TIM2_CH3 //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

  TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE �����ʹ��	

//	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	 
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
 
} 
																										
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PIDA (int Encoder,int Target)
{ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias = Encoder - Target;                                  								//����ƫ��
	
	if ((Encoder > Target) && (1 == Flag_Foreward))														//�жϱ����� �� Ŀ��ĶԱ�
	{
		Pwm = 0;	
		return Pwm;
	}
	if ((Encoder < Target) && (1 == Flag_Reversal))														//�жϱ����� �� Ŀ��ĶԱ�
	{
		Pwm = 0;	
		return Pwm;
	}
	Integral_bias += Bias;	                                 									//���ƫ��Ļ���
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias);	//λ��ʽPID������
	Last_Bias=Bias;                                       										//������һ��ƫ��
	return Pwm;                                           										//�������	
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PIDB (int Encoder,int Target)
{ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias = Encoder - Target;                                  									//����ƫ��
	
	if ((Encoder > Target) && (1 == Flag_Foreward))															//�жϱ����� �� Ŀ��ĶԱ�
	{
		Pwm = 0;	
		return Pwm;
	}
	if ((Encoder < Target) && (1 == Flag_Reversal))															//�жϱ����� �� Ŀ��ĶԱ�
	{
		Pwm = 0;	
		return Pwm;
	}
	Integral_bias += Bias;	                                 										//���ƫ��Ļ���
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias);	//λ��ʽPID������
	Last_Bias=Bias;                                       											//������һ��ƫ��
	return Pwm;                                           											//�������	
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude = 255; 
    if(Moto1<-Amplitude) Moto1 = -Amplitude;	
		if(Moto1>Amplitude)  Moto1 =  Amplitude;	
	  if(Moto2<-Amplitude) Moto2 = -Amplitude;	
		if(Moto2>Amplitude)  Moto2 =  Amplitude;		
}

/**************************************************************************
�������ܣ������޸�����״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)  
{	 
	//tmp = click_N_Double(50,KEY1); 
	
	if(tmp==1)
	{
		Target_Position += Position_Amplitude;
//		Target_Position=963;		//180*5.35
//		Flag_Foreward = 1;		//��ת
//		Flag_Reversal = 0;		//��ת
		
		LED0=!LED0;
		delay_ms(100);
		LED0=!LED0;
	}
	if(tmp==2)
	{
		Target_Position -= Position_Amplitude; 
//		Target_Position=0;
//		Flag_Foreward = 0;		//��ת
//		Flag_Reversal = 1;    //��ת
		
		LED0=!LED0;
		delay_ms(500);
		LED0=!LED0;
	}		
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
	    int siqu=0;//��������
			if(moto1<0)			BIN2=0,			BIN1=1;
			else 	          BIN2=1,			BIN1=0;
			PWMB=myabs(moto1)+siqu;
		  if(moto2<0)	AIN1=0,			AIN2=1;
			else        AIN1=1,			AIN2=0;
			PWMA=myabs(moto2)+siqu;
}

void Set_Pwm2(int moto1,int moto2)
{
	    int siqu=5;//��������
			if(moto1<0)			BIN2s=0,			BIN1s=1;
			else 	          BIN2s=1,			BIN1s=0;
			PWMBs=myabs(moto1)+siqu;
		  if(moto2<0)	AIN1s=0,			AIN2s=1;
			else        AIN1s=1,			AIN2s=0;
			PWMAs=myabs(moto2)+siqu;
}

void Stop_motor(int num)
{
	int siqu=5;//��������
	switch(num)
	{
		
		case 1:
			PWMA=siqu;
			break;
		case 2:
			PWMB=siqu;
			break;
		case 3:
			PWMAs=siqu;
			break;
		case 4:
			PWMBs=siqu;
			break;
		default:
			break;
	}
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}




