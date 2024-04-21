#if ((SOFTWARE_MODE == SOFTWARE_MODE_SERIAL) || (SOFTWARE_MODE == SOFTWARE_MODE_ANOTECH))
	#include "dri_serial.h"
	#include "stm32f10x.h"
	#include "string.h"
	#include "stdio.h"
	#include "stdarg.h"
	#include "sys.h"
	
	uint8_t rxData;
	uint8_t rxFlag;

	void Serial_Init()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitTypeDef GPIO_initTypeDef;
		USART_InitTypeDef USART_initTypeDef;

		GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_9;
		GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_initTypeDef);
		
		GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_10;
		GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_initTypeDef);
		
		USART_initTypeDef.USART_BaudRate = 9600;
		USART_initTypeDef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_initTypeDef.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_initTypeDef.USART_Parity = USART_Parity_No;
		USART_initTypeDef.USART_StopBits = USART_StopBits_1;
		USART_initTypeDef.USART_WordLength = USART_WordLength_8b;
		USART_Init(USART1, &USART_initTypeDef);
			
		USART_Cmd(USART1, ENABLE);
	}

	void Serial_Send_Byte(uint8_t byte)
	{
		USART_SendData(USART1, byte);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}

	void Serial_Send_Array(uint8_t *array, uint8_t length)
	{
		uint8_t i;
		for(i = 0; i < length; i++)
		{
			Serial_Send_Byte(array[i]);
		}
	}

	void Serial_Send_String(char *string)
	{
		uint8_t i;
		for(i = 0; string[i] != '\0'; i++)
		{
			Serial_Send_Byte(string[i]);
		}
	}

	void Serial_Send_Number(unsigned int number, uint8_t length)
	{
		uint8_t i;
		for(i = 0; i < length; i++)
		{
			Serial_Send_Byte(number / MathPow(10, length - i - 1) % 10 + '0');
		}
	}

	uint8_t Serial_Get_rxFlag()
	{
		if(rxFlag == 1)
		{
			rxFlag = 0;
			return 1;
		}
		return 0;
	}

	uint8_t Serial_Get_rxData()
	{
		return rxData;
	}

	int fputc(int ch, FILE *f)
	{	
		while((USART1->SR & 0x40) == 0);
		USART1->DR = (uint8_t)ch;
		
		return ch;
	}

	void SerialSendDatI(uint8_t *dat, uint8_t len)
	{
		for(uint8_t i = 0; i < len; i++)
		{
			Serial_Send_Number(dat[i], 4);
		}
	}

	void SerialSendDatF(float *dat, uint8_t len)
	{
		for(uint8_t i = 0; i < len; i++)
		{
			Serial_Send_Float(dat[i]);
		}
	}

	void Serial_Printf(char *format, ...)
	{
		char String[100];
		va_list arg;
		va_start(arg, format);
		vsprintf(String, format, arg);
		va_end(arg);
		Serial_Send_String(String);
	}

	void Serial_Send_Float(float num)
	{
		if(num < 0)
		{
			num = -num;
			Serial_Printf("-%d.%d\n", (uint32_t)num, (uint32_t)((float)(num - (uint32_t)num) * 1000));
		}
		else
		{
			Serial_Printf("+%d.%d\n", (uint32_t)num, (uint32_t)((float)(num - (uint32_t)num) * 1000));
		}
		
	}
#endif
