#include "key.h"
#include "delay.h"

//������ʼ������ 
//PA0.15��PC5 ���ó�����
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTBʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PB5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB5
	
} 


/**************************************************************************
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������ 2��˫�� 
**************************************************************************/
u8 click_N_Double (u8 time,u8 KEY)
{
	static	u8 flag_key,count_key,double_key;	
	static	u16 count_single,Forever_count;
	
	if(KEY==0)  
		Forever_count++;   //������־λδ��1
	else        
		Forever_count=0;
	
	if(0==KEY&&0==flag_key)		
		flag_key=1;	
	
	if(0==count_key)
	{
		if(flag_key==1) 
		{
			double_key++;
			count_key=1;	
		}
		if(double_key==2) 
		{
			double_key=0;
			count_single=0;
			return 2;//˫��ִ�е�ָ��
		}
	}
	
	if(1==KEY)			
		flag_key=0,count_key=0;

	if(1==double_key)
	{
		count_single++;
		if(count_single>time&&Forever_count<time)
		{
			double_key=0;
			count_single=0;	
			return 1;//����ִ�е�ָ��
		}
		if(Forever_count>time)
		{
			double_key=0;
			count_single=0;	
		}
	}	
	return 0;
}

/**************************************************************************
�������ܣ��������
��ڲ�������
����  ֵ������״̬ 0���޶��� 1������2s
**************************************************************************/
u8 Long_Press(u8 KEY)
{
	static u16 Long_Press_count,Long_Press;
	
	if(Long_Press==0&&KEY==0)  
		Long_Press_count++;   				//������־λδ��1
	else                       
		Long_Press_count=0; 
	
	if(Long_Press_count>200)		
	{
		Long_Press=1;	
		Long_Press_count=0;
		return 1;
	}				
	
	if(Long_Press==1)     					//������־λ��1
	{
		Long_Press=0;
	}
	
	return 0;
}


