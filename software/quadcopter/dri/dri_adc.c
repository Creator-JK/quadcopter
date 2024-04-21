#include "dri_adc.h"
#include "stm32f10x.h"
#include "sys.h"

void ADCInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	GPIO_InitTypeDef GPIO_initTypeDef;
	GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_7;
	GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initTypeDef);

	ADC_InitTypeDef ADC_initTypeDef;
	ADC_initTypeDef.ADC_Mode = ADC_Mode_Independent;
	ADC_initTypeDef.ADC_ScanConvMode = DISABLE;
	ADC_initTypeDef.ADC_ContinuousConvMode = DISABLE;
	ADC_initTypeDef.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_initTypeDef.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_initTypeDef.ADC_NbrOfChannel = ADC_CHAN_NUM;
	ADC_Init(ADC1, &ADC_initTypeDef);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
}

uint16_t ADC_GetVal()
{
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_7, ADC_CONV_RANK, ADC_SampleTime_239Cycles5);
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//	
	//while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	//return ADC_GetConversionValue(ADC1)
	
	return 0;
}
