

#include   "myInclude.h"

void PWM_pinmap(PWM_t *pwm){
   //through GPIOX, pinnumber, chnnel is setted as diffrently.
    //through pin map, channel is setted as right value.
if (pwm->port == GPIOA) {             
    if(pwm->pin == 0)   { pwm->ch = 1; pwm->timer = TIM2; }
    if(pwm->pin == 1)   { pwm->ch = 2; pwm->timer = TIM2; }
    if(pwm->pin == 5)   { pwm->ch = 1; pwm->timer = TIM2; }
    if(pwm->pin == 6)   { pwm->ch = 1; pwm->timer = TIM3; }
    if(pwm->pin == 8)   { pwm->ch = 1; pwm->timer = TIM1; }
    if(pwm->pin == 9)   { pwm->ch = 2; pwm->timer = TIM1; }
		if(pwm->pin == 11)  { pwm->ch = 4; pwm->timer = TIM1; } //TIMER1 for GPIOA pin11(without LAB PINMAP)
		
  if(pwm->pin == 10)  { pwm->ch = 3; pwm->timer = TIM1; }
    if(pwm->pin == 15)  { pwm->ch = 1; pwm->timer = TIM2; }
   }
   if(pwm->port == GPIOB) {
    if(pwm->pin == 3)   { pwm->ch = 2; pwm->timer = TIM2; }
    if(pwm->pin == 4)   { pwm->ch = 1; pwm->timer = TIM3; }
    if(pwm->pin == 5)   { pwm->ch = 2; pwm->timer = TIM3; }
    if(pwm->pin == 6)   { pwm->ch = 1; pwm->timer = TIM4; }
    if(pwm->pin == 7)   { pwm->ch = 2; pwm->timer = TIM4; }
    if(pwm->pin == 8)   { pwm->ch = 3; pwm->timer = TIM4; }
    if(pwm->pin == 9)   { pwm->ch = 4; pwm->timer = TIM4; }
      if(pwm->pin == 10){ pwm->ch = 3; pwm->timer = TIM2; }
   }
   if(pwm->port == GPIOC){
      if(pwm->pin == 6) { pwm->ch = 1; pwm->timer = TIM3; }
    if(pwm->pin == 7) { pwm->ch = 2; pwm->timer = TIM3; }
    if(pwm->pin == 8) { pwm->ch = 3; pwm->timer = TIM3; }
    if(pwm->pin == 9) { pwm->ch = 4; pwm->timer = TIM3; }
  }  
}
void  Timer_interupt_init(PWM_t *pwm, GPIO_TypeDef *Port, int pin){
int af;
   pwm->port = Port;
  pwm->pin = pin;
   
  PWM_pinmap(pwm);
   if((pwm->timer == TIM1) || (pwm->timer == TIM2)) { //Verify from AF0 to AF11
      af = 1; }
   else if ((pwm->timer == TIM3) || (pwm->timer == TIM4) || (pwm->timer == TIM5)) {
      af = 2; }
   else if ((pwm->timer == TIM9) || (pwm->timer == TIM10) || (pwm->timer == TIM11)) {
      af = 3; }

   if((0 <= pin) &  (pin <= 7)) {
      pwm->port->AFR[0] &= ~(15<<(4*pin));
      pwm->port->AFR[0] |= af <<(4*pin);}
   else if((8 <= pin) &  (pin <= 15)) {
      pwm->port->AFR[1] &= ~(15<<(4*(pin%8)));
      pwm->port->AFR[1] |= af <<(4*(pin%8));}

   int pose;
   if((pwm->timer == TIM1)||(pwm->timer == TIM2)) { pose = 0; }  //Verify TIM pose
   else if(pwm->timer == TIM3) { pose = 1; }
   else if(pwm->timer == TIM4) { pose = 2; }
   else if(pwm->timer == TIM5) { pose = 3; }
   else if(pwm->timer == TIM9) { pose = 16; }
   else if(pwm->timer == TIM10){ pose = 17; }
   else if(pwm->timer == TIM11){ pose = 18; }

   if((pwm->timer == TIM2) || (pwm->timer == TIM3) || (pwm->timer == TIM4) || (pwm->timer == TIM5)) { 
   RCC->APB1ENR               |=  1UL << pose;      }                       // Enable TIMER clock
   else { RCC->APB2ENR  |=  1UL << pose;
   if(pwm->timer == TIM1) {TIM1->BDTR = TIM_BDTR_MOE ;}
   }
   
   pwm->timer->CR1           &= ~(1UL<<4);                                   // Counting direction: 0 = up-counting, 1 = down-counting
   
   
   float fsys = 0;
   if((RCC->CFGR & (3<<0)) == 2)        { fsys = 84; }
   else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16; }
   
  pwm->timer->PSC = fsys-1;                                  // f_cnt = 1MHz 
                              
   pwm->timer->ARR = 999999;                                                // Auto-reload: Upcouting (0..ARR)
   
      if((pwm->ch%2) == 1)          {pose = 3;}                                       //Verify channel by %
   else if((pwm->ch%2) == 0) {pose = 11;}
   
   if((pwm->ch>=1) & (pwm->ch<= 2)) {                                       // Set CCMR1 or CCMR2
   pwm->timer->CCMR1 &= ~(0xF << pose);                    // Clear ouput compare mode bits for channel 1
   } else if((pwm->ch>=3) & (pwm->ch<=4)) {
   pwm->timer->CCMR2 &= ~(0xF << pose);
   }
   
       
   TIM2->DIER |= TIM_DIER_UIE;
   

   
  pwm->timer->CR1      &=~TIM_CR1_CEN;                         // Counter reset      
   pwm->timer->CR1        |= TIM_CR1_CEN;                              // Enable counter
}
   
void   PWM_init(PWM_t *pwm, GPIO_TypeDef *Port, int pin) {
   int af;
   pwm->port = Port;
  pwm->pin = pin;
   
  PWM_pinmap(pwm);
   if((pwm->timer == TIM1) || (pwm->timer == TIM2)) { //Verify from AF0 to AF11
      af = 1; }
   else if ((pwm->timer == TIM3) || (pwm->timer == TIM4) || (pwm->timer == TIM5)) {
      af = 2; }
   else if ((pwm->timer == TIM9) || (pwm->timer == TIM10) || (pwm->timer == TIM11)) {
      af = 3; }

   if((0 <= pin) &  (pin <= 7)) {
      pwm->port->AFR[0] &= ~(7<<(4*pin));
      pwm->port->AFR[0] |= af <<(4*pin);}
   else if((8 <= pin) &  (pin <= 15)) {
      pwm->port->AFR[1] &= ~(7<<(4*(pin%8)));
      pwm->port->AFR[1] |= af <<(4*(pin%8));}

   int pose;
   if((pwm->timer == TIM1)||(pwm->timer == TIM2)) { pose = 0; }  //Verify TIM pose
   else if(pwm->timer == TIM3) { pose = 1; }
   else if(pwm->timer == TIM4) { pose = 2; }
   else if(pwm->timer == TIM5) { pose = 3; }
   else if(pwm->timer == TIM9) { pose = 16; }
   else if(pwm->timer == TIM10){ pose = 17; }
   else if(pwm->timer == TIM11){ pose = 18; }

   if((pwm->timer == TIM2) || (pwm->timer == TIM3) || (pwm->timer == TIM4) || (pwm->timer == TIM5)) { 
   RCC->APB1ENR               |=  1UL << pose;      }                       // Enable TIMER clock
   else { RCC->APB2ENR  |=  1UL << pose;
   if(pwm->timer == TIM1) {TIM1->BDTR = TIM_BDTR_MOE ;}
   }
   
   pwm->timer->CR1           &= ~(1UL<<4);                                   // Counting direction: 0 = up-counting, 1 = down-counting
   
   
   float fsys = 0;
   if((RCC->CFGR & (3<<0)) == 2)        { fsys = 84; }
   else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16; }
   
  pwm->timer->PSC = fsys-1;                                  // f_cnt = 1MHz 
                              
   pwm->timer->ARR = 999;                                                // Auto-reload: Upcouting (0..ARR)
   

   if((pwm->ch%2) == 1)          {pose = 3;}                                       //Verify channel by %
   else if((pwm->ch%2) == 0) {pose = 11;}
   
   if((pwm->ch>=1) & (pwm->ch<= 2)) {                                       // Set CCMR1 or CCMR2
      pwm->timer->CCMR1 &= ~(0xF << pose);                    // Clear ouput compare mode bits for channel 1
      pwm->timer->CCMR1 |=  (13<<pose);                                     // Set OC1M value for PWM Mode 1 output on ch1
   } else if((pwm->ch>=3) & (pwm->ch<=4)) {
      pwm->timer->CCMR2 &= ~(0xF << pose);
      pwm->timer->CCMR2 |=   (13<<pose);
   
   }
   
   pose = 4*(pwm->ch-1);                   
   pwm->timer->CCER       &= ~(3<<pose);                                        
   pwm->timer->CCER       |= (1<<pose);                                        // select active high Enable output for CHn          

   
   if(pwm->ch == 1)          {pwm->timer->CCR1 = 499;}                  // Set CCRx by channel
   else if(pwm->ch == 2) {pwm->timer->CCR2 = 499;}
   else if(pwm->ch == 3) {pwm->timer->CCR3 = 499;}
   else if(pwm->ch == 4) {pwm->timer->CCR4 = 499;}
   
  pwm->timer->CR1      &=~TIM_CR1_CEN;                         // Counter reset      
   pwm->timer->CR1        |= TIM_CR1_CEN;                              // Enable counter
      
}
   
void   PWM_period_ms(PWM_t *pwm, float period_ms) {            //set PWM period as ms
   float fsys = 0;
   if((RCC->CFGR & (3<<0)) == 2)          { fsys = 84; }
   else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16; }
   
   float fclk = fsys * pow(10,3)/((pwm->timer->PSC)+1);
   float value = period_ms  * fclk;
   
   pwm->timer->ARR   =   value - 1;
}
   
void   PWM_pulsewidth_ms(PWM_t *pwm, float pulsewidth_ms) {     //set pulse width as ms
   float fsys = 0;
   if((RCC->CFGR & (3<<0)) == 2)          { fsys = 84; }
   else if((RCC->CFGR & (3<<0)) == 0) { fsys = 16; }
   
   float fclk = fsys * pow(10,3)/((pwm->timer->PSC)+1);
   float value = pulsewidth_ms * fclk - 1;
   
  if(pwm->ch == 1)          { pwm->timer->CCR1 = value; }
  else if(pwm->ch == 2) { pwm->timer->CCR2 = value; }
  else if(pwm->ch == 3) { pwm->timer->CCR3 = value; }
  else if(pwm->ch == 4) { pwm->timer->CCR4 = value; }
}
   
void   PWM_duty(PWM_t *pwm, float duty) {                      //set duty ratio as %
   float value = ((pwm->timer->ARR) + 1) * duty - 1;           
   
   if(pwm->ch == 1)          { pwm->timer->CCR1 = value; }          //set channel      
   else if(pwm->ch == 2) { pwm->timer->CCR2 = value; }
   else if(pwm->ch == 3) { pwm->timer->CCR3 = value; }
   else if(pwm->ch == 4) { pwm->timer->CCR4 = value; }
   }
   
   