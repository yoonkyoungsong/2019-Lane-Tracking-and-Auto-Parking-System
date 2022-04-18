#include "myInclude.h"
/*
GPIOx_MODER   Output
GPIOx_OTYPER   Output Type: push-pull
GPIOx_OSPEEDR   Medium speed
GPIOx_PUPDR   No Pull-Up, No Pull-Down: 
GPIOx_ODR   Output Data Register
*/

uint32_t direction = 1; 
uint32_t step_delay = 100; 
uint32_t step_per_rev = 64;
//uint32_t motor_type = Unipolar;

int step_number = 0; 

int pin1 = 10;
int pin2 = 4;
int pin3 = 5;
int pin4 = 3;

void Stepper_init(GPIO_TypeDef* port1, int pin1, GPIO_TypeDef* port2, int pin2, GPIO_TypeDef* port3, int pin3, GPIO_TypeDef* port4, int pin4){
   GPIO_init(port1, pin1, WRITE);
   GPIO_ospeed(port1, pin1, MEDIUM);
   
   GPIO_init(port2, pin2, WRITE);
   GPIO_ospeed(port2, pin2, MEDIUM);
   
   GPIO_init(port3, pin3, WRITE);
   GPIO_ospeed(port3, pin3, MEDIUM);
   
   GPIO_init(port4, pin4, WRITE);
   GPIO_ospeed(port4, pin4, MEDIUM);
}


void Stepper_setSpeed(uint32_t speed){
   step_delay = 60*1000/step_per_rev/speed + 0.5
   ; //in milli sec
}

void Stepper_step (uint32_t step){
   
   for(;step>0;step--){
      delay(step_delay);
      if(direction) step_number++;
      else step_number--;
      
      Stepper_stepMotor(step_number%4);
   }
   
      
}
void Stepper_stepMotor (uint32_t sequence){
   switch(sequence){
      case 0 : //1001
         GPIO_write(GPIOB, pin1, 1);
         GPIO_write(GPIOB, pin2, 0);
         GPIO_write(GPIOB, pin3, 0);
         GPIO_write(GPIOB, pin4, 1);
      break;
      
      case 1 : //1010
         GPIO_write(GPIOB, pin1, 1);
         GPIO_write(GPIOB, pin2, 0);
         GPIO_write(GPIOB, pin3, 1);
         GPIO_write(GPIOB, pin4, 0);
      break;
      
      case 2 : //0110
         GPIO_write(GPIOB, pin1, 0);
         GPIO_write(GPIOB, pin2, 1);
         GPIO_write(GPIOB, pin3, 1);
         GPIO_write(GPIOB, pin4, 0);
      break;
      
      case 3 : //0101
         GPIO_write(GPIOB, pin1, 0);
         GPIO_write(GPIOB, pin2, 1);
         GPIO_write(GPIOB, pin3, 0);
         GPIO_write(GPIOB, pin4, 1);
      break;

   }
}