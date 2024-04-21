#include "sys.h"
#include "freeRTOS.h"
#include "task.h"
#include "sys_task.h"
#include "stm32f10x.h"
#include "dri_serial.h"
#include "dri_mpu6050.h"
#include "dri_nrf24l01.h"
#include "dri_adc.h"

uint8_t sysState = 0x00;
void SetSysState(uint8_t index, uint8_t newState)
{
	if (index <= 3)
	{
		sysState &= (~(0x01 << index));
		sysState |= (newState << index);
	} else if ((index >= 4) && (index <= 7))
	{
		sysState &= (~(0x03 << index));
		sysState |= (newState << index);
	} else
	{
		;
	}
}
uint8_t GetSysState(uint8_t index)
{
	if (index <= 3)
	{
		return ((sysState & (0x01 << index)) >> index);

	} else if ((index >= 4) && (index <= 7))
	{
		return ((sysState & (0x03 << index)) >> index);

	} else
	{
		return 0x00;
	}
}
uint8_t GetSysStates()
{
	return sysState;
}

float blancePosture[6] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F };
void SetBlancePosture(uint8_t index, float newVal)
{
	blancePosture[index] = newVal;
}
float GetBlancePosture(uint8_t index)
{
	return blancePosture[index];
}

float posture[6] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F };
float GetPosture(uint8_t index)
{
	return posture[index];
}
void SetPosture(uint8_t index, float newVal)
{
	posture[index] = newVal;
}

uint16_t motorSpeed[4] = { 0, 0, 0, 0 };
void SetMotorSpeed(uint8_t index, uint16_t newVal)
{
	motorSpeed[index] = newVal;
	TIM_SetCompare1(TIM2, motorSpeed[MOTOR_TL]);
	TIM_SetCompare2(TIM2, motorSpeed[MOTOR_TR]);
	TIM_SetCompare3(TIM2, motorSpeed[MOTOR_BL]);
	TIM_SetCompare4(TIM2, motorSpeed[MOTOR_BR]);
}
uint16_t GetMotorSpeed(uint8_t index)
{
	return motorSpeed[index];
}

float dumpEnergy = 0.0F;
void SetDumpEnergy(float newVal)
{
	dumpEnergy = newVal / DUMP_ENERGY_MAX;
	SetTxMsg(TXMSG_VBAT, Round(dumpEnergy * 100));
}
float GetDumpEnergy()
{
	return dumpEnergy;
}

uint8_t rxMsg[MSG_LEN] = { NULL, NULL };
uint8_t GetRxMsg(uint8_t index)
{
	nRF_RxMode();
	nRF_RxPacket(rxMsg);

	if (index <= 7)
	{
		return (rxMsg[0] >> index) & 0x03;

	} else if ((index >= 8) & (index <= 15))
	{
		return (rxMsg[1] >> ((index - 8) & 0x03));

	} else
	{
		return 0;
	}

}

uint8_t txMsg[MSG_LEN] = { NULL, NULL };
void SetTxMsg(uint8_t index, uint8_t newVal)
{
	nRF_TxMode();

	if (index <= 7)
	{
		txMsg[0] = newVal;

	} else if ((index >= 8) & (index <= 13))
	{
		txMsg[1] &= 0xC0;
		txMsg[1] |= newVal;

	} else if ((index >= 14) & (index <= 15))
	{
		txMsg[1] &= 0x63;
		txMsg[1] |= (newVal << (index - 8));
	}

	nRF_TxPacket(txMsg);
}

uint8_t sysWarning = 0x00;
uint8_t GetSysWarning()
{
	return sysWarning;
}
void SetSysWarning(uint8_t newVal)
{
	sysWarning = newVal;
}

void SYS_Delay_us(uint32_t us)
{
	uint16_t i = 0;
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
	uint32_t res = 1;
	while (y--)
	{
		res *= x;
	}
	return res;
}

uint8_t Round(float val)
{
	return (uint8_t)(val + 0.5F);
}
