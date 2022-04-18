#include	"stm32f4xx.h"
#include	"stm32f411xe.h"

#ifndef	EXTI_HEADER
#define	EXTI_HEADER

void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority);

#endif