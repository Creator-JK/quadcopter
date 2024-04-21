#include "stm32f10x.h"
#include "sys.h"
#include "freeRTOS.h"
#include "task.h"
#include "dri_serial.h"
#include "sys_task.h"
#include "dri_nrf24l01.h"

uint8_t checkResult;
int main(void)
{	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	xTaskCreate(vTaskReceiveMsg, NULL, TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);

	vTaskStartScheduler();
}
