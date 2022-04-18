#include	"stm32f4xx.h"
#include	"stm32f411xe.h"

#ifndef	PWM_HEADER
#define	PWM_HEADER

typedef struct {
   GPIO_TypeDef   *port;
   TIM_TypeDef    *timer;
   int   pin;
   int   ch;
}   PWM_t;

void   PWM_pinmap(PWM_t *pwm);
void   PWM_init(PWM_t *pwm, GPIO_TypeDef *Port, int pin);
void   PWM_period_ms(PWM_t *pwm, float period_ms);
void   PWM_pulsewidth_ms(PWM_t *pwm, float pulsewidth_ms);
void   PWM_duty(PWM_t *pwm, float duty);
void   Timer_interupt_init(PWM_t *pwm, GPIO_TypeDef *Port, int pin);
#endif