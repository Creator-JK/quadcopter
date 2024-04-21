#ifndef _NRF24L01_H_
#define _NRF24L01_H_

#include "stm32f10x.h"

// SPI(nRF) commands
#define nRF_READ_REG    0x00  // Define read command to register
#define nRF_WRITE_REG   0x20  // Define write command to register
#define nRF_RX_PLOAD 	0x61  // Define RX payload register address
#define nRF_TX_PLOAD 	0xA0  // Define TX payload register address
#define nRF_FLUSH_TX    0xE1  // Define flush TX register command
#define nRF_FLUSH_RX    0xE2  // Define flush RX register command
#define nRF_REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define nRF_NOP         0xFF  // Define No Operation, might be used to read status register

// SPI(nRF) registers(addresses)
#define nRF_CFG      0x00  // 'Config' register address
#define nRF_EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define nRF_EN_RX_ADDR  0x02  // 'Enabled RX addresses' register address
#define nRF_SETUP_AW    0x03  // 'Setup address width' register address
#define nRF_SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define nRF_RF_CH       0x05  // 'RF channel' register address
#define nRF_RF_SETUP    0x06  // 'RF setup' register address
#define nRF_STATUS      0x07  // 'Status' register address
#define nRF_OBSERVE_TX  0x08  // 'Observe TX' register address
#define nRF_CD          0x09  // 'Carrier Detect' register address
#define nRF_RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define nRF_RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define nRF_RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define nRF_RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define nRF_RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define nRF_RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define nRF_TX_ADDR     0x10  // 'TX address' register address
#define nRF_RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define nRF_RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define nRF_RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define nRF_RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define nRF_RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define nRF_RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define nRF_FIFO_STATUS 0x17  // 'FIFO Status Register' register address

#define nRF_ADDR_WIDTH	5  // 5字节宽度的发送/接收地址
#define nRF_PLOAD_WIDTH 32  // 数据通道有效数据宽度

#define nRF_MAX_TX	0x10
#define nRF_TX_OK	0x20
#define nRF_RX_OK	0x40

uint8_t nRF_Check(void);
void nRF_Init(void);
void nRF_SendData(uint8_t *txBuf);
void nRF_TxMode(void);
void nRF_RxMode(void);
uint8_t nRF_RxPacket(uint8_t *rxBuf);
uint8_t nRF_TxPacket(uint8_t *txBuf);

#endif //_NRF24L01_H_
