#include "sys.h"
#include "freeRTOS.h"
#include "task.h"
#include "sys_task.h"
#include "stm32f10x.h"
#include "dri_serial.h"
#include "dri_nrf24l01.h"

uint8_t rxMsg[MSG_LEN] = { NULL, NULL };
uint8_t GetRxMsg(uint8_t index)
{
	if (index <= 7)
		return rxMsg[0];
	else if ((index >= 8) && (index <= 13))
	{
		return ((rxMsg[1] >> (index - 8)) & 0x3F);
	} else if ((index >= 14) && (index <= 15))
	{
		return (rxMsg[1] >> (index - 8) & 0x03);
	} else
	{
		;
	}
}

uint8_t txMsg[MSG_LEN] = { NULL, NULL };
void SetTxMsg(uint8_t index, uint8_t newVal)
{
	if (index <= 7)
	{
		txMsg[0] &= (~(0x03 << index));
		txMsg[0] |= (newVal << index);
	} else if ((index >= 8) && (index <= 15))
	{
		txMsg[1] &= (~(0x03 << (index - 8)));
		txMsg[1] |= (newVal << (index - 8));
	}
}

void SysInit()
{
	nRF_Init();
}

void SYS_Delay_us(uint32_t us)
{
	uint8_t i;

	while (us--)
	{
		i = 10;
		while (i--);
	}
}

void SYS_Delay_ms(uint32_t ms)
{
	while (ms--)
	{
		SYS_Delay_us(1000);
	}
}

void SYS_Delay_s(uint32_t s)
{
	while (s--)
	{
		SYS_Delay_ms(1000);
	}
}

uint32_t MathPow(uint32_t x, uint32_t y)
{
	uint32_t result = 1;
	while (y--)
	{
		result *= x;
	}
	return result;
}
