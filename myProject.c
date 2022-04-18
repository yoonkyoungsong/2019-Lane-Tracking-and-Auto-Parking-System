#include "myInclude.h"

// DC Motor parameter//
PWM_t pwm2, pwm3, pwm4, pwm5;

// Servo Motor parameter//
PWM_t pwm1;

void Set_TIM9(){
	
	/*----------- TIM9 ---------------*/
	TIM9_Init(0.5);                            // 100Hz ,10msec
	NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 5);  // Set the priority of TIM9 interrupt request
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);	

}


void Set_ADC(){
	
	/*----------- ADC IR sensor (PA0) ---------------*/
	TIM5_Init(1);
  GPIO_init (GPIOA, 0, ANALOG); //GPIO
  GPIO_init (GPIOA, 1, ANALOG); //GPIO
  JADC_init(GPIOA,0);           //JADC_init for  GPIOA 0,1 continous coversion mode

}


void Set_Inputcapture(){
	
	/*----------- Trigger Generation(PC8) -----------*/
   
	 GPIO_init(GPIOC, 8, ALTE);
   
   PWM_t   trig;
   PWM_init(&trig, GPIOC, 8);

   PWM_period_ms(&trig, 25);
   PWM_pulsewidth_ms(&trig, 0.01);
   
  /*------------ Echo Reception(PB8) -------------*/
   GPIO_init(GPIOB, 8, ALTE);
   TIME_period_us(4, 1);           // TIM4, 1us(1MHz) counter

   TIM_t   echo;
   CAP_init(&echo, GPIOB, 8);
   
   NVIC_SetPriority(TIM4_IRQn, 2); // Set the priority of TIM4 interrupt request
   NVIC_EnableIRQ(TIM4_IRQn);      // TIM4 interrupt request enable
   printf("\r\nhi UT");

}


void Set_Servomotor(){

	/*------------ RC servo motor       -------------*/ 	 
	 
	GPIO_init(GPIOA, 6, ALTE);       // GPIOA pin6 set as ALTERNATE
	GPIO_ospeed(GPIOA, 6, HIGH);	   // give speed as high
	GPIO_pupd(GPIOA, 6, NOPUPD);     // No pullup pulldown
	
	PWM_init(&pwm1, GPIOA, 6);       // PWM init as GPIOA pin6
  PWM_period_ms(&pwm1, 25);        // set period as 25ms
	
	PWM_duty(&pwm1, (0.125));        // degree 0.025 ~ 0.125
	
	//printf("\r\nhi RC servo");
	
}


void Set_DCmotor(){
	
		/*------------ DC motor (TIM1)       -------------*/  

	GPIO_init(GPIOA, 8, ALTE);     // GPIOA pin8 set as ALTERNATE
	GPIO_ospeed(GPIOA, 8, HIGH);	   // give speed as high
	GPIO_pupd(GPIOA, 8, NOPUPD);     // No pullup pulldown
	
  GPIO_init(GPIOA, 9, ALTE);     // GPIOA pin8 set as ALTERNATE
	GPIO_ospeed(GPIOA, 9, HIGH);	   // give speed as high
	GPIO_pupd(GPIOA, 9, NOPUPD);     // No pullup pulldown
	
	GPIO_init(GPIOA, 10, ALTE);     // GPIOA pin8 set as ALTERNATE
	GPIO_ospeed(GPIOA, 10, HIGH);	   // give speed as high
	GPIO_pupd(GPIOA, 10, NOPUPD);     // No pullup pulldown
	
	GPIO_init(GPIOA, 11, ALTE);     // GPIOA pin8 set as ALTERNATE
	GPIO_ospeed(GPIOA, 11, HIGH);	   // give speed as high
	GPIO_pupd(GPIOA, 11, NOPUPD);     // No pullup pulldown
	
	PWM_init(&pwm2, GPIOA, 8);        // PWM init as GPIOA pin8
  PWM_period_ms(&pwm2, 1);         // set period as 1ms - 1kHz
		
	PWM_init(&pwm3, GPIOA, 9);        // PWM init as GPIOA pin8
  PWM_period_ms(&pwm3, 1);         // set period as 1ms - 1kHz
		
	PWM_init(&pwm4, GPIOA, 10);        // PWM init as GPIOA pin8
  PWM_period_ms(&pwm4, 1);         // set period as 1ms - 1kHz
		
	PWM_init(&pwm5, GPIOA, 11);        // PWM init as GPIOA pin8
  PWM_period_ms(&pwm5, 1);         // set period as 1ms - 1kHz
	
}

void Front(float leftSpeed, float rightSpeed){
	
	PWM_duty(&pwm2, leftSpeed);   // left
	PWM_duty(&pwm3, (0));
	PWM_duty(&pwm4, rightSpeed);   // right
	PWM_duty(&pwm5, (0));

}

void Back(float leftSpeed, float rightSpeed){

	PWM_duty(&pwm2, 0);   // left
	PWM_duty(&pwm3, leftSpeed);
	PWM_duty(&pwm4, 0);   // right
	PWM_duty(&pwm5, rightSpeed);

}

void Stop(){

	PWM_duty(&pwm2, 0);   // left
	PWM_duty(&pwm3, 0);
	PWM_duty(&pwm4, 0);   // right
	PWM_duty(&pwm5, 0);

}

void Front_speed(float Speed1,float Speed2,float Speed3, float Speed4){
	
	PWM_duty(&pwm2, Speed1);   // left
	PWM_duty(&pwm3, Speed2);
	PWM_duty(&pwm4, Speed3);   // right
	PWM_duty(&pwm5, Speed4);

}


