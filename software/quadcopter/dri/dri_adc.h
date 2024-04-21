#ifndef __DRI_ADC_H_
#define __DRI_ADC_H_

#include "stm32f10x.h"

#define ADC_CHAN_NUM	1
#define ADC_CONV_RANK	1
#define ADC_MAX_VBAT    1000

void ADCInit(void);
uint16_t ADC_GetVal(void);

#endif /* __DRI_ADC_H_ */
