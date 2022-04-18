#include	"myInclude.h"

uint32_t ADC_pinmap(GPIO_TypeDef *port, uint32_t pin) {

	if(port==GPIOA) return pin%8; // GPIOA pinmap channel
	else if(port==GPIOB) return pin%2+8; // GPIOB pinmap channel
	else if(port==GPIOC) return pin%6+10; // GPIOB pinmap channel
}

void ADC_init(GPIO_TypeDef *port,  int pin, int mode){
  uint32_t channel;
	
	channel =  ADC_pinmap(port,pin);
	if(mode == POL || mode == INT){
		 printf("channel = %d\n",channel);
	 RCC->APB2ENR &= ~(1<<8);
   RCC->APB2ENR |= 1<<8;         //ADC clock setting
	
	 ADC ->CCR &=~(3<<16); //reset 
   ADC ->CCR |=0<<16;    //PCLK2/2
	 
	 ADC1->CR2 &=~(1<<1);  //reset
   ADC1->CR2 |= 1<<1;    //continuouis conversion mode
	
	 ADC1->CR2 &=~(1<<11); //reset
   ADC1->CR2 |= 0<<11;   //right allignment
	
	 ADC1->CR1 &=~(3<<24);   //reset
   ADC1->CR1 |= 0<<24;     //12bit resolution
	
	 ADC1->SQR1 &= ~(0xF<<20); //reset
	 ADC1->SQR1 |= 0<<20;      //1 conversion in the regular channel
   
	 ADC1 -> SQR3 &= ~31; // reset
	 ADC1 -> SQR3 |= channel; // channel setting
	 
	
	 if(channel<10){                    // 0~9 channel
	 	 ADC1->SMPR2 &= ~(7<<channel*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<channel*3;
	 }
   else if(channel>9){                          // 10~18 channel
	 	 ADC1->SMPR2 &= ~(7<<(channel%10)*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<(channel%10)*3;
	 }
	 
	 ADC1->CR1 &= ~(1<<5);    //interrupt enable
	 ADC1->CR1 |= (1<<5);
	 
	 ADC1->CR2 &= ~(1<<0);
   ADC1->CR2 |= 1<<0;
	 ADC1->CR2 &= ~(1<<30);
   ADC1->CR2 |= 1<<30;
  
	 if(mode==INT){ 
	 NVIC_SetPriority(ADC_IRQn,2); //NVIC interrupt setting
	 NVIC_EnableIRQ(ADC_IRQn);     //Enable NVIC
	 }
	}
	

 
	else if(mode==EXT){
	 printf("channel = %d\n",channel);
	 RCC->APB2ENR &= ~(1<<8);
   RCC->APB2ENR |= 1<<8;         //ADC clock setting
	
	 ADC ->CCR &=~(3<<16); //reset 
   ADC ->CCR |=0<<16;    //PCLK2/2
	 
	 ADC1->CR2 &=~(1<<1);  //reset
   //ADC1->CR2 |= 1<<1;    //continuouis conversion mode
	
	 ADC1->CR2 &=~(1<<11); //reset
   ADC1->CR2 |= 0<<11;   //right allignment
	
	 ADC1->CR1 &=~(3<<24);   //reset
   ADC1->CR1 |= 0<<24;     //12bit resolution
	
	 ADC1->SQR1 &= ~(0xF<<20); //reset
	 ADC1->SQR1 |= 0<<20;      //1 conversion in the regular channel
   
	 ADC1 -> SQR3 &= ~31; // reset
	 ADC1 -> SQR3 |= channel; // channel setting
	 
	
	 if(channel<10){                    // 0~9 channel
	 	 ADC1->SMPR2 &= ~(7<<channel*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<channel*3;
	 }
   else if(channel>9){                          // 10~18 channel
	 	 ADC1->SMPR2 &= ~(7<<(channel%10)*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<(channel%10)*3;
	 }
	 
	 ADC1->CR2 &=~(0xF<<24);
   ADC1->CR2 |= 8<<24;
	 
	 ADC1->CR2 &=~(3<<28);
   ADC1->CR2 |= 1<<28;
	 
	 ADC1->CR1 &= ~(1<<5);    //interrupt enable
	 ADC1->CR1 |= (1<<5);
	 
	 ADC1->CR2 &= ~(1<<0);
   ADC1->CR2 |= 1<<0;
	 ADC1->CR2 &= ~(1<<30);
   ADC1->CR2 |= 1<<30;
	
	 NVIC_SetPriority(ADC_IRQn,1); //NVIC interrupt setting
	 NVIC_EnableIRQ(ADC_IRQn);     //Enable NVIC
	}
}

uint32_t ADC_read(){
    if(ADC1->SR & ADC_SR_EOC){return ADC1->DR;}
		
	
}

void ADC_init_TRGO(GPIO_TypeDef *port, int pin){
	
  uint32_t channel;
	
	channel =  ADC_pinmap(port,pin);
	
	 printf("channel = %d\n",channel);
	 RCC->APB2ENR &= ~(1<<8);
   RCC->APB2ENR |= 1<<8;         //ADC clock setting
	
	 ADC ->CCR &=~(3<<16); //reset 
   ADC ->CCR |=0<<16;    //PCLK2/2
	 
	 ADC1->CR2 &=~(1<<1);  //reset
   //ADC1->CR2 |= 1<<1;    //continuouis conversion mode
	
	 ADC1->CR2 &=~(1<<11); //reset
   ADC1->CR2 |= 0<<11;   //right allignment
	
	 ADC1->CR1 &=~(3<<24);   //reset
   ADC1->CR1 |= 0<<24;     //12bit resolution
	
	 ADC1->SQR1 &= ~(0xF<<20); //reset
	 ADC1->SQR1 |= 0<<20;      //1 conversion in the regular channel
   
	 ADC1 -> SQR3 &= ~31; // reset
	 ADC1 -> SQR3 |= channel; // channel setting
	 
	
	 if(channel<10){                    // 0~9 channel
	 	 ADC1->SMPR2 &= ~(7<<channel*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<channel*3;
	 }
   else if(channel>9){                          // 10~18 channel
	 	 ADC1->SMPR2 &= ~(7<<(channel%10)*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<(channel%10)*3;
	 }
	 

	 
	 ADC1->CR2 &=~(0xF<<24);
   ADC1->CR2 |= 8<<24;
	 
	 ADC1->CR2 &=~(3<<28);
   ADC1->CR2 |= 1<<28;
	 
	 ADC1->CR1 &= ~(1<<5);    //interrupt enable
	 ADC1->CR1 |= (1<<5);
	 
	 ADC1->CR2 &= ~(1<<0);
   ADC1->CR2 |= 1<<0;
	 ADC1->CR2 &= ~(1<<30);
   ADC1->CR2 |= 1<<30;
	
	 NVIC_SetPriority(ADC_IRQn,1); //NVIC interrupt setting
	 NVIC_EnableIRQ(ADC_IRQn);     //Enable NVIC
	
}



void TIM3_Init(float msec){

    // Default PLL 84MHz - 0.1<msec<1000 //
    if(msec>1000 || msec<0.1) {msec = 100;}
   
    // Enable Clock for Timer3 //
   RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
   
    // Up-counter //
   TIM3->CR1 &= ~(1<<4);
   
    // OC1M : output compare 1 Mode //
     // OC1M = 110 for compare 1 Mode ch1 //
   TIM3->CCMR1 &= ~(7<<4);
   TIM3->CCMR1 |= 6<<4; //
   
   // Master Mode Selection: MMS[2:0] //
    // Select OC1REF as TRGO //
    //100: Compare - OC1REF signal is used as trigger output (TRGO)
   TIM3->CR2 &=~(7<<4);
    TIM3->CR2 |= 4<<4;
    
   //Configure Timer Period -> 1kHz
   TIM3->PSC = 83999;
    
    float value;
    
    value = msec *(84000/(TIM3->PSC+1));
    TIM3->ARR &= ~(0xFFFFFFFF); 
    TIM3->ARR = (value-1);
   //TIM3->ARR = 1000*msec-1;
    
    TIM3->CCER &= ~TIM_CCER_CC1P;
   
    // OC1 signal //
   TIM3->CCER |=1;
    TIM3->CCR1  = value-1;
   
    // Enable TIM3 //
   TIM3->CR1 |= 1; //counter enable
    
}
void TIM_Init(float msec,TIM_TypeDef *TIMx){

    // Default PLL 84MHz - 0.1<msec<1000 //
    if(msec>1000 || msec<0.1) {msec = 100;}
   
    // Enable Clock for Timer //
		if(TIMx==TIM1) RCC->APB2ENR |=RCC_APB2ENR_TIM1EN;
		if(TIMx==TIM2) RCC->APB1ENR |=RCC_APB1ENR_TIM2EN;
		if(TIMx==TIM3) RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
		if(TIMx==TIM4) RCC->APB1ENR |=RCC_APB1ENR_TIM4EN;
		if(TIMx==TIM5) RCC->APB1ENR |=RCC_APB1ENR_TIM5EN;
		if(TIMx==TIM9) RCC->APB2ENR |=RCC_APB2ENR_TIM9EN;
		if(TIMx==TIM10) RCC->APB2ENR |=RCC_APB2ENR_TIM10EN;
		if(TIMx==TIM11) RCC->APB2ENR |=RCC_APB2ENR_TIM11EN;
   
    // Up-counter //
   TIMx->CR1 &= ~(1<<4);
   
    // OC1M : output compare 1 Mode //
     // OC1M = 110 for compare 1 Mode ch1 //
   TIMx->CCMR1 &= ~(7<<4);
   TIMx->CCMR1 |= 6<<4; //
   
   // Master Mode Selection: MMS[2:0] //
    // Select OC1REF as TRGO //
    //100: Compare - OC1REF signal is used as trigger output (TRGO)
   TIMx->CR2 &=~(7<<4);
    TIMx->CR2 |= 4<<4;
    
   //Configure Timer Period -> 1kHz
   TIMx->PSC = 83999;
    
    float value;
    
    value = msec *(84000/(TIM3->PSC+1));
    TIMx->ARR &= ~(0xFFFFFFFF); 
    TIMx->ARR = (value-1);
   //TIM3->ARR = 1000*msec-1;
    
    TIMx->CCER &= ~TIM_CCER_CC1P;
   
    // OC1 signal //
   TIMx->CCER |=1;
    TIMx->CCR1  = value-1;
   //TIMx->DIER |= 1;
    // Enable TIMx //
   TIMx->CR1 |= 1; //counter enable
    
}

void Timer_Init(float msec,TIM_TypeDef *TIMx){
    float value;
	
	    // Default PLL 84MHz - 0.1<msec<1000 //
    if(msec>1000 || msec<0.1) {msec = 100;}
   
    // Enable Clock for Timer //
		if(TIMx==TIM1) RCC->APB2ENR |=RCC_APB2ENR_TIM1EN;
		if(TIMx==TIM2) RCC->APB1ENR |=RCC_APB1ENR_TIM2EN;
		if(TIMx==TIM3) RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
		if(TIMx==TIM4) RCC->APB1ENR |=RCC_APB1ENR_TIM4EN;
		if(TIMx==TIM5) RCC->APB1ENR |=RCC_APB1ENR_TIM5EN;
		if(TIMx==TIM9) RCC->APB2ENR |=RCC_APB2ENR_TIM9EN;
		if(TIMx==TIM10) RCC->APB2ENR |=RCC_APB2ENR_TIM10EN;
		if(TIMx==TIM11) RCC->APB2ENR |=RCC_APB2ENR_TIM11EN;
		
		TIMx->CR1 &= ~TIM_CR1_DIR; // counter used as upcounter
		TIMx->PSC = 83999;
		value = msec *(84000/(TIM3->PSC+1));
    TIMx->ARR &= ~(0xFFFFFFFF); 
    TIMx->ARR = (value-1);
	  TIMx->DIER |= TIM_DIER_UIE;
		TIMx->CR1 |= TIM_CR1_CEN;
}

void JADC_init(GPIO_TypeDef *port,  int pin){
  uint32_t channel;
	
	channel =  ADC_pinmap(port,pin);

		 printf("channel = %d\n",channel);
	 RCC->APB2ENR &= ~(1<<8);
   RCC->APB2ENR |= 1<<8;         //ADC clock setting
	
	 ADC ->CCR &=~(3<<16); //reset 
   ADC ->CCR |=0<<16;    //PCLK2/2
	 
	 ADC1->CR2 &=~(1<<1);  //reset
   ADC1->CR2 |= 1<<1;    //continuouis conversion mode
	
	 ADC1->CR2 &=~(1<<11); //reset
   ADC1->CR2 |= 0<<11;   //right allignment
	
	 ADC1->CR1 &=~(3<<24);   //reset
   ADC1->CR1 |= 0<<24;     //12bit resolution
	
	 ADC1->CR1 &= ~(1<<8); //reset
	 ADC1->CR1 |= 1<<8;    //SCAN mode
	 
	 ADC1->JSQR &= ~(3<<20); //reset
	 ADC1->JSQR |= 1<<20;      //1 conversion in the regular channel
   
	 ADC1 -> JSQR &= ~(31<<10); // reset
	 ADC1 -> JSQR |= 1<<10; // channel setting
	 
	 ADC1 -> JSQR &= ~(31<<15); // reset
	 ADC1 -> JSQR |= channel<<15; // channel setting
	 
	
	 if(channel<10){                    // 0~9 channel
	 	 ADC1->SMPR2 &= ~(7<<channel*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<channel*3;
	 }
	 
   else if(channel>9){                          // 10~18 channel
	 	 ADC1->SMPR2 &= ~(7<<(channel%10)*3);  // channel x samling time 84
	   ADC1->SMPR2 |= 4<<(channel%10)*3;
	 }
	 ADC1->SMPR2 &= ~(7<<1*3);  // channel 1 samling time 84
	 ADC1->SMPR2 |= 4<<1*3;
	 
	 ADC1->CR2 &=~(0xF<<16);
   ADC1->CR2 |= 11<<16;
	 
	 ADC1->CR2 &=~(3<<20);
   ADC1->CR2 |= 1<<20;
	 
	 ADC1->CR1 &= ~(1<<7);    // JEOC interrupt enable
	 ADC1->CR1 |= (1<<7);
	 
	 ADC1->CR2 &= ~(1<<0);
   ADC1->CR2 |= 1<<0;
	 ADC1->CR2 &= ~(1<<22);
   ADC1->CR2 |= 1<<22;
	
	 NVIC_SetPriority(ADC_IRQn,1); //NVIC interrupt setting
	 NVIC_EnableIRQ(ADC_IRQn);     //Enable NVIC
	}



