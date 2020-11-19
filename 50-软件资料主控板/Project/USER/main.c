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
int Encoder_Left, Encoder_Right;            //=====���ұ��������������
int Moto1, Moto2;                           //=====���PWM���� Ӧ��Motor�� ��Moto�¾�	
int Target_Position = 0; 								//=====���������������
int Position_Amplitude = 45*5.35;							//=====���� 
int tmp = 0;																//=====������������
int Flag_Foreward = 0, Flag_Reversal = 0;		//=====����ת��־λ
int power_flag=0;

void uart(void){
	u8 t;
	u8 len,Cache;	
//	u16 times=0;
	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;				//�õ��˴ν��յ������ݳ���
		for(t=0;t<len;t++)
		{
			USART1->DR=USART_RX_BUF[t];
			while((USART1->SR&0X40)==0);		//�ȴ����ͽ���
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
				case 0:Moto1_turn=1;Moto2_turn=1;break;//���1:�� ���2:��
				case 1:Moto1_turn=-1;Moto2_turn=1;break;//���1:�� ���2:��
				case 2:Moto1_turn=1;Moto2_turn=-1;break;//���1:�� ���2:��
				case 3:Moto1_turn=-1;Moto2_turn=-1;break;//���1:�� ���2:��
				default:break;
			}
			if(USART_RX_BUF[1]!=4){
				if(USART_RX_BUF[4]==0)					//port1
					Set_Pwm(USART_RX_BUF[3] * Moto2_turn,USART_RX_BUF[2] * -Moto1_turn);
				else if(USART_RX_BUF[4]==1)					//port2
					Set_Pwm2(USART_RX_BUF[3] * Moto2_turn,USART_RX_BUF[2] * -Moto1_turn);
			}
			else if(USART_RX_BUF[1]==4)			//���ֹͣ��־λ
			{
				Stop_motor(USART_RX_BUF[4]);
			}
		}
		else if(USART_RX_BUF[0]==0x0F){
			clear_screen();
			LCD_show();
		}
		LED0=!LED0;				//��˸LED,��ʾϵͳ��������.
		delay_ms(10);
//		printf("\n");							//���뻻��
		USART_RX_STA=0;
	}else
	{
//		Set_Pwm(0,0);
//		Set_Pwm2(0,0);
//		times++;
//		printf("123");
//		if(times%30==0)LED0=!LED0;				//��˸LED,��ʾϵͳ��������.
//		delay_ms(100);   
	}
}

void Init(void){
	delay_init();	    	 						//��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// �����ж����ȼ�����2
	Power_off_button_Init();
	Power_button=1;									//����
	uart_init(115200);							 	//���ڳ�ʼ��Ϊ115200
	initial_lcd();
	PWM_Init(260,0);
	PWM_Init2(260,0);
	Encoder_Init_TIM2();            				//=====�������ӿ�
    Encoder_Init_TIM4();
	EXTIX_Init();
	Timer1_Init(99,7199);
	Adc_Init();
	LED_Init();		  	 							//��ʼ����LED���ӵ�Ӳ���ӿ� 
	backlight_Init();
	backlight=0;									//�������
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
	if(key==1)					//����3��
	{
		clear_screen();
		LCD_show();
		LCD12864_P6x8Str(4,1,"About to shut down");
		delay_ms(500);
		power_flag=1;			//�ػ���־λ
		backlight=1;			//�ر����
		LED0=!LED0;
		delay_ms(500);
		LED0=!LED0;
		delay_ms(500); 
		Power_button=0;			//�ػ�
	}
}



int main(void)
{	
	Init();						//���ֳ�ʼ��
	LCD_show();					//��Ļ������ʾ
	while(1){
		if(power_flag==0){
			uart();					//������Ϣ����
			PWR_show();				//��ص�����ʾ
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
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int TIM1_UP_IRQHandler(void) 
{    
	if(TIM1->SR&0X0001)//5ms��ʱ�ж�		
	{   
		TIM1->SR		 &=  ~(1<<0);                             						//===�����ʱ��1�жϱ�־λ	
		Encoder_Left  =  Read_Position(2);                          					//===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right =  Read_Position(4);                         						//===��ȡ��������ֵ 
		if(Encoder_Left>Target_Position)
		{
			Flag_Foreward = 1;		//��ת
			Flag_Reversal = 0;		//��ת
		}
		else
		{
			Flag_Foreward = 0;		//��ת
			Flag_Reversal = 1;    //��ת
		}
		if(Encoder_Right<Target_Position)
		{
			Flag_Foreward = 1;		//��ת
			Flag_Reversal = 0;		//��ת
		}
		else
		{
			Flag_Foreward = 0;		//��ת
			Flag_Reversal = 1;    //��ת
		}
		Key();																		//===����ɨ��	
		Moto1 = Position_PIDA(Encoder_Left,  Target_Position); 						//===�������ֵ������PWM
		Moto2 = Position_PIDB(Encoder_Right, Target_Position); 				        //===�������ֵ������PWM																							
		Xianfu_Pwm();                                                       		//===PWM�޷�
		Set_Pwm(Moto1, Moto2);                                               		//===��ֵ��PWM�Ĵ���  
		Power_off_button();			//�ػ�������
	}       	
	 return 0;	  
} 

void EXTI9_5_IRQHandler(void)										//��
{			
	delay_ms(10);   //����			 
	if(KEY0==0){
		
	}
 	EXTI_ClearITPendingBit(EXTI_Line5);    //���LINE5�ϵ��жϱ�־λ  
}





