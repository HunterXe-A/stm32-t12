#include "adc.h"
#include "pid.h"
#include "math.h"

u16 TEMP_T12;

u16 Get_Adc(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	return ADC_GetConversionValue(ADC1);
}

void Get_T12_Adc(void)
{
    u16 readAdc,i=2048;
    TIM_SetCompare(TIM2,0);
    while(i--);
    readAdc = Get_Adc(6)
    if(readAdc<4096) Adc_t12 = readAdc;
    TIM_SetCompare(TIM2,Output);
}

void Change_T12_Temp(void)
{
    
}
