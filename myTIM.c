
#include   "myInclude.h"

void TIME_period_us(uint32_t   nTimer, float usec) {
   uint32_t timerbase, apbbase;
   
   int pose;
   if((nTimer == 1)||(nTimer == 2)) { pose = 0; timerbase = 0x0000UL;}  //Verify TIM pose
   else if(nTimer == 3) { pose = 1; timerbase = 0x0400UL;}
   else if(nTimer == 4) { pose = 2; timerbase = 0x0800UL;}
   else if(nTimer == 5) { pose = 3; timerbase = 0x0C00UL;}
   else if(nTimer == 9) { pose = 16; timerbase = 0x4000UL;}
   else if(nTimer == 10){ pose = 17; timerbase = 0x44000L;}
   else if(nTimer == 11){ pose = 18; timerbase = 0x4800UL;}
   
   if((nTimer == 2) || (nTimer == 3) || (nTimer == 4) || (nTimer == 5)) { 
   RCC->APB1ENR               |=  1UL << pose;   apbbase = APB1PERIPH_BASE;}                       // Enable TIMER clock
   else { RCC->APB2ENR  |=  1UL << pose; apbbase = APB2PERIPH_BASE;}                                       

   float fsys = 0;
   if((RCC->CFGR & (3<<0)) == 2)        { fsys = 84; }
   else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16; }

   float pscvalue = 84 * usec - 1;

   float fclk = fsys /(pscvalue+1)*pow(10,6);
   
   float arrvalue = 0xFFFF;
   
   ((TIM_TypeDef *) (apbbase + timerbase))->PSC   =   pscvalue;
   ((TIM_TypeDef *) (apbbase + timerbase))->ARR   =   arrvalue;
}


void TIME_pinmap(TIM_t* timer_pin) {
   //through GPIOX, pinnumber, chnnel is setted as diffrently.
    //through pin map, channel is setted as right value.
   if (timer_pin->port == GPIOA) {             
      if(timer_pin->pin == 0)   { timer_pin->ch = 1; timer_pin->timer = TIM2; }
    if(timer_pin->pin == 1)   { timer_pin->ch = 2; timer_pin->timer = TIM2; }
      if(timer_pin->pin == 5)   { timer_pin->ch = 1; timer_pin->timer = TIM2; }
    if(timer_pin->pin == 6)   { timer_pin->ch = 1; timer_pin->timer = TIM3; }
      if(timer_pin->pin == 8)   { timer_pin->ch = 1; timer_pin->timer = TIM1; }
      if(timer_pin->pin == 9)   { timer_pin->ch = 2; timer_pin->timer = TIM1; }
    if(timer_pin->pin == 10){ timer_pin->ch = 3; timer_pin->timer = TIM1; }
      if(timer_pin->pin == 15){ timer_pin->ch = 1; timer_pin->timer = TIM2; }
   }
   if(timer_pin->port == GPIOB) {
    if(timer_pin->pin == 3)   { timer_pin->ch = 2; timer_pin->timer = TIM2; }
    if(timer_pin->pin == 4) { timer_pin->ch = 1; timer_pin->timer = TIM3; }
    if(timer_pin->pin == 5) { timer_pin->ch = 2; timer_pin->timer = TIM3; }
    if(timer_pin->pin == 6) { timer_pin->ch = 1; timer_pin->timer = TIM4; }
    if(timer_pin->pin == 7) { timer_pin->ch = 2; timer_pin->timer = TIM4; }
    if(timer_pin->pin == 8) { timer_pin->ch = 3; timer_pin->timer = TIM4; }
    if(timer_pin->pin == 9) { timer_pin->ch = 4; timer_pin->timer = TIM4; }
      if(timer_pin->pin == 10){ timer_pin->ch = 3; timer_pin->timer = TIM2; }
   }
   if(timer_pin->port == GPIOC){
      if(timer_pin->pin == 6) { timer_pin->ch = 1; timer_pin->timer = TIM3; }
    if(timer_pin->pin == 7) { timer_pin->ch = 2; timer_pin->timer = TIM3; }
    if(timer_pin->pin == 8) { timer_pin->ch = 3; timer_pin->timer = TIM3; }
    if(timer_pin->pin == 9) { timer_pin->ch = 4; timer_pin->timer = TIM3; }
  }   
}


void CAP_init(TIM_t* cap_pin, GPIO_TypeDef* port, int pin) {
   int af;
   
   cap_pin->port = port;
  cap_pin->pin    = pin;
   
   TIME_pinmap(cap_pin);
   
   
   if((cap_pin->timer == TIM1) || (cap_pin->timer == TIM2)) { //Verify from AF0 to AF11
      af = 1; } 
   else if ((cap_pin->timer == TIM3) || (cap_pin->timer == TIM4) || (cap_pin->timer == TIM5)) {
      af = 2; }
   else if ((cap_pin->timer == TIM9) || (cap_pin->timer == TIM10) || (cap_pin->timer == TIM11)) {
      af = 3; }

   if((pin >= 0) &&  (pin <= 7)) {
      cap_pin->port->AFR[0] &= ~(7<<(4*pin));
      cap_pin->port->AFR[0] |= af <<(4*pin);}
   else if((pin >= 8) &&  (pin <= 15)) {
      cap_pin->port->AFR[1] &= ~(7<<(4*(pin%8)));
      cap_pin->port->AFR[1] |= af <<(4*(pin%8));}
   
   if(cap_pin->ch == 1) {                                                    // Set CCMR1 or CCMR2
      cap_pin->timer->CCMR1    &= ~TIM_CCMR1_OC1M;
    cap_pin->timer->CCMR1    &= ~TIM_CCMR1_OC2M;                     // Clear ouput compare mode bits for channel 1       // Set OC1M value for PWM Mode 1 output on ch1
      cap_pin->timer->CCMR1     |= (1<<0);
      cap_pin->timer->CCMR1     |= (2<<8);
      
   } else if(cap_pin->ch == 2) {
      cap_pin->timer->CCMR1    &= ~TIM_CCMR1_OC1M;
    cap_pin->timer->CCMR1    &= ~TIM_CCMR1_OC2M; 
      cap_pin->timer->CCMR1     |= (2<<0);
      cap_pin->timer->CCMR1     |= (1<<8);
   }   else if(cap_pin->ch == 3) {   
    cap_pin->timer->CCMR2    &= ~TIM_CCMR2_CC3S;
    cap_pin->timer->CCMR2    &= ~TIM_CCMR2_CC4S; 
      cap_pin->timer->CCMR2     |= (1<<0); //TI3
      cap_pin->timer->CCMR2     |= (2<<8); //TI3
   } else if(cap_pin->ch == 4) {
    cap_pin->timer->CCMR2    &= ~TIM_CCMR2_CC3S;
    cap_pin->timer->CCMR2    &= ~TIM_CCMR2_CC4S; 
      cap_pin->timer->CCMR2     |= (2<<0);
      cap_pin->timer->CCMR2     |= (1<<8);
   }      
   
      if((cap_pin->ch == 1) || (cap_pin->ch == 2)) {                                       // Set CCMR1 or CCMR2
      cap_pin->timer->CCER |= 1;                                          
      cap_pin->timer->CCER &= ~(1<<1);                                      // rising edge                                   
      cap_pin->timer->CCER &= ~(1<<3);
   
      cap_pin->timer->CCER |= (1<<4);                                       
      cap_pin->timer->CCER |= (1<<5);                                       // falling edge
      cap_pin->timer->CCER &= ~(1<<7);
   } else if((cap_pin->ch == 3) || (cap_pin->ch == 4)) {
      cap_pin->timer->CCER |= (1<<8);                                          
      cap_pin->timer->CCER &= ~(1<<9);                                       
      cap_pin->timer->CCER &= ~(1<<11);                  

      cap_pin->timer->CCER |= (1<<12);
      cap_pin->timer->CCER |= (1<<13);
      cap_pin->timer->CCER &= ~(1<<15);
   }
   
   
   if((cap_pin->ch == 1) || (cap_pin->ch == 2)) {                                       // Set CCMR1 or CCMR2
      cap_pin->timer->DIER |= (1<<1);
      cap_pin->timer->DIER |= (1<<2);
   } else if((cap_pin->ch == 3) || (cap_pin->ch == 4)) {
      cap_pin->timer->DIER |= (1<<3);
      cap_pin->timer->DIER |= (1<<4);
   }
   
   cap_pin->timer->DIER |= 1;                                          // Update interrupt enable   

   cap_pin->timer->CR1  |= 1;                                          // Counter enable
}
void TIM5_Init(float msec){

   RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; 
   
   TIM5->CR1        &= ~(1<<4); // up counter

  TIM5->CCMR1    &= ~(7<<4);    
  TIM5->CCMR1   |= 6<<4;           //  output compare pwm_1 mode 
   
   
   TIM5->CCER |= 1;         // CC1 Capture enabled    
  TIM5->CCER &= ~(7<<1) ;         // CC1 Capture rising edge  
   TIM5->CCR1 = (10*msec)-1;
   
  TIM5->CR2 &= ~(7<<4); // timer master mode    OC1REF to TRGO
  TIM5->CR2 |= 4<<4;
   
   TIM5->PSC = 8399 ;   // Timer counter clock: 84Mhz / PSC + 1 = 10Khz
  TIM5-> ARR = (10*msec)-1 ;         // Set auto reload register                         
   TIM5->DIER |= 1 ;   
   
   TIM5->CR1  |= 1;                  // Enable counter
   
 
}
void TIM9_Init(float msec){

   RCC->APB2ENR |= RCC_APB2ENR_TIM9EN; 

   TIM9->CCMR1    &= ~(7<<4);    
   TIM9->CCMR1   |= 6<<4;     //  output compare pwm_1 mode 

   TIM9->CCER |= 1;         // CC1 Capture enabled    
   TIM9->CCER &= ~(4<<1) ;     // CC1 Capture rising edge  
    TIM9->CCR1 = (10*msec)-1;
   
   
   TIM9->PSC = 8299 ;   // Timer counter clock: 84Mhz / PSC + 1 = 10Khz
   TIM9-> ARR = (10*msec)-1 ;     // Set auto reload register                         
   TIM9->DIER |= 1 ;   
   
   TIM9->CR1  |= 1;              // Enable counter
   
 
}