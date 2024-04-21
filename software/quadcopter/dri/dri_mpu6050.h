#ifndef _DRI_MPU6050_H_
#define _DRI_MPU6050_H_

#include "stm32f10x.h"

/**
 * @brief MPU6050指令
 */
#define MPU6050_ADDR 			0xD0
#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CFG		0x1B
#define	MPU6050_ACCEL_CFG		0x1C
#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

#define MPU6050_START_CNT		123

/**
 * @brief MPU6050初始化
 */
void MPU6050_Init(void);

/**
 * @brief MPU6050读取reg值
 * @param uint8_t regAddress 要读去reg的地址
 * @retval uint8_t 读取到的数据
 */
uint8_t MPU6050_Read_REG(uint8_t regAddress);

/**
 * @brief MOU6050写reg
 * @param uint8_t regAddress 	目标reg的地址
 * @param uint8_t data 		要写入的数据
*/
void MPU6050_Write_REG(uint8_t regAddress, uint8_t data);

/**
 * @brief MPU6050获取模块ID
 */
uint8_t MPU6050_Check(void);

/**
 * @brief 获取MPU6050的数据
 */
void MPU6050_Get_Data(int16_t* AccX, int16_t* AccY, int16_t* AccZ, int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ);

/**
 * @brief MPU6050获取启动时的姿态信息
 */
void MPU6050GetStartPosture(void);

#endif
