#ifndef __SYS_H__
#define __SYS_H__

#include "stm32f10x.h"

#define MSG_LEN     2

#define TASK_STACK_SIZE	configMINIMAL_STACK_SIZE
#define TASK_PRIORITY	(tskIDLE_PRIORITY + 1)

void SYS_Delay_s(uint32_t s);
void SYS_Delay_ms(uint32_t ms);
void SYS_Delay_us(uint32_t us);
uint32_t MathPow(uint32_t x, uint32_t y);

#endif /* __SYS_H__ */
