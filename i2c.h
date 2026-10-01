#ifndef _I2CS_H_
#define _I2CS_H_

#include "stm32f4xx.h"
#include "delay.h"

#define PORT_B_EN						0x00000002
#define I2C1_PERIPHERAL					0x00200000
#define MODER_CLEARING_PB6_PB9_BITS		0x000C3000
#define MODER_PB6_PB9_BITS				0x00082000
#define OSPEEDR_CLEARING_PB6_PB9_BITS	0x000C3000
#define OSPEEDR_PB6_PB9_BITS			0x00082000
#define PUPDR_CLEARING_PB6_PB9_BITS		0x000C3000
#define PUPDR_PB6_PB9_BITS				0x00041000
#define OTYPER_PB6_PB9_BITS				0x00000240
#define AFR_CLEARING_PB6_BITS			0x0F000000
#define AFR_PB6_BITS					0x04000000
#define AFR_CLEARING_PB9_BITS			0x000000F0
#define AFR_PB9_BITS					0x00000040
#define SWRTS_BIT						0x8000
#define I2C1_EN							0x0001
#define I2C1_CR2_FREQ					0x00000008
#define I2C1_CCR_FORTY					0x00000028
#define I2C1_TRISE_NINE					0x00000009
#define I2C1_7B_DC						0x4000
#define ACK_BIT							0x0400
#define TRA_BUSY_MSL_TXE_FLAGS			0x00070080
#define BTF_FLAG						0x0004
#define STOP_BIT						0x0200
#define BUSY_FLAG						0x0002
#define START_BIT						0x0100
#define TRA_BUSY_MSL_SB_FLAGS			0x00030001
#define TRA_BUSY_MSL_TXE_ADDR_FLAGS		0x00070082

void initI2C(uint8_t addr);
void txByteI2C(uint8_t data);
void writeI2C(uint8_t reg, uint8_t data);

#endif
