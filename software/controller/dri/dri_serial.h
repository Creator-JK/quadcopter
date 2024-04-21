#ifndef _DRI_SERIAL_H_
#define _DRI_SERIAL_H_

#include "stm32f10x.h"

/**
 * @brief USART初始化
 */
void Serial_Init(void);

/**
 * @brief 使用USART发送数据
 * @param uint8_t* array 要发送的目标数据
 * @param	uint8_t length 目标数据的长度
 */
void Serial_Send_Array(uint8_t* array, uint8_t length);

/**
 * @brief 使用USART发送字符串
 * @param char* string 要发送的目标字符串
 */
void Serial_Send_String(char* string);

/**
 * @brief 使用USART发送数字
 * @param 要发送的目标数字
 * @param 目标数字的长度
 */
void Serial_Send_Number(unsigned int number, uint8_t length);

void Serial_Printf(char *format, ...);

void Serial_Send_Float(float num);

#endif
