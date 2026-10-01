#include "stm32f4xx.h"
#include "delay.h"
#include "lis3dsh.h"
#include "cs43l22.h"
#include <math.h>

#define BTN_IDLE						0
#define BTN_DETECTED					1
#define BTN_WAIT4LOW					2
#define BTN_DEBOUNCE					3
#define ALARM_OFF						0
#define ALARM_ON						1
#define ALARM_TRIGGERED					2
#define THRESHOLD						42
#define AUDIO_BUFFER_SIZE				1000
#define PI								3.14159
#define SAMPLE_RATE						48000
#define EXTI0							0x00000001
#define EXTI1							0x00000002
#define PAO_HIGH						0x0001
#define PAO_LOW							0x0000
#define DEBOUNCE_PERIOD					50000
#define ALL_LEDS						0xF000
#define Z_OFFSET						64
#define BLUE_LED						0x8000
#define SLOW_BLINKS						500000
#define FAST_BLINKS						100000
#define ACCEL_PERIOD					100000
#define PORT_A							0x00000001
#define SYSCFG_EN						0x00004000
#define MODER_PA0						0x00000003
#define EXTI0_LINE						0x0000000F
#define EXTI0_IMR						0x00000001
#define EXTI0_EMR						0x00000001
#define EXTI0_RTSR						0x00000001
#define EXTI0_FTSR						0x00000001
#define MODER_CLEARING_LEDS_BITS		0xFF000000
#define MODER_LED_BITS					0x55000000
#define PP_PINS							0x0000F000

volatile uint8_t g_button_state = BTN_IDLE;
volatile uint8_t g_alarm_state = ALARM_OFF;
volatile uint32_t g_debounce_timer = 0;
volatile uint32_t g_led_timer = 0;
volatile uint8_t g_audio_muted = 1;
volatile uint16_t g_alarm_tone[AUDIO_BUFFER_SIZE];


void generateAlarmTone(void){
	
	uint16_t n;
	float y, t = 0;
	float freq = 3200.0;
	
	for(n = 0; n < AUDIO_BUFFER_SIZE; n++){
		y = 0.4 * sinf(2 * PI * freq * t) + 0.5;
		g_alarm_tone[n] = (uint16_t)(y * 32767);
		t = t + 0.1e-4;
	}
}


void stopAudio(void){
	
	writeI2C(CS43L22_REG_PLAYBACK_CONTROL2, ALL_OFF);
	writeI2C(CS43L22_REG_POWER_CONTROL2, ALL_OFF);
	DMA1_Stream5->CR &= ~(EN_BIT);
	while((DMA1_Stream5->CR & EN_BIT) == EN_BIT);
	SPI3->I2SCFGR &= ~(I2S_EN);
	RCC->APB1ENR &= ~(SPI3_PERIPHERAL);
}


void startAudio(void){
	
	RCC->APB1ENR |= SPI3_PERIPHERAL;
	writeI2C(CS43L22_REG_POWER_CONTROL1, POWER_UP);
	writeI2C(CS43L22_REG_POWER_CONTROL2, HEADPHONES_ON_SPEAKERS_OFF);
	SPI3->I2SCFGR |= I2S_EN;
	DMA1_Stream5->CR |= EN_BIT;
	writeI2C(CS43L22_REG_PLAYBACK_CONTROL2, HEADPHONES_UNMUTE);
}


int abs(int x){
	
	if(x < 0)
		return -x;
	else
		return x;
}


void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void serviceButton(void);
void checkMotion(int8_t *accel_data);
void updateLEDs(void);
void initEXTI0_UserButton(void);
void initLEDs(void);


int main(void){
	
	uint32_t accel_timer;
	int8_t accel_data[3];
	
	initLEDs();
	initSYSTIMER();
	initEXTI0_UserButton();
	initLis3dsh();
	initLis3dshDoubleTapEXTI();
	generateAlarmTone();
	initCs43l22(SAMPLE_RATE, (uint16_t *)g_alarm_tone, AUDIO_BUFFER_SIZE);
	delay_ms(100);
	stopAudio();
	
	accel_timer = getSYSTIMER();
	
	while(1){
		serviceButton();
		
		if(chk4TimeoutSYSTIMER(accel_timer, ACCEL_PERIOD) == SYSTIMER_TIMEOUT){
			getLis3dshData(accel_data);
			checkMotion(accel_data);
			accel_timer = getSYSTIMER();
		}
		updateLEDs();
	}
}


void EXTI0_IRQHandler(void){
	
	if((EXTI->PR & EXTI0) == EXTI0){
		if(g_button_state == BTN_IDLE){
			g_button_state = BTN_DETECTED;
		}
		EXTI->PR = EXTI0;
	}
}


void EXTI1_IRQHandler(void){
	
	if((EXTI->PR & EXTI1) == EXTI1){
		EXTI->PR = EXTI1;
		lis3dshReadReg(LIS3DSH_OUTS1);
		
		if(g_alarm_state == ALARM_TRIGGERED)
			g_alarm_state = ALARM_OFF;
		else if(g_alarm_state == ALARM_OFF)
			g_alarm_state = ALARM_ON;
		else
			g_alarm_state = ALARM_OFF;
	}
}


void serviceButton(void){
	
	switch(g_button_state){
		case BTN_IDLE:
			break;
		case BTN_DETECTED:
			if(g_alarm_state == ALARM_TRIGGERED)
				g_alarm_state = ALARM_OFF;
			else if(g_alarm_state == ALARM_OFF)
				g_alarm_state = ALARM_ON;
			else
				g_alarm_state = ALARM_OFF;
			g_button_state = BTN_WAIT4LOW;
			break;
		case BTN_WAIT4LOW:
			if((GPIOA->IDR & PAO_HIGH) == PAO_LOW){
				g_button_state = BTN_DEBOUNCE;
				g_debounce_timer = getSYSTIMER();
			}
			break;
		case BTN_DEBOUNCE:
			if(chk4TimeoutSYSTIMER(g_debounce_timer, DEBOUNCE_PERIOD) == SYSTIMER_TIMEOUT)
				g_button_state = BTN_IDLE;
			break;
	}
}


void checkMotion(int8_t *accel_data){
	
	if(g_alarm_state == ALARM_ON){
		if(abs(accel_data[0]) > THRESHOLD ||
			abs(accel_data[1]) > THRESHOLD ||
			abs(accel_data[2] - Z_OFFSET) > THRESHOLD){
				g_alarm_state = ALARM_TRIGGERED;
				GPIOD->ODR |= ALL_LEDS;
				g_led_timer = getSYSTIMER();
		}
	}
}


void updateLEDs(void){
	
	if(g_led_timer == 0)
		g_led_timer = getSYSTIMER();
	
	switch(g_alarm_state){
		case ALARM_OFF:
			GPIOD->ODR &= ~(ALL_LEDS);
			if(g_audio_muted == 0){
				stopAudio();
				g_audio_muted = 1;
			}
			break;
		case ALARM_ON:
			if(chk4TimeoutSYSTIMER(g_led_timer, SLOW_BLINKS) == SYSTIMER_TIMEOUT){
				GPIOD->ODR ^= BLUE_LED;
				g_led_timer = getSYSTIMER();
			}
			if(g_audio_muted == 0){
				stopAudio();
				g_audio_muted = 1;
			}
			break;
		case ALARM_TRIGGERED:
			if(chk4TimeoutSYSTIMER(g_led_timer, FAST_BLINKS) == SYSTIMER_TIMEOUT){
				GPIOD->ODR ^= ALL_LEDS;
				g_led_timer = getSYSTIMER();
			}
			if(g_audio_muted == 1){
				startAudio();
				g_audio_muted = 0;
			}
			break;
	}
}


void initEXTI0_UserButton(void){
	
	RCC->AHB1ENR |= PORT_A;
	RCC->APB2ENR |= SYSCFG_EN;
	
	GPIOA->MODER &= ~(MODER_PA0);
	SYSCFG->EXTICR[0] &= ~(EXTI0_LINE);
	EXTI->IMR |= EXTI0_IMR;
	EXTI->EMR &= ~(EXTI0_EMR);
	EXTI->RTSR |= EXTI0_RTSR;
	EXTI->FTSR &= ~(EXTI0_FTSR);
	NVIC_EnableIRQ(EXTI0_IRQn);
}


void initLEDs(void){
	
	RCC->AHB1ENR |= PORT_D_EN;
	GPIOD->MODER &= ~(MODER_CLEARING_LEDS_BITS);
	GPIOD->MODER |= MODER_LED_BITS;
	GPIOD->OTYPER &= ~(PP_PINS);
}
