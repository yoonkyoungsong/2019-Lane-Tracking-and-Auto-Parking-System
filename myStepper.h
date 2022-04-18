#ifndef __MY_STEPPER_H
#define __MY_STEPPER_H

#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include <stdint.h>

extern uint32_t direction; 
extern uint32_t step_delay; 
extern uint32_t step_per_rev;
extern uint32_t motor_type;

extern int step_number;

extern int pin1;
extern int pin2;
extern int pin3;
extern int pin4;


void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4);
void Stepper_setSpeed(uint32_t speed);
void Stepper_step (uint32_t step);
void Stepper_stepMotor (uint32_t sequence);


#endif