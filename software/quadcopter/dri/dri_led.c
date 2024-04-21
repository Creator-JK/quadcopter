#include "dri_led.h"
#include "stm32f10x.h"

void LEDInit()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_initTypeDef;
	GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initTypeDef);
	
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_initTypeDef);
}

void LEDOn()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
}

void LEDOff()
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);	
}
