#include "sys.h"
#include "freeRTOS.h"
#include "task.h"
#include "sys_task.h"
#include "dri_nrf24l01.h"

int main(void)
{		
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	
	SysTaskInit();
	
	vTaskStartScheduler();
}
