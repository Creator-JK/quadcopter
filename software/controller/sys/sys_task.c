#include "sys_task.h"
#include "stm32f10x.h"
#include "math.h"
#include "dri_serial.h"
#include "freeRTOS.h"
#include "task.h"
#include "dri_nrf24l01.h"
#include "sys.h"

static void vTaskCommunication(void* args)
{
	while (1)
	{
		
	}
}

TaskHandle_t vTaskCommunicationHandle = NULL;
void vTaskInit()
{
	xTaskCreate(vTaskCommunication, "Communication", TASK_STACK_SIZE, NULL, TASK_PRIORITY, &vTaskCommunicationHandle);
}
