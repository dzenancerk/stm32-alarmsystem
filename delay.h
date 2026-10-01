#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

#define SYSTIMER_TIMEOUT			0
#define SYSTIMER_KEEP_ALIVE			1
#define PSC83						(0x0054 - 0x0001)
#define ARR1000						0x03E8
#define ARPE_URS_BITS				0x0084
#define UG_BIT						0x0001
#define CEN_BIT						0x0001
#define TIM12_EN					0x00000040
#define UIF_FLAG					0x0001
#define PSC0						(0x0001 - 0x0001)
#define ARR84						0x0054
#define TIM2_EN						0x0001
#define MAX32B_VALUE				0xFFFFFFFF

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

void initSYSTIMER(void);
uint32_t getSYSTIMER(void);
uint8_t chk4TimeoutSYSTIMER(uint32_t btime, uint32_t period);

#endif
