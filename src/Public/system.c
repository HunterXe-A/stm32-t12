#include "system.h"

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������
/******************************����˵��************************************
* �� �� ��         : SysTick_Init
* ��������		     : ��ʼ���ӳٺ���,SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
* ��    ��         : SYSCLK:ϵͳʱ��Ƶ��(Ĭ��72)
* ��    ��         : ��
*******************************************************************************/
void SysTick_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;					
	fac_ms=(u16)fac_us*1000;				   
}	


/********************************����˵��***************************************
* �� �� ��         : delay_us
* ��������		   	 : us��ʱ��
* ��    ��         : nus��Ҫ��ʱ��us��(��72M������,nms<=1864*1000 )
* ��    ��         : ��
*******************************************************************************/		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}

/********************************����˵��***************************************
* �� �� ��         : delay_ms
* ��������		     : ms��ʱ��
* ��    ��         : nms��Ҫ��ʱ��ms��
					ע��:nms��ֵ,SysTick->LOADΪ24λ�Ĵ�����
					��Ҫ����0xffffff*8*1000/SYSCLK
					��72M������,nms<=1864ms 
* ��    ��         : ��
*******************************************************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	    
} 

void delay_Ms(u16 nms)
{	
  int i;	
	for(i=1;i<=(nms/1000);i++)
	{
		delay_ms(1000);
	}
	if(nms-1000*i>0)
	{
		delay_ms(nms-1000*i);		
	}
} 


