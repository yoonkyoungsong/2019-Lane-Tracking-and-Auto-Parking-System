#ifndef __MY_SYSTICK_H
#define __MY_SYSTICK_H

#include "stm32f4xx.h"
#include <stdint.h>

void SysTick_init(void);
void SysTick_Handler(void);
void SysTick_counter();
void delay (uint32_t T);

#endif