#include	"myInclude.h"

uint32_t msTicks = 0; 

void SysTick_init(void){	                         //systick -> system intenal timer
	//  SysTick Control and Status Register
	SysTick->CTRL = 0;										// Disable SysTick IRQ and SysTick Counter
	
	// SysTick Reload Value Register
	SysTick->LOAD = 84000000/1000 - 1;    // 1ms, Default clock
	
	// SysTick Current Value Register
	SysTick->VAL = 0;

	NVIC_SetPriority(SysTick_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);					// Enable interrupt in NVIC

	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // when load  to '0' tickinterrupt load
	
	// Select processor clock
	// 1 = processor clock;  0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
}

void SysTick_Handler(void){
	SysTick_counter();	
}

void SysTick_counter(){
	msTicks++;
}	

void delay (uint32_t T){
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < T);
	
	msTicks = 0;
}
