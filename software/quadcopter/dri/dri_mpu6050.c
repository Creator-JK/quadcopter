#include "dri_mpu6050.h"
#include "stm32f10x.h"
#include "sys.h"
#include "math.h"

#define MPU6050_Write_SCL(x)	GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define MPU6050_Write_SDA(x)	GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))

/**
 * @brief MPU6050读取SDA数据
 */
uint8_t MPU6050_Read_SDA()
{
	uint8_t bit = 0x00;
	bit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	return bit;
}

/**
 * @brief MPU6050引脚初始化
 */
void MPU6050_GPIO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_initTypeDef;
	GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_initTypeDef);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
}

/**
 * @brief MPU6050 IIC通信开始
 */
void MPU6050_IIC_Start()
{
	MPU6050_Write_SDA(1);
	MPU6050_Write_SCL(1);
	MPU6050_Write_SDA(0);
	MPU6050_Write_SCL(0);
}

/**
 * @brief MPU6050 IIC通信结束
 */
void MPU6050_IIC_Stop()
{
	MPU6050_Write_SDA(0);
	MPU6050_Write_SCL(1);
	MPU6050_Write_SDA(1);
}

/**
 * @brief MPU6050 IIC发送一字节数据 
 */
void MPU6050_IIC_Send_Byte(uint8_t byt)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		MPU6050_Write_SDA(byt & (0x80 >> i));
		MPU6050_Write_SCL(1);
		MPU6050_Write_SCL(0);
	}
}

/**
 * @brief MPU6050 IIC接受一字节数据
 * @retval uint8_t 接到的一字节数据
 */
uint8_t MPU6050_IIC_Receive_Byte()
{
	uint8_t i, byt = 0x00;
	MPU6050_Write_SDA(1);
	for(i = 0; i < 8; i++)
    {
		MPU6050_Write_SCL(1);
		if(MPU6050_Read_SDA() == 1) { byt |= (0x80 >> i); }
		MPU6050_Write_SCL(0);
    }
	return byt;
}

/**
 * @brief MPU6050 IIC发送应答位
 */
void MPU6050_IIC_Send_ACK(uint8_t ack)
{
	MPU6050_Write_SDA(ack);
	MPU6050_Write_SCL(1);
	MPU6050_Write_SCL(0);
}

/**
 * @brief MPU6050 IIC接受应答位
 * @retval uint8_t 接收到的应答位
 */
uint8_t MPU6050_IIC_Receive_ACK()
{
	uint8_t ack;
	MPU6050_Write_SDA(1);
	MPU6050_Write_SCL(1);
	ack = MPU6050_Read_SDA();
	MPU6050_Write_SCL(0);
	return ack;
}

void MPU6050_Write_REG(uint8_t regAddress, uint8_t data)
{
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(MPU6050_ADDR);
	MPU6050_IIC_Receive_ACK();
	MPU6050_IIC_Send_Byte(regAddress);
	MPU6050_IIC_Receive_ACK();
	MPU6050_IIC_Send_Byte(data);
	MPU6050_IIC_Receive_ACK();
	MPU6050_IIC_Stop();
}

uint8_t MPU6050_Read_REG(uint8_t regAddress)
{
	uint8_t data;
	
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(MPU6050_ADDR);
	MPU6050_IIC_Receive_ACK();
	MPU6050_IIC_Send_Byte(regAddress);
	MPU6050_IIC_Receive_ACK();
	
	MPU6050_IIC_Start();
	MPU6050_IIC_Send_Byte(MPU6050_ADDR | 0x01);
	MPU6050_IIC_Receive_ACK();
	data = MPU6050_IIC_Receive_Byte();
	MPU6050_IIC_Send_ACK(1);
	MPU6050_IIC_Stop();
	
	return data;
}

void MPU6050_Init()
{	
	MPU6050_GPIO_Init();
	MPU6050_Write_REG(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_Write_REG(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_Write_REG(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_Write_REG(MPU6050_CONFIG, 0x06);
	MPU6050_Write_REG(MPU6050_GYRO_CFG, 0x18);
	MPU6050_Write_REG(MPU6050_ACCEL_CFG, 0x00);
}

uint8_t MPU6050_Check()  
{
	uint8_t ID = MPU6050_Read_REG(MPU6050_WHO_AM_I);
	
	if(ID == 0x68)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void MPU6050_Get_Data(int16_t* AccX, int16_t* AccY, int16_t* AccZ, int16_t* GyroX, int16_t* GyroY, int16_t* GyroZ)
{
	uint8_t Data_H, Data_L;
	
	Data_H = MPU6050_Read_REG(MPU6050_ACCEL_XOUT_H);
	Data_L = MPU6050_Read_REG(MPU6050_ACCEL_XOUT_L);
	*AccX = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_Read_REG(MPU6050_ACCEL_YOUT_H);
	Data_L = MPU6050_Read_REG(MPU6050_ACCEL_YOUT_L);
	*AccY = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_Read_REG(MPU6050_ACCEL_ZOUT_H);
	Data_L = MPU6050_Read_REG(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_Read_REG(MPU6050_GYRO_XOUT_H);
	Data_L = MPU6050_Read_REG(MPU6050_GYRO_XOUT_L);
	*GyroX = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_Read_REG(MPU6050_GYRO_YOUT_H);
	Data_L = MPU6050_Read_REG(MPU6050_GYRO_YOUT_L);
	*GyroY = (Data_H << 8) | Data_L;
	
	Data_H = MPU6050_Read_REG(MPU6050_GYRO_ZOUT_H);
	Data_L = MPU6050_Read_REG(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (Data_H << 8) | Data_L;
}

void MPU6050GetStartPosture()
{
	int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
	float accPitch, accYaw, accRoll, gyroPitch, gyroYaw, gyroRoll;
	
	for(uint8_t i = 0; i < MPU6050_START_CNT; i++)
	{
		MPU6050_Get_Data(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
		
		accPitch += -gyroY / 16.4F;
		accYaw += -gyroZ / 16.4F;
		accRoll += -gyroX / 16.4F;
		gyroPitch += -atan((float)(accX) / (float)(accZ)) * 57.2974F;
		gyroYaw += -atan((float)(accX) / (float)(accY)) * 57.2974F;
		gyroRoll += -atan((float)(accY) / (float)(accZ)) * 57.2974F;
	}
		
	SetPosture(POSTURE_SPEED_X, accRoll / MPU6050_START_CNT);
	SetPosture(POSTURE_SPEED_Y, accPitch / MPU6050_START_CNT);
	SetPosture(POSTURE_SPEED_Z, accYaw / MPU6050_START_CNT);
	SetPosture(POSTURE_PITCH, gyroPitch / MPU6050_START_CNT);
	SetPosture(POSTURE_YAW, gyroYaw / MPU6050_START_CNT);
	SetPosture(POSTURE_ROLL, gyroRoll / MPU6050_START_CNT);
}
