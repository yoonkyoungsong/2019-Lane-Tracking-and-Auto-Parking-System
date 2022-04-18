#include "stm32f4xx.h"
#include "stm32f411xe.h"

#ifndef	PROJECT_HEADER
#define	PROJECT_HEADER

	void Set_TIM9();
	void Set_ADC();
	void Set_Inputcapture();
	void Set_Servomotor();
	void Set_DCmotor();
	
	void Front(float leftSpeed, float rightSpeed);
	void Back(float leftSpeed, float rightSpeed);	
	void Stop();	
	void Front_speed(float Speed1,float Speed2,float Speed3, float Speed4);

#endif

