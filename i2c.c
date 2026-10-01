#include "i2c.h"

volatile uint8_t i2c_addr;
void startI2C(void);
void stopI2C(void);

void initI2C(uint8_t addr){
	
	RCC->AHB1ENR |= PORT_B_EN;
	RCC->APB1ENR |= I2C1_PERIPHERAL;
	
	GPIOB->MODER &= ~(MODER_CLEARING_PB6_PB9_BITS);
	GPIOB->MODER |= MODER_PB6_PB9_BITS;
	GPIOB->OSPEEDR &= ~(OSPEEDR_CLEARING_PB6_PB9_BITS);
	GPIOB->OSPEEDR |= OSPEEDR_PB6_PB9_BITS;
	GPIOB->PUPDR &= ~(PUPDR_CLEARING_PB6_PB9_BITS);
	GPIOB->PUPDR |= PUPDR_PB6_PB9_BITS;
	GPIOB->OTYPER |= OTYPER_PB6_PB9_BITS;
	GPIOB->AFR[0] &= ~(AFR_CLEARING_PB6_BITS);
	GPIOB->AFR[0] |= AFR_PB6_BITS;
	GPIOB->AFR[1] &= ~(AFR_CLEARING_PB9_BITS);
	GPIOB->AFR[1] |= AFR_PB9_BITS;
	
	i2c_addr = addr;
	
	I2C1->CR1 |= SWRTS_BIT;
	I2C1->CR1 &= ~(SWRTS_BIT);
	I2C1->CR1 &= ~(I2C1_EN);
	
	I2C1->CR2 = I2C1_CR2_FREQ;
	I2C1->CCR = I2C1_CCR_FORTY;
	I2C1->TRISE = I2C1_TRISE_NINE;
	I2C1->OAR1 = I2C1_7B_DC;
	
	I2C1->CR1 |= ACK_BIT;
	I2C1->CR1 |= I2C1_EN;
}


void writeI2C(uint8_t reg, uint8_t data){
	
	startI2C();
	txByteI2C(reg);
	txByteI2C(data);
	stopI2C();
	delay_ms(5);
}


void txByteI2C(uint8_t data){
	
	uint32_t sreg;
	
	while(1){
		sreg = (I2C1->SR2) << 16;
		sreg |= I2C1->SR1;
		
		if((sreg & TRA_BUSY_MSL_TXE_FLAGS) == TRA_BUSY_MSL_TXE_FLAGS)
			break;
	}
	I2C1->DR = data;
}


void stopI2C(void){
	
	while(!(I2C1->SR1 & BTF_FLAG));
	I2C1->CR1 |= STOP_BIT;
	while(I2C1->SR2 & BUSY_FLAG);
}


void startI2C(void){
	
	uint32_t sreg;
	
	while(I2C1->SR2 & BUSY_FLAG);
	
	I2C1->CR1 |= START_BIT;
	
	while(1){
		sreg = (I2C1->SR2) << 16;
		sreg |= I2C1->SR1;
		
		if((sreg & TRA_BUSY_MSL_SB_FLAGS) == TRA_BUSY_MSL_SB_FLAGS)
			break;
	}
	
	I2C1->DR = i2c_addr;
	delay_us(50);
	while(1){
		sreg = (I2C1->SR2) << 16;
		sreg |= I2C1->SR1;
			
		if((sreg & TRA_BUSY_MSL_TXE_ADDR_FLAGS) == TRA_BUSY_MSL_TXE_ADDR_FLAGS)
			break;
	}
}
