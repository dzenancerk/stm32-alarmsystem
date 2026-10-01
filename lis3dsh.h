#ifndef __LIS3DSH_H_
#define __LIS3DSH_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "spi.h"

#define LIS3DSH_READ			0x80

#define LIS3DSH_CTRL_REG4		0x20
#define LIS3DSH_CTRL_REG1		0x21
#define LIS3DSH_CTRL_REG3		0x23
#define LIS3DSH_CTRL_REG5		0x24

#define LIS3DSH_OUT_X_L			0x28
#define LIS3DSH_OUT_X_H			0x29
#define LIS3DSH_OUT_Y_L			0x2A
#define LIS3DSH_OUT_Y_H			0x2B
#define LIS3DSH_OUT_Z_L			0x2C
#define LIS3DSH_OUT_Z_H			0x2D

#define LIS3DSH_TIM4_1			0x50
#define LIS3DSH_TIM3_1			0x51
#define LIS3DSH_TIM2_1_L		0x52
#define LIS3DSH_TIM1_1_L		0x54
#define LIS3DSH_THRS2_1			0x56
#define LIS3DSH_THRS1_1			0x57
#define LIS3DSH_MASK1_B			0x59
#define LIS3DSH_MASK1_A			0x5A
#define LIS3DSH_SETT1			0x5B
#define LIS3DSH_OUTS1			0x5F

#define LIS3DSH_ST1_1			0x40
#define LIS3DSH_ST1_2			0x41
#define LIS3DSH_ST1_3			0x42
#define LIS3DSH_ST1_4			0x43
#define LIS3DSH_ST1_5			0x44
#define LIS3DSH_ST1_6			0x45
#define LIS3DSH_ST1_7			0x46
#define LIS3DSH_ST1_8			0x47
#define LIS3DSH_ST1_9			0x48
#define LIS3DSH_ST1_10			0x49
#define LIS3DSH_ST1_11			0x4A

#define BW_FS_SPI_BITS			0x00
#define ODR_XYZ_EN_BITS			0x67
#define SM1_INT2				0x09
#define IEA_INT2_EN				0x50
#define TWENTY_MS				0x02
#define FIFTY_MS				0x05
#define SIX_HUNDRED_FORTY_MS	0x50
#define THRS_XYZ_50				0x50
#define XYZ_AXIS_EN				0xFC
#define SITR_BIT				0x01
#define ST1						0x51
#define ST2						0x51
#define ST3						0x06
#define ST4						0x38
#define ST5						0x04
#define ST6						0x91
#define ST7						0x26
#define ST8						0x38
#define ST9						0x04
#define ST10					0x91
#define ST11					0x11
#define PORT_E_EN				0x00000010
#define MODER_PE1_BITS			0x0000000C
#define PUPDR_CLEARING_PE1_BITS	0x0000000C
#define PUPDR_PE1_BITS			0x00000008
#define SYSCFG_EN				0x00004000
#define CLEARING_EXTI1_BITS		0x000000F0
#define EXTI1_BITS				0x00000040
#define PE1_IMR					0x00000002
#define PE1_EMR					0x00000002
#define PE1_RTSR				0x00000002
#define PE1_FTSR				0x00000002


void initLis3dsh(void);
void getLis3dshData(int8_t *accel_data);
void initLis3dshDoubleTapEXTI(void);
void lis3dshWriteReg(uint8_t reg, uint8_t val);
uint8_t lis3dshReadReg(uint8_t reg);

#endif
