#include "lis3dsh.h"

void lis3dshWriteReg(uint8_t reg, uint8_t val){
	
	SPI1_CS_LOW;
	txByteSPI1(reg);
	txByteSPI1(val);
	SPI1_CS_HIGH;
}


uint8_t lis3dshReadReg(uint8_t reg){
	
	uint8_t val;
	SPI1_CS_LOW;
	txByteSPI1(reg | LIS3DSH_READ);
	val = rxByteSPI1();
	SPI1_CS_HIGH;
	return val;
}


void initLis3dsh(void){
	
	initSPI1(SPI_BaudRatePrescaler_32);
	
	lis3dshWriteReg(LIS3DSH_CTRL_REG4, ODR_XYZ_EN_BITS);
	lis3dshWriteReg(LIS3DSH_CTRL_REG5, BW_FS_SPI_BITS);
	delay_ms(10);
	
	lis3dshWriteReg(LIS3DSH_TIM4_1, TWENTY_MS);
	lis3dshWriteReg(LIS3DSH_TIM3_1, FIFTY_MS);
	lis3dshWriteReg(LIS3DSH_TIM1_1_L, FIFTY_MS);
	lis3dshWriteReg(LIS3DSH_TIM2_1_L, SIX_HUNDRED_FORTY_MS);
	lis3dshWriteReg(LIS3DSH_THRS1_1, THRS_XYZ_50);
	lis3dshWriteReg(LIS3DSH_THRS2_1, THRS_XYZ_50);
	lis3dshWriteReg(LIS3DSH_MASK1_B, XYZ_AXIS_EN);
	lis3dshWriteReg(LIS3DSH_MASK1_A, XYZ_AXIS_EN);
	lis3dshWriteReg(LIS3DSH_SETT1, SITR_BIT);
	
	lis3dshWriteReg(LIS3DSH_ST1_1, ST1);
	lis3dshWriteReg(LIS3DSH_ST1_2, ST2);
	lis3dshWriteReg(LIS3DSH_ST1_3, ST3);
	lis3dshWriteReg(LIS3DSH_ST1_4, ST4);
	lis3dshWriteReg(LIS3DSH_ST1_5, ST5);
	lis3dshWriteReg(LIS3DSH_ST1_6, ST6);
	lis3dshWriteReg(LIS3DSH_ST1_7, ST7);
	lis3dshWriteReg(LIS3DSH_ST1_8, ST8);
	lis3dshWriteReg(LIS3DSH_ST1_9, ST9);
	lis3dshWriteReg(LIS3DSH_ST1_10, ST10);
	lis3dshWriteReg(LIS3DSH_ST1_11, ST11);
	
	lis3dshWriteReg(LIS3DSH_CTRL_REG1, SM1_INT2);
	lis3dshWriteReg(LIS3DSH_CTRL_REG3, IEA_INT2_EN);
	delay_ms(100);
}


void getLis3dshData(int8_t *accel_data){
	
	uint8_t data[6];
	
	SPI1_CS_LOW;
	
	txByteSPI1(LIS3DSH_OUT_X_L | LIS3DSH_READ);
	
	data[0] = rxByteSPI1();
	data[1] = rxByteSPI1();
	data[2] = rxByteSPI1();
	data[3] = rxByteSPI1();
	data[4] = rxByteSPI1();
	data[5] = rxByteSPI1();
	
	SPI1_CS_HIGH;
	
	accel_data[0] = data[1];
	accel_data[1] = data[3];
	accel_data[2] = data[5];
}


void initLis3dshDoubleTapEXTI(void){
	
	RCC->AHB1ENR |= PORT_E_EN;
	
	GPIOE->MODER &= ~(MODER_PE1_BITS);
	GPIOE->PUPDR &= ~(PUPDR_CLEARING_PE1_BITS);
	GPIOE->PUPDR |= PUPDR_PE1_BITS;
	RCC->APB2ENR |= SYSCFG_EN;
	
	SYSCFG->EXTICR[0] &= ~(CLEARING_EXTI1_BITS);
	SYSCFG->EXTICR[0] |= EXTI1_BITS;
	
	EXTI->IMR |= PE1_IMR;
	EXTI->EMR &= ~(PE1_EMR);
	EXTI->RTSR |= PE1_RTSR;
	EXTI->FTSR &= ~(PE1_FTSR);
	
	NVIC_EnableIRQ(EXTI1_IRQn);
}
