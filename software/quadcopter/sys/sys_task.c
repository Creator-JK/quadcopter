#include "sys_task.h"
#include "dri_mpu6050.h"
#include "stm32f10x.h"
#include "math.h"
#include "dri_serial.h"
#include "dri_nrf24l01.h"
#include "freeRTOS.h"
#include "task.h"
#include "dri_motor.h"
#include "sys.h"
#include "dri_led.h"
#include "dri_adc.h"

static void vTaskSysCheck(void* args)
{
	while (1)
	{
		uint8_t cResMPU6050, cResNrf24l01;

		taskENTER_CRITICAL();

		cResMPU6050 = MPU6050_Check();

		cResNrf24l01 = nRF_Check();

		taskEXIT_CRITICAL();

		if (cResMPU6050 == SYS_DRI_ENABLE)
			SetSysState(SYS_DRI_MPU6050, SYS_DRI_ENABLE);
		else
			SetSysState(SYS_DRI_MPU6050, SYS_DRI_DISABLE);

		if (cResNrf24l01 == SYS_DRI_ENABLE)
			SetSysState(SYS_DRI_NRF24L01, SYS_DRI_ENABLE);
		else
			SetSysState(SYS_DRI_NRF24L01, SYS_DRI_DISABLE);

		taskYIELD();
	}
}

static void vTaskGetMPU6050Data(void* args)
{
	while (1)
	{
		if (GetSysState(SYS_DRI_MPU6050) == SYS_DRI_ENABLE)
		{
			int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;

			taskENTER_CRITICAL();

			MPU6050_Get_Data(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);

			taskEXIT_CRITICAL();

			SetPosture(POSTURE_SPEED_X, -gyroY / 16.4F);
			SetPosture(POSTURE_SPEED_Y, -gyroX / 16.4F);
			SetPosture(POSTURE_SPEED_Z, -gyroZ / 16.4F);
			SetPosture(POSTURE_PITCH, -atan((float)(accX) / (float)(accZ)) * 57.2974F);
			SetPosture(POSTURE_YAW, -atan((float)(accX) / (float)(accY)) * 57.2974F);
			SetPosture(POSTURE_ROLL, -atan((float)(accY) / (float)(accZ)) * 57.2974F);

			taskYIELD();

		} else
		{
			taskYIELD();
		}
	}
}

static void vTaskCommunication(void* args)
{
	while (1)
	{
		if (GetSysState(SYS_DRI_NRF24L01) == SYS_DRI_ENABLE)
		{
			uint8_t rxMsgFlyCtrlUD, rxMsgFlyCtrlRL, rxMsgFlyCtrlBF, rxMsgSysCtrl;

			taskENTER_CRITICAL();

			SetTxMsg(TXMSG_SYSSTATE, GetSysStates());
			SetTxMsg(TXMSG_VBAT, round(GetDumpEnergy() * 64));
			SetTxMsg(TXMSG_SYS_WARNING, GetSysWarning());

			// rxMsgFlyCtrlUD = GetRxMsg(RXMSG_FLY_CTRL_UD);
			// rxMsgFlyCtrlRL = GetRxMsg(RXMSG_FLY_CTRL_RL);
			// rxMsgFlyCtrlBF = GetRxMsg(RXMSG_FLY_CTRL_BF);
			// rxMsgSysCtrl = GetRxMsg(RXMSG_SYS_CTRL);

			taskEXIT_CRITICAL();

			switch (rxMsgFlyCtrlUD)
			{
			case 0:	break;
			case 1:	SetBlancePosture(POSTURE_SPEED_Z, -SYS_FLY_SPEED);	break;
			case 2:	SetBlancePosture(POSTURE_SPEED_Z, SYS_FLY_SPEED);	break;
			case 3:	break;
			}

			switch (rxMsgFlyCtrlRL)
			{
			case 0:	break;
			case 1:	SetBlancePosture(POSTURE_SPEED_Y, SYS_FLY_SPEED);	break;
			case 2:	SetBlancePosture(POSTURE_SPEED_Y, -SYS_FLY_SPEED);	break;
			case 3:	break;
			}

			switch (rxMsgFlyCtrlBF)
			{
			case 0:	break;
			case 1:	SetBlancePosture(POSTURE_SPEED_X, SYS_FLY_SPEED);	break;
			case 2:	SetBlancePosture(POSTURE_SPEED_X, -SYS_FLY_SPEED);	break;
			case 3:	break;
			}

			switch (rxMsgSysCtrl)
			{
			case 0:	break;
			case 1:	SetBlancePosture(POSTURE_SPEED_X, SYS_FLY_SPEED);	break;
			case 2:	SetBlancePosture(POSTURE_SPEED_X, -SYS_FLY_SPEED);	break;
			case 3:	break;
			}
			taskYIELD();

		} else
		{
			taskYIELD();
		}
	}
}

#if (SOFTWARE_MODE == SOFTWARE_MODE_SERIAL)
static void vTaskGetBugs(void* args)
{
	while (1)
	{
		;
	}
}
#elif (SOFTWARE_MODE == SOFTWARE_MODE_ANOTECH)
static void vTaskGetBugs(void* args)
{
	while (1)
	{
		;
	}
}
#elif (SOFTWARE_MODE == SOFTWARE_MODE_RELEASE)

#endif

static float k = 10.0F, err;
static void vTaskFlyCtrl(void* args)
{
	while (1)
	{
		err = GetPosture(POSTURE_ROLL) - GetBlancePosture(POSTURE_ROLL);
		SetMotorSpeed(MOTOR_BL, GetMotorSpeed(MOTOR_BL) + err * k);
		SetMotorSpeed(MOTOR_TL, GetMotorSpeed(MOTOR_TL) + err * k);
		SetMotorSpeed(MOTOR_BR, GetMotorSpeed(MOTOR_BR) - err * k);
		SetMotorSpeed(MOTOR_TR, GetMotorSpeed(MOTOR_TR) - err * k);

		err = GetPosture(POSTURE_PITCH) - GetBlancePosture(POSTURE_PITCH);
		SetMotorSpeed(MOTOR_BL, GetMotorSpeed(MOTOR_BL) - err * k);
		SetMotorSpeed(MOTOR_TL, GetMotorSpeed(MOTOR_TL) + err * k);
		SetMotorSpeed(MOTOR_BR, GetMotorSpeed(MOTOR_BR) - err * k);
		SetMotorSpeed(MOTOR_TR, GetMotorSpeed(MOTOR_TR) + err * k);

		err = GetPosture(POSTURE_YAW) - GetPosture(POSTURE_YAW);
		SetMotorSpeed(MOTOR_BL, GetMotorSpeed(MOTOR_BL) + err * k);
		SetMotorSpeed(MOTOR_TL, GetMotorSpeed(MOTOR_TL) - err * k);
		SetMotorSpeed(MOTOR_BR, GetMotorSpeed(MOTOR_BR) - err * k);
		SetMotorSpeed(MOTOR_TR, GetMotorSpeed(MOTOR_TR) + err * k);
	}
}

static void vTaskLedCtrl(void* args)
{
	uint8_t flag = 0;
	uint8_t t = 0;
	while (1)
	{
		uint8_t s = GetSysState(SYS_DRI_LED);

		switch (s)
		{
		case 0:	LEDOff();	break;
		case 1:
			LEDOn();
			vTaskDelay(LED_SHINE_TIM);
			LEDOff();
			break;
		case 2:
			LEDOff();
			if (flag == 0)
			{
				t++;
				if (t > LED_SHINE_TIM)	flag = 1;

			} else
			{
				t--;
				if (t <= 0)	flag = 0;
			}
			vTaskDelay(t);
			LEDOn();
			break;
		case 3:	LEDOn();	break;
		}
	}
}

static void vTaskVbatDetect(void* args)
{
	while (1)
	{
		taskENTER_CRITICAL();

		SetDumpEnergy((float)(ADC_GetVal() / ADC_MAX_VBAT));

		taskEXIT_CRITICAL();

		taskYIELD();
	}
}

TaskHandle_t vTaskSysCheckHandle = NULL;
TaskHandle_t vTaskGetMPU6050DataHandle = NULL;
TaskHandle_t vTaskCommunicationHandle = NULL;
#if (SOFTWARE_MODE == SOFTWARE_MODE_SERIAL) || (SOFTWARE_MODE == SOFTWARE_MODE_SERIAL)
TaskHandle_t vTaskGetBugsHandle = NULL;
#endif
TaskHandle_t vTaskFlyCtrlHandle = NULL;
TaskHandle_t vTaskLedCtrlHandle = NULL;
TaskHandle_t vTaskVbatDetectHandle = NULL;
void SysTaskInit()
{
	MotorInit();
	MPU6050_Init();
	nRF_Init();
	MotorStart();
	ADCInit();

	xTaskCreate(vTaskSysCheck, "SysCheck", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskSysCheckHandle);
	xTaskCreate(vTaskGetMPU6050Data, "MPU6050Data", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskGetMPU6050DataHandle);
	xTaskCreate(vTaskCommunication, "Communication", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskCommunicationHandle);
	xTaskCreate(vTaskFlyCtrl, "FlyCtrl", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskFlyCtrlHandle);
	xTaskCreate(vTaskLedCtrl, "LedCtrl", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskLedCtrlHandle);
	xTaskCreate(vTaskVbatDetect, "VbatDetect", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskVbatDetectHandle);

#if (SOFTWARE_MODE == SOFTWARE_MODE_SERIAL) || (SOFTWARE_MODE == SOFTWARE_MODE_SERIAL)
	Serial_Init();
	xTaskCreate(vTaskGetBugs, "GetBugs", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskGetBugsHandle);
#endif
}
