#ifndef __SPI1_H_
#define __SPI1_H_

#include "stm32f4xx.h"

#define SPI_BaudRatePrescaler_32				0x0020
#define SPI_I2S_FLAG_RXNE						0x0001
#define SPI_I2S_FLAG_TXE						0x0002
#define SPI_I2S_FLAG_BSY						0x0080
#define PORT_A_EN								0x00000001
#define MODER_CLEARING_PA5_PA6_PA7_BITS			0x0000FC00
#define MODER_PA5_PA6_PA7_BITS					0x0000A800
#define AFR_CLEARING_PA5_PA6_PA7_BITS			0xFFF00000
#define AFR_PA5_PA6_PA7_BITS					0x55500000
#define OSPEEDR_CLEARING_PA5_PA6_PA7_BITS		0x0000FC00
#define OSPEEDR_PA5_PA6_PA7_BITS				0x0000A800
#define PORT_E_EN								0x00000010
#define MODER_CLEARING_PE3_BITS					0x000000C0
#define MODER_PE3_BITS							0x00000040
#define OTYPER_PE3_BIT							0x00000008
#define OSPEEDR_CLEARING_PE3_BITS				0x000000C0
#define OSPEEDR_PE3_BITS						0x00000080
#define SPI1_PERIPHERAL_EN						0x00001000
#define MSTR_BIT								0x0004
#define SSM_SSI_BITS							0x0300
#define SPI1_EN									0x0040
#define DUMMY_BYTE								0x0000

#define SPI1_CS_HIGH							GPIOE->ODR |= 0x0008;
#define SPI1_CS_LOW								GPIOE->ODR &= ~(0x0008);

void initSPI1(uint16_t prescaler);
uint8_t rxByteSPI1(void);
uint8_t txByteSPI1(uint8_t data);

#endif
