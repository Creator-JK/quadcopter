#include "dri_nrf24l01.h"
#include "stm32f10x.h"
#include "dri_serial.h"
#include "sys.h"

#define nRF_Write_CSN(x)	do{ GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)(x)); }while(0)
#define nRF_Write_CE(x)		do{ GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)(x)); }while(0)
#define nRF_Write_MOSI(x)	do{ GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(x)); }while(0)
#define nRF_Write_SCK(x)	do{ GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(x)); }while(0)

uint8_t nRF_Read_MISO()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
}

uint8_t nRF_Read_IRQ()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
}

const uint8_t ADDR[5] = { 0x4C, 0x45, 0x44, 0x52, 0x01 };
uint8_t nRF_SwapByt(uint8_t byt)
{
	uint8_t swapByt;
		
	for(uint8_t i = 0; i < 8; i++)
	{
		nRF_Write_MOSI(byt & (0x80 >> i));
		nRF_Write_SCK(1);
		if(nRF_Read_MISO() == 1) { swapByt |= (0x80 >> i);}
		nRF_Write_SCK(0);
	}
	
	return swapByt;
}

uint8_t nRF_ReadReg(uint8_t addr)
{
	uint8_t val;
	
	nRF_Write_CSN(0);
		
	nRF_SwapByt(addr);
	val = nRF_SwapByt(0xFF);
	
	nRF_Write_CSN(1);
	
	return val;
}

uint8_t nRF_ReadBuf(uint8_t addr, uint8_t *pBuf, uint8_t len)
{
	uint8_t  s;
	
	nRF_Write_CSN(0);
	
	s = nRF_SwapByt(addr);
	for(uint8_t i = 0; i < len; i++)
		pBuf[i] = nRF_SwapByt(0x00);
	
	nRF_Write_CSN(1);
	
	return s;
}

uint8_t nRF_WriteReg(uint8_t addr, uint8_t val)
{
	uint8_t s;
	
	nRF_Write_CSN(0);
	
	s = nRF_SwapByt(addr);
	nRF_SwapByt(val);
	
	nRF_Write_CSN(1);
	
	return s;
}

uint8_t nRF_WriteBuf(uint8_t addr, uint8_t *pBuf, uint8_t len)
{
	uint8_t s;
	
	nRF_Write_CSN(0);
	
	s = nRF_SwapByt(addr);
	SYS_Delay_us(10);
	for(uint8_t i = 0; i < len; i++)
		nRF_SwapByt(pBuf[i]);
	
	nRF_Write_CSN(1);
	
	return s;
}

uint8_t nRF_Check()
{
	uint8_t cBuf[5] = { 0xA5, 0xBD, 0xCD, 0xBF, 0x45 };
	uint8_t rBuf[5];

	nRF_WriteBuf(nRF_WRITE_REG + nRF_RX_ADDR_P1, cBuf, 5);
	nRF_ReadBuf(nRF_RX_ADDR_P1, rBuf, 5);
		
	for(uint8_t i = 0; i < 5; i++)
	{
		if(cBuf[i] != rBuf[i])
		{
			return 0;
		}
	}
	return 1;
}

void nRF_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_initTypeDef;
	GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4;
	GPIO_initTypeDef.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_initTypeDef);
	
	GPIO_initTypeDef.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_initTypeDef.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_initTypeDef);
	
	nRF_Write_CSN(1);
	nRF_Write_CE(0);
}


void nRF_RxMode()
{
	nRF_Write_CE(0);

	nRF_WriteReg(nRF_WRITE_REG + nRF_EN_RX_ADDR, 0x01);
	nRF_WriteReg(nRF_WRITE_REG + nRF_RX_PW_P0, 5);
	nRF_WriteReg(nRF_WRITE_REG + nRF_CFG, 0x0F);
	nRF_WriteReg(nRF_FLUSH_RX, 0xFF);
	nRF_WriteReg(nRF_FLUSH_TX, 0xFF);
	nRF_WriteReg(nRF_WRITE_REG + 0x1C, 0x01);
	nRF_WriteReg(nRF_WRITE_REG + 0x1D, 0x06);
	
	nRF_Write_CE(1);
}

void nRF_TxMode()
{
	nRF_Write_CE(0);

	nRF_WriteReg(nRF_WRITE_REG + nRF_CFG, 0x0E);
	nRF_WriteReg(nRF_FLUSH_RX, 0xFF);
	nRF_WriteReg(nRF_FLUSH_TX, 0xFF);
	nRF_WriteReg(nRF_WRITE_REG + 0x1C, 0x01);
	nRF_WriteReg(nRF_WRITE_REG + 0x1D, 0x06);

	nRF_Write_CE(1);
}

uint8_t i;
uint8_t nRF_TxPacket(uint8_t *pBuf)
{
	nRF_Write_CE(0);
	nRF_WriteBuf(nRF_TX_PLOAD, pBuf, nRF_PLOAD_WIDTH);
	nRF_Write_CE(1);
	
	i = nRF_Read_IRQ();
	
	uint8_t s = nRF_ReadReg(nRF_STATUS);
	nRF_WriteReg(nRF_WRITE_REG + nRF_STATUS, s);
	if(s & nRF_MAX_TX)
	{
		nRF_WriteReg(nRF_FLUSH_TX, 0xFF);
		return nRF_MAX_TX;
	}
	if(s & nRF_TX_OK)
	{
		return nRF_TX_OK;
	}
	return 0xFF; //发送数据失败
}

uint8_t nRF_RxPacket(uint8_t *pBuf)
{
	uint8_t s = nRF_ReadReg(nRF_STATUS);
	nRF_WriteReg(nRF_WRITE_REG + nRF_STATUS, s);
	
	if(s & nRF_RX_OK)
	{
		nRF_ReadBuf(nRF_RX_PLOAD, pBuf, nRF_PLOAD_WIDTH);
		nRF_WriteReg(nRF_FLUSH_RX, 0xFF);
		return 0;
	}
	return 1; //未接收到任何数据
}
