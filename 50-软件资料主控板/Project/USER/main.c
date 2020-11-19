#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "12864.h"
#include "exti.h"
#include "key.h"
#include "adc.h"
#include "motor.h"
#include "encoder.h"

void LCD_show(void);

extern unsigned char H[];
extern unsigned char ii[];
extern unsigned char t[];
extern unsigned char B[];
//extern unsigned char i[];
extern unsigned char c[];
extern unsigned char zhuang1[];
extern unsigned char tai1[];
extern unsigned char shi1[];
extern unsigned char yong1[];
extern unsigned char dianchi[];
extern unsigned char dianchi100[];
extern unsigned char dianchi75[];
extern unsigned char dianchi50[];
extern unsigned char dianchi25[];
extern unsigned char dianchi0[];
extern unsigned char dianchi1[];
extern unsigned char F6x8[][6];
extern unsigned char F8X16[];
extern unsigned char server[];
extern unsigned char motor[];
extern unsigned char information[];
extern unsigned char biaoti[];
extern unsigned char xiahuaxiang[];
unsigned char Volta[]={"Volta:  .  v"};
unsigned char Null[]={"                     "};
int i=0,flog=1,OK_flog=0,Refresh=0,dontdisplay=0;
u16 adcx,temp;
float tempx;
int Moto1_turn=1,Moto2_turn=1;
int Encoder_Left, Encoder_Right;            //=====左右编码器的脉冲计数
int Moto1, Moto2;                           //=====电机PWM变量 应是Motor的 向Moto致敬	
int Target_Position = 0; 								//=====编码器的脉冲计数
int Position_Amplitude = 45*5.35;							//=====步长 
int tmp = 0;																//=====按键次数计算
int Flag_Foreward = 0, Flag_Reversal = 0;		//=====正反转标志位
int power_flag=0;

void uart(void){
	u8 t;
	u8 len,Cache;	
//	u16 times=0;
	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;				//得到此次接收到的数据长度
		for(t=0;t<len;t++)
		{
			USART1->DR=USART_RX_BUF[t];
			while((USART1->SR&0X40)==0);		//等待发送结束
		}
		if(USART_RX_BUF[0]!=0x58&&USART_RX_BUF[0]!=0x0F)
		{
			if(USART_RX_BUF[0]!=9)
			{
				Cache=USART_RX_BUF[0];
				USART_RX_BUF[len]=NULL;
//					clear_screen();
				LCD12864_P6x8Str(Cache,1,Null);
				delay_ms(10);
				LCD12864_P6x8Str(Cache,1,USART_RX_BUF+1);
			}
			else{
				clear_screen();
				LCD_show();
			}
//			if(USART_RX_BUF[0]==1)
//				dontdisplay=1;
		}
		else if(USART_RX_BUF[0]==0x58)
		{
			switch(USART_RX_BUF[1])
			{
				case 0:Moto1_turn=1;Moto2_turn=1;break;//马达1:正 马达2:正
				case 1:Moto1_turn=-1;Moto2_turn=1;break;//马达1:反 马达2:正
				case 2:Moto1_turn=1;Moto2_turn=-1;break;//马达1:正 马达2:反
				case 3:Moto1_turn=-1;Moto2_turn=-1;break;//马达1:反 马达2:反
				default:break;
			}
			if(USART_RX_BUF[1]!=4){
				if(USART_RX_BUF[4]==0)					//port1
					Set_Pwm(USART_RX_BUF[3] * Moto2_turn,USART_RX_BUF[2] * -Moto1_turn);
				else if(USART_RX_BUF[4]==1)					//port2
					Set_Pwm2(USART_RX_BUF[3] * Moto2_turn,USART_RX_BUF[2] * -Moto1_turn);
			}
			else if(USART_RX_BUF[1]==4)			//电机停止标志位
			{
				Stop_motor(USART_RX_BUF[4]);
			}
		}
		else if(USART_RX_BUF[0]==0x0F){
			clear_screen();
			LCD_show();
		}
		LED0=!LED0;				//闪烁LED,提示系统正在运行.
		delay_ms(10);
//		printf("\n");							//插入换行
		USART_RX_STA=0;
	}else
	{
//		Set_Pwm(0,0);
//		Set_Pwm2(0,0);
//		times++;
//		printf("123");
//		if(times%30==0)LED0=!LED0;				//闪烁LED,提示系统正在运行.
//		delay_ms(100);   
	}
}

void Init(void){
	delay_init();	    	 						//延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// 设置中断优先级分组2
	Power_off_button_Init();
	Power_button=1;									//开机
	uart_init(115200);							 	//串口初始化为115200
	initial_lcd();
	PWM_Init(260,0);
	PWM_Init2(260,0);
	Encoder_Init_TIM2();            				//=====编码器接口
    Encoder_Init_TIM4();
	EXTIX_Init();
	Timer1_Init(99,7199);
	Adc_Init();
	LED_Init();		  	 							//初始化与LED连接的硬件接口 
	backlight_Init();
	backlight=0;									//开背光灯
	clear_screen(); 
	LED0=!LED0;
	delay_ms(100);
	LED0=!LED0;
	delay_ms(100); 
}

void PWR_show(){
	adcx=Get_Adc_Average(ADC_Channel_6,10);
	tempx=(float)adcx*(3.3/4096)*11;
//	temp=tempx*100;
//	Volta[6]=temp/1000%10+'0';
//	Volta[7]=temp/100%10+'0';
//	Volta[9]=temp/10%10+'0';
//	Volta[10]=temp%10+'0';
	if(tempx>=7.4)
	{
		disp_blink_16x16(1,113,0,0,dianchi100);
	}
	else if(tempx<7.4&&tempx>=7.0)
	{
		disp_blink_16x16(1,113,0,0,dianchi75);
	}
	else if(tempx<7.0&&tempx>=6.5)
	{
		disp_blink_16x16(1,113,0,0,dianchi50);
	}
	else if(tempx<6.5&&tempx>=6.1)
	{
		disp_blink_16x16(1,113,0,0,dianchi25);
	}
	else if(tempx<5.8)
	{
		disp_blink_16x16(1,113,0,0,dianchi0);
	}
}

void LCD_show(void)
{
//	clear_screen();  
	LCD12864_P8x16Str(1,1,"     HicBit");
	disp_blink_16x16(3,1,0,0,xiahuaxiang);
	disp_blink_16x16(3,17,0,0,xiahuaxiang);
	disp_blink_16x16(3,33,0,0,xiahuaxiang);
	disp_blink_16x16(3,49,0,0,xiahuaxiang);
	disp_blink_16x16(3,65,0,0,xiahuaxiang);
	disp_blink_16x16(3,81,0,0,xiahuaxiang);
	disp_blink_16x16(3,97,0,0,xiahuaxiang);
	disp_blink_16x16(3,113,0,0,xiahuaxiang);
}

void Power_off_button()
{
	int key;
	key=Long_Press(KEY0);
	if(key==1)					//长按3秒
	{
		clear_screen();
		LCD_show();
		LCD12864_P6x8Str(4,1,"About to shut down");
		delay_ms(500);
		power_flag=1;			//关机标志位
		backlight=1;			//关背光灯
		LED0=!LED0;
		delay_ms(500);
		LED0=!LED0;
		delay_ms(500); 
		Power_button=0;			//关机
	}
}



int main(void)
{	
	Init();						//各种初始化
	LCD_show();					//屏幕界面显示
	while(1){
		if(power_flag==0){
			uart();					//串口信息接收
			PWR_show();				//电池电量显示
		}
		else
			clear_screen();
//		printf("Target_Position:%d\r\n",Target_Position);
//		printf("Encoder_Left:%d\r\n",Encoder_Left);
//		printf("Encoder_Right:%d\r\n",Encoder_Right);
//		delay_ms(100);
	}	
}

/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
int TIM1_UP_IRQHandler(void) 
{    
	if(TIM1->SR&0X0001)//5ms定时中断		
	{   
		TIM1->SR		 &=  ~(1<<0);                             						//===清除定时器1中断标志位	
		Encoder_Left  =  Read_Position(2);                          					//===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right =  Read_Position(4);                         						//===读取编码器的值 
		if(Encoder_Left>Target_Position)
		{
			Flag_Foreward = 1;		//正转
			Flag_Reversal = 0;		//反转
		}
		else
		{
			Flag_Foreward = 0;		//正转
			Flag_Reversal = 1;    //反转
		}
		if(Encoder_Right<Target_Position)
		{
			Flag_Foreward = 1;		//正转
			Flag_Reversal = 0;		//反转
		}
		else
		{
			Flag_Foreward = 0;		//正转
			Flag_Reversal = 1;    //反转
		}
		Key();																		//===按键扫描	
		Moto1 = Position_PIDA(Encoder_Left,  Target_Position); 						//===计算左轮电机最终PWM
		Moto2 = Position_PIDB(Encoder_Right, Target_Position); 				        //===计算右轮电机最终PWM																							
		Xianfu_Pwm();                                                       		//===PWM限幅
		Set_Pwm(Moto1, Moto2);                                               		//===赋值给PWM寄存器  
		Power_off_button();			//关机键控制
	}       	
	 return 0;	  
} 

void EXTI9_5_IRQHandler(void)										//中
{			
	delay_ms(10);   //消抖			 
	if(KEY0==0){
		
	}
 	EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位  
}





