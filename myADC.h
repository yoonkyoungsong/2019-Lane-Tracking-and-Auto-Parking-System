#include	"stm32f4xx.h"
#include	"stm32f411xe.h"

#ifndef	ADC_HEADER
#define	ADC_HEADER


uint32_t ADC_pinmap(GPIO_TypeDef *port, uint32_t pin) ;
void ADC_init(GPIO_TypeDef *port,  int pin, int mode); 
uint32_t ADC_read();
void ADC_init_TRGO(GPIO_TypeDef *port, int pin);
void TIM3_Init(float msec);
void TIM_Init(float msec,TIM_TypeDef *TIMx);
void Timer_Init(float msec,TIM_TypeDef *TIMx);
void JADC_init(GPIO_TypeDef *port,  int pin);

#endif

