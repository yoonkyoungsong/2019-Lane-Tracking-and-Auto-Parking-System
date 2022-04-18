/*-----------------------------------------------------

             2019-2 Embeded Controller 1
                
					    			Final Project:
    Driving an RC Car with Automatic Parking System

               21300399 Hyeongseok Song
               21600372 Yoonkyoung Song

--------------------------------------------------------*/

#include "myInclude.h"
#include "myProject.h"

// IR Sensor parameter//
uint32_t IR1 = 0;
uint32_t IR2 = 0;
int flag_adc = 0;
uint32_t thresh = 1500;  // black 200~~500 whilte > 1000


// UT Sensor parameter//
int UT1 = 0;
float	timeInterval = 0;
int overf = 0;
float time1 = 0;
float time2 = 0;

// DC Motor parameter//
extern PWM_t pwm2, pwm3, pwm4, pwm5;


// Servo Motor parameter//
extern PWM_t pwm1;

//BT parameter//
uint8_t BT_Data = 1;
uint8_t PC_Data = 0;

//Systme//
int CNT=0;
int CNT_1=0;
int flag_p =0;


int main(void){
	
	 RCC_PLL_init();
	 SysTick_init();
	
   UART2_init(9600, POL);
	 BT_init(9600);
	 printf("Hello Nucleo\r\n");
	
	 Set_TIM9();
	 Set_TIM9();
	 Set_ADC();
	 Set_Inputcapture();
	 Set_Servomotor();
	 Set_DCmotor();
	 while(1){;}
		 
	 }

	void TIM1_BRK_TIM9_IRQHandler(void){  //for debug
		if(TIM9->SR & TIM_SR_UIF){
     
     if(timeInterval<=0) UT1=30;		
		 else UT1 = (float) timeInterval/58;
		 
		 if(CNT==1000){

			 CNT =0;
		 }
		 if(BT_Data=='1'&&flag_p==0){
		   PWM_duty(&pwm1, (0.06));
			 if(IR2<thresh){ 
			   Front(0.65,0.55);
		   }
		   else if(IR1<thresh){ 
		     Front(0.48,0.8);
		   }
		   else{
			   Front(0.85,0.9); 
		     if(UT1<17){     // wall detecting 
				   Stop();
			     flag_p=1;
		     }
		   }
	   }
		 else{
			 Stop();
		 }			 

		 if(flag_p==1){
			 PWM_duty(&pwm1, (0.125));	
			 Back(0.8,0.8);
		   if(UT1>32) flag_p=2;			 
		 }
		 
		 if(flag_p==2){
		   PWM_duty(&pwm1, (0.125));
			 Back(0.8,0.8);
			 if(UT1<25) flag_p=3;			 
		 }
		 
		 if(flag_p==3){
			 PWM_duty(&pwm1, (0.06));
			 Stop();
		   flag_p=4;		 
		 }
		 if(flag_p==4) CNT_1++;
		 
		 if(CNT_1==200) BT_Data='p';
		 
		 if(flag_p==4 & BT_Data=='p'){
			 Front(0.45,0.85);	 
			 if(UT1<28) flag_p=5;				 
		 }
		 if(flag_p==5){
			 Stop();
		   flag_p=6;			
		 }
	
		 if(flag_p==6){
			 PWM_duty(&pwm1, (0.125));
			 Back(0.7,0.7);
		   if(UT1>40) flag_p=7;			
		 }	
		 
		 if(flag_p==7){
			 Stop();
			 CNT_1=0;
			 flag_p=8;
		 }		 

		 if(flag_p==8){ CNT_1++; }
		 if(flag_p==8 & CNT_1==200) flag_p=9;
		 if(flag_p==9){
			 PWM_duty(&pwm1, (0.06));
			 Front(0.9,0.9);
			 if(UT1<27) flag_p=10;
		 }
		 
		 if(flag_p==10){
		   PWM_duty(&pwm1, (0.06));
			 Front(1,0.5);
			 if(UT1<14) flag_p=11;
		 }
		 			 
		 if(flag_p==11){
		   PWM_duty(&pwm1, (0.06));
			 Back(0.8,0.8);
			 if(UT1>20) flag_p=12;
		 }
		 
		 if(flag_p==12){
		   PWM_duty(&pwm1, (0.125));
			 Stop();		 			 		  
			}					 
		 
		 CNT++;

		TIM9->SR &= ~1; 							// clear update interrupt flag
		}
	}
	
void USART1_IRQHandler(){
   if(USART1->SR & USART_SR_RXNE){
      
      // Bluetooth
      BT_Data = BT_read();
      BT_write((uint8_t*) "BT sent : ",10);
      BT_write(&BT_Data,1);
      BT_write((uint8_t*) "\r\n",2);
      
      // PC
      printf("NUCLEO got : %c (from BT)\r\n", BT_Data);
      
   }
}

void TIM4_IRQHandler(void){
	if(TIM4->SR & TIM_SR_UIF){ 
		overf = overf + 1;		

		TIM4->SR &= ~1; 
		}
	
	if((TIM4->SR & TIM_SR_CC3IF) != 0){ 
		time1 = TIM4->CCR3;

		TIM4->SR &= ~(1<<3);				
		}	
	if((TIM4->SR & TIM_SR_CC4IF) != 0){ 
		time2 = TIM4->CCR4;
		//printf("Ut\r\n");
		overf = 0;
		TIM4->SR &= ~(1<<4);	 
		timeInterval = (overf * (TIM4->ARR+1) + time2 - time1);
	}
}

void ADC_IRQHandler(void){
	if((ADC1->SR & ADC_SR_OVR) == ADC_SR_OVR){
	ADC1->SR&=~ADC_SR_OVR;
	}
	
	if(ADC1->SR & ADC_SR_JEOC){       //after finishing sequence
	  //printf("hi ADC\r\n");
		IR1 = ADC1->JDR1;   //read JDR1 value

		IR2 = ADC1->JDR2;   //read JDR2 value 
	
		
	  
	ADC1->SR &= ~(ADC_SR_JEOC);
 }
}