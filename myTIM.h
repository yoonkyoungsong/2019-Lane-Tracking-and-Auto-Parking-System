#include	"stm32f4xx.h"
#include	"stm32f411xe.h"

#ifndef	TIM_HEADER
#define	TIM_HEADER

typedef struct {
	GPIO_TypeDef*	port;
	TIM_TypeDef*	timer;
	int	pin;
	int ch;
}	TIM_t;

void TIME_period_us(uint32_t	nTimer, float usec);
void TIME_pinmap(TIM_t* timer_pin);
void CAP_init(TIM_t* cap_pin, GPIO_TypeDef* port, int pin);
void TIM5_Init(float msec);
void TIM9_Init(float msec);

#endif