#include "motor.h"
#include "key.h"
#include "led.h"
#include "delay.h"
#include "usart.h"	

extern int Target_Position, Position_Amplitude;															//===目标 步长
float Position_KP = 1.5, Position_KI=0, Position_KD=3;									//===PID比例
extern int Flag_Foreward, Flag_Reversal;																		//===左转 右转标志位
extern int tmp;		
extern int Moto1, Moto2;                           //=====电机PWM变量 应是Motor的 向Moto致敬	

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
}

void Motor_Init2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
}

void PWM_Init(u16 arr,u16 psc)
{		 		
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	Motor_Init();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
   //设置该引脚为复用输出功能,输出TIM1 CH1 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM_CH1 //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
//	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
 
} 

void PWM_Init2(u16 arr,u16 psc)
{		 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	Motor_Init2();	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);     //从映射PA2到PB10
	
   //设置该引脚为复用输出功能,输出TIM1 CH1 CH4的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //TIM2_CH3 //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE 主输出使能	

//	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH3预装载使能	 
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH4预装载使能	 
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
 
} 
																										
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PIDA (int Encoder,int Target)
{ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias = Encoder - Target;                                  								//计算偏差
	
	if ((Encoder > Target) && (1 == Flag_Foreward))														//判断编码器 与 目标的对比
	{
		Pwm = 0;	
		return Pwm;
	}
	if ((Encoder < Target) && (1 == Flag_Reversal))														//判断编码器 与 目标的对比
	{
		Pwm = 0;	
		return Pwm;
	}
	Integral_bias += Bias;	                                 									//求出偏差的积分
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias);	//位置式PID控制器
	Last_Bias=Bias;                                       										//保存上一次偏差
	return Pwm;                                           										//增量输出	
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PIDB (int Encoder,int Target)
{ 	
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias = Encoder - Target;                                  									//计算偏差
	
	if ((Encoder > Target) && (1 == Flag_Foreward))															//判断编码器 与 目标的对比
	{
		Pwm = 0;	
		return Pwm;
	}
	if ((Encoder < Target) && (1 == Flag_Reversal))															//判断编码器 与 目标的对比
	{
		Pwm = 0;	
		return Pwm;
	}
	Integral_bias += Bias;	                                 										//求出偏差的积分
	Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias);	//位置式PID控制器
	Last_Bias=Bias;                                       											//保存上一次偏差
	return Pwm;                                           											//增量输出	
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
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
函数功能：按键修改运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)  
{	 
	//tmp = click_N_Double(50,KEY1); 
	
	if(tmp==1)
	{
		Target_Position += Position_Amplitude;
//		Target_Position=963;		//180*5.35
//		Flag_Foreward = 1;		//正转
//		Flag_Reversal = 0;		//反转
		
		LED0=!LED0;
		delay_ms(100);
		LED0=!LED0;
	}
	if(tmp==2)
	{
		Target_Position -= Position_Amplitude; 
//		Target_Position=0;
//		Flag_Foreward = 0;		//正转
//		Flag_Reversal = 1;    //反转
		
		LED0=!LED0;
		delay_ms(500);
		LED0=!LED0;
	}		
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
	    int siqu=0;//死区补偿
			if(moto1<0)			BIN2=0,			BIN1=1;
			else 	          BIN2=1,			BIN1=0;
			PWMB=myabs(moto1)+siqu;
		  if(moto2<0)	AIN1=0,			AIN2=1;
			else        AIN1=1,			AIN2=0;
			PWMA=myabs(moto2)+siqu;
}

void Set_Pwm2(int moto1,int moto2)
{
	    int siqu=5;//死区补偿
			if(moto1<0)			BIN2s=0,			BIN1s=1;
			else 	          BIN2s=1,			BIN1s=0;
			PWMBs=myabs(moto1)+siqu;
		  if(moto2<0)	AIN1s=0,			AIN2s=1;
			else        AIN1s=1,			AIN2s=0;
			PWMAs=myabs(moto2)+siqu;
}

void Stop_motor(int num)
{
	int siqu=5;//死区补偿
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
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}




