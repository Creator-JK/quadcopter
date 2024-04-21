#include "dri_motor.h"
#include "stm32f10x.h"
#include "sys.h"

#define startSpeed	10

void MotorInit()
{
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_initTypeDef;
	GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_initTypeDef);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_timeBaseInitTypeDef;
	TIM_timeBaseInitTypeDef.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_timeBaseInitTypeDef.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_timeBaseInitTypeDef.TIM_Period = 100 - 1;		//ARR
	TIM_timeBaseInitTypeDef.TIM_Prescaler = 36 - 1;		//PSC
	TIM_timeBaseInitTypeDef.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_timeBaseInitTypeDef);
	
	TIM_OCInitTypeDef TIM_ocInitTypeDef;
	TIM_ocInitTypeDef.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_ocInitTypeDef.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_ocInitTypeDef.TIM_OutputState = TIM_OutputState_Enable;
	TIM_ocInitTypeDef.TIM_Pulse = 0;		//CCR
	TIM_OCStructInit(&TIM_ocInitTypeDef);
	
	TIM_OC1Init(TIM2, &TIM_ocInitTypeDef);
	TIM_OC1PreloadConfig(TIM2, ENABLE);
	
	TIM_OC2Init(TIM2, &TIM_ocInitTypeDef);
	TIM_OC2PreloadConfig(TIM2, ENABLE);
	
	TIM_OC3Init(TIM2, &TIM_ocInitTypeDef);
	TIM_OC3PreloadConfig(TIM2, ENABLE);
	
	TIM_OC4Init(TIM2, &TIM_ocInitTypeDef);
	TIM_OC4PreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);
}

void MotorStart()
{
	uint16_t speed = 0;
	
	for(uint16_t i = 0; i < 100; i++)
	{
		speed += startSpeed;

		SetMotorSpeed(MOTOR_TL, speed);
		SetMotorSpeed(MOTOR_TR, speed);
		SetMotorSpeed(MOTOR_BL, speed);
		SetMotorSpeed(MOTOR_BR, speed);

		SYS_Delay_ms(1);
	}
}

void MotorStop()
{
	SetMotorSpeed(MOTOR_TL, 0);
	SetMotorSpeed(MOTOR_TR, 0);
	SetMotorSpeed(MOTOR_BL, 0);
	SetMotorSpeed(MOTOR_BR, 0);
}
