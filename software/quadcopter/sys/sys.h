#ifndef __SYS_H__
#define __SYS_H__

#include "stm32f10x.h"

#define SOFTWARE_MODE_SERIAL	1
#define SOFTWARE_MODE_ANOTECH	2
#define SOFTWARE_MODE_RELEASE	3
#define SOFTWARE_MODE			SOFTWARE_MODE_RELEASE

#define TASK_STACK_SIZE	configMINIMAL_STACK_SIZE
#define TASK_PRIORITY	(tskIDLE_PRIORITY + 1)

#define SYS_DRI_MPU6050	    0
#define SYS_DRI_NRF24L01	1
#define SYS_DRI_MOTOR		2
#define SYS_DRI_VBAT		3
#define SYS_DRI_LED			4
#define SYS_DRI_SELF		6
#define SYS_DRI_ENABLE		1
#define SYS_DRI_DISABLE		0

#define SYS_FLY_SPEED	10

#define POSTURE_SPEED_X		0
#define POSTURE_SPEED_Y		1
#define POSTURE_SPEED_Z		2
#define POSTURE_PITCH		3
#define POSTURE_YAW			4
#define POSTURE_ROLL		5

#define MOTOR_TL    0
#define MOTOR_TR    1
#define MOTOR_BL    2
#define MOTOR_BR    3

#define MSG_LEN		2

#define DUMP_ENERGY_MAX		64.0F

#define WARNING_TYPE_LOW_POWER		0
#define WARNING_TYPE_Instability	1
#define WARNING_TYPE_WEAK_SIGNAL	2
#define WARNING_TYPE_SYS_ERR		3

#define TXMSG_SYSSTATE		0
#define TXMSG_VBAT			8
#define TXMSG_SYS_WARNING	14

#define RXMSG_FLY_CTRL_UD	0
#define RXMSG_FLY_CTRL_RL	2
#define RXMSG_FLY_CTRL_BF	4
#define RXMSG_SYS_CTRL		6
#define RXMSG_SYS_LED_CTRL	8

#define LED_SHINE_TIM       100

void SetSysState(uint8_t index, uint8_t newState);
uint8_t GetSysState(uint8_t pos);
uint8_t GetSysStates(void);
float GetPosture(uint8_t index);
void SetPosture(uint8_t index, float newVal);
uint8_t GetSysWarning(void);
void SetSysWarning(uint8_t newVal);
void SetMotorSpeed(uint8_t index, uint16_t newVal);
void SetDumpEnergy(float newVal);
float GetDumpEnergy(void);
uint16_t GetMotorSpeed(uint8_t index);
uint8_t GetRxMsg(uint8_t index);
void SetTxMsg(uint8_t index, uint8_t newVal);
float GetBlancePosture(uint8_t index);
void SetBlancePosture(uint8_t index, float newVal);

void SYS_Delay_s(uint32_t s);
void SYS_Delay_ms(uint32_t ms);
void SYS_Delay_us(uint32_t us);
uint32_t MathPow(uint32_t x, uint32_t y);
uint8_t Round(float val);

#endif /* __SYS_H__ */
