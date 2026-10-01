#include "delay.h"

void delay_ms(uint32_t ms){
	
	RCC->APB1ENR |= TIM12_EN;
	
	TIM12->PSC = PSC83;
	TIM12->ARR = ARR1000;
	TIM12->CR1 = ARPE_URS_BITS;
	TIM12->EGR = UG_BIT;
	TIM12->CR1 |= CEN_BIT;
	
	while(ms > 0){
		while((TIM12->SR & UIF_FLAG) == 0x0000);
		TIM12->SR &= ~(UIF_FLAG);
		ms--;
	}
	
	TIM12->CR1 &= ~(CEN_BIT);
	RCC->APB1ENR &= ~(TIM12_EN);
}


void delay_us(uint32_t us){
	
	RCC->APB1ENR |= TIM12_EN;
	
	TIM12->PSC = PSC0;
	TIM12->ARR = ARR84;
	TIM12->CR1 = ARPE_URS_BITS;
	TIM12->EGR = UG_BIT;
	TIM12->CR1 |= CEN_BIT;
	
	while(us > 0){
		while((TIM12->SR & UIF_FLAG) == 0x0000);
		TIM12->SR &= ~(UIF_FLAG);
		us--;
	}
	
	TIM12->CR1 &= ~(CEN_BIT);
	RCC->APB1ENR &= ~(TIM12_EN);
}


void initSYSTIMER(void){
	
	RCC->APB1ENR |= TIM2_EN;
	
	TIM2->PSC = PSC83;
	TIM2->ARR = MAX32B_VALUE;
	TIM2->CR1 = ARPE_URS_BITS;
	TIM2->EGR = UG_BIT;
	TIM2->CR1 |= CEN_BIT;
}


uint32_t getSYSTIMER(void){
	
	uint32_t time = TIM2->CNT;
	return time;
}


uint8_t chk4TimeoutSYSTIMER(uint32_t btime, uint32_t period){
	
	uint32_t time = TIM2->CNT;
	if(time >= btime){
		if(time >= (btime + period))
			return SYSTIMER_TIMEOUT;
		else
			return SYSTIMER_KEEP_ALIVE;
	}
	else{
		uint32_t utmp32 = MAX32B_VALUE - btime;
		if((time + utmp32) >= period)
			return SYSTIMER_TIMEOUT;
		else
			return SYSTIMER_KEEP_ALIVE;
	}
}
