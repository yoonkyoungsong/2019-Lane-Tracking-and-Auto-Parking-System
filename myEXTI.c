#include	"myInclude.h"

void EXTI_init(GPIO_TypeDef *port, int pin, int trig_type, int priority)	{
	
	IRQn_Type extiport;
	uint32_t value;
	
	//classification through pin number
	if((pin >= 5) && (pin <= 9))           { extiport = EXTI9_5_IRQn;}   // pin 5~9 ? ?? ????    
  else if ((pin >= 10) && (pin <= 15))   { extiport = EXTI15_10_IRQn; }// pin 10~15 ? ?? ????
	else if(pin == 0)                      { extiport = EXTI0_IRQn; }
	else if(pin == 1)                      { extiport = EXTI1_IRQn;}
	else if(pin == 2)                      { extiport = EXTI2_IRQn;}
	else if(pin == 3)                      { extiport = EXTI3_IRQn;}
	else if(pin == 4)                      { extiport = EXTI4_IRQn;}
	
	//give priority to extiport 
	NVIC_SetPriority(extiport, priority); // priority setting
	
	//open the clock for config
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //  same as == 1<<14; 
	
	//classification through porttype
	if      (port == GPIOA) { value = 0UL;} 
	else if (port == GPIOB) { value = 1UL;} 
	else if (port == GPIOC) { value = 2UL;} 
	else if (port == GPIOD) { value = 3UL;} 
	else if (port == GPIOD) { value = 4UL;}
	
	//pin is classified as four EXTICR 
	//0~3 is 00XX, 4~7 01XX, 8~11 10XX, 9~12, 13~16 11XX
	//so shift right 2 can make 0,1,2,3,4 which EXTICR's pin classification 

	SYSCFG->EXTICR[pin>>2] &= ~(0xF << ((pin % 4)*4));  // clear four bit
	
	SYSCFG->EXTICR[pin>>2] |= (value<< ((pin % 4)*4));  // setting port C
	
	//edge external interrupt 10.3 /10.3.4 falling triger    ///again
	
	//EXTI->FTSR &= ~(1UL << pin);      //?? ??? ??????
	if(trig_type == FALL) EXTI->FTSR |= (1 << pin);     // FALLedge trigger
	if(trig_type == RISE) EXTI->RTSR |= (1 << pin);     // RISEedge trigger
	
	//EXTI->IMR &= ~(1UL << pin);
	EXTI->IMR |= (1UL << pin);          //enable the pin not mask = enable
	
	NVIC_EnableIRQ(extiport); 
}

