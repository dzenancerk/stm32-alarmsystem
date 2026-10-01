#ifndef __CS43L22_H_
#define __CS43L22_H_

#include "stm32f4xx.h"
#include "i2c.h"

#define CS43L22_I2C_ADDR						0x94
#define CS43L22_REG_POWER_CONTROL1				0x02
#define CS43L22_REG_POWER_CONTROL2				0x04
#define CS43L22_REG_CLOCK_CONTROL				0x05
#define CS43L22_REG_INTERFACE_CONTROL			0x06
#define CS43L22_REG_MISC_CONTROL				0x0E
#define CS43L22_REG_PLAYBACK_CONTROL2			0x0F
#define CS43L22_REG_PCMA_VOLUME					0x1A
#define CS43L22_REG_PCMB_VOLUME					0x1B
#define PORT_D_EN								0x00000008
#define MODER_CLEARING_PD4_BITS					0x00000300
#define MODER_PD4_BITS							0x00000100
#define OSPEEDR_CLEARING_PD4_BITS				0x00000300
#define OSPEEDR_PD4_BITS						0x00000200
#define PORT_C_EN								0x00000004
#define MODER_CLEARING_PC7_PC10_PC12_BITS		0x0330C000
#define MODER_PC7_PC10_PC12_BITS				0x02208000
#define AFR_CLEARING_PC7_BITS					0xF0000000
#define AFR_PC7_BITS							0x60000000
#define AFR_CLEARING_PC10_PC_12_BITS			0x000F0F00
#define AFR_PC10_PC12_BITS						0x00060600
#define PORT_A_EN								0x00000001
#define MODER_CLEARING_PA4_BITS					0x00000300
#define MODER_PA4_BITS							0x00000200
#define AFR_CLEARING_PA4_BITS					0x000F0000
#define AFR_PA4_BITS							0x00060000
#define POWER_DOWN								0x01
#define HEADPHONES_ON_SPEAKERS_OFF				0xAF
#define AUTO									0xA0
#define SLAVE_LEFT_16B							0x03
#define PASSTHROUGH								0x30
#define HEADPHONES_UNMUTE						0x00
#define A_B_UNMUTE								0x00
#define INIT_STEP1_REG							0x00
#define INIT_STEP2_REG							0x47
#define INIT_STEP3_REG							0x32
#define INIT_STEP4_REG							0x32
#define INIT_STEP5_REG							0x00
#define INIT_STEP1_VAL							0x99
#define INIT_STEP2_VAL							0x80
#define INIT_STEP3_VAL							0x80
#define INIT_STEP4_VAL							0x00
#define INIT_STEP5_VAL							0x00
#define POWER_UP								0x9E
#define I2S_CLOCK_FREQ							86000000
#define ODDS									0x00000001
#define FACTOR									256
#define ODD_BIT									0x0100
#define SPI3_PERIPHERAL							0x00008000
#define MCKOE_BIT								0x0200
#define TXDMAEN_BIT								0x0002
#define I2S_MODE								0x0800
#define CLEARING_I2SCFG_BITS					0x0300
#define I2SCFG_BITS								0x0200
#define CLEARING_I2SSTD_BITS					0x0030
#define I2SSTD_BITS								0x0010
#define CKPOL_BIT								0x0008
#define I2S_EN									0x0400
#define DMA1_PERIPHERAL							0x00200000
#define EN_BIT									0x00000001
#define CLEARING_INTERRUPT_FLAGS				0xFFFFFFFF
#define CLEAR_STATES							0x00000000
#define PINC_BIT								0x00000400
#define CIRC_BIT								0x00000100
#define MINC_BIT								0x00000800
#define MSIZE_BITS								0x00002000
#define DIR_BITS								0x00000040
#define ALL_OFF									0xFF

void initCs43l22(uint32_t sample_rate, uint16_t *ptr, uint16_t size);

#endif
