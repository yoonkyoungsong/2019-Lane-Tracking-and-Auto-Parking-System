#include	"myInclude.h"

//GPIO
void	GPIO_init		(GPIO_TypeDef *port, int pin, int mode){
	int value;
	if(port == GPIOA) { //change value as the GPIO port
		value = 0UL;
	} else if(port == GPIOB) {
		value = 1UL;
	} else if (port == GPIOC) {
		value = 2UL;
	}
	
	RCC->AHB1ENR |= (0x1UL << value); //Enable AHB1*/
	GPIO_mode		(port, pin, mode);
	GPIO_otype	(port, pin, RESET);
	GPIO_ospeed	(port, pin, RESET);
	GPIO_pupd		(port, pin, PUP);
	
}


/* GPIO_Mode
	READ			= 0UL
	WRITE			= 1UL
	Alternate	=	2UL
	Analog		= 3UL
*/
void	GPIO_mode		(GPIO_TypeDef *port, int pin, int mode) {
	port->MODER &= ~(3<<pin*2);
	port->MODER |=	(mode<<pin*2);
	

}

/* GPIO_Otype
	Push-pull		= 0UL
	Open-drain	= 1UL
*/
void	GPIO_otype	(GPIO_TypeDef *port, int pin, int type) {
	port->OTYPER &=	~(1<<pin);
	port->OTYPER |=	 (type<<pin);
}

/* GPIO_Ospeed
	Low			= 0UL
	Medium	= 1UL
	Fast		= 2UL
	High		= 3UL
*/
void	GPIO_ospeed	(GPIO_TypeDef *port, int pin, int speed) {
	port->OSPEEDR &=	~(3<<pin*2);
	port->OSPEEDR |=	 (speed<<pin*2);
}

/* GPIO_PUPD
	No PUPD		=	0UL
	Pull-Up		=	1UL
	Pull-Down	=	2UL
	Reserved	= 3UL
*/
void	GPIO_pupd	(GPIO_TypeDef *port, int pin, int pupd) {
	port->PUPDR &= ~(3<<pin*2);
	port->PUPDR |=	(pupd<<pin*2);
}

void	GPIO_write	(GPIO_TypeDef *port, int pin, int output) {
	port->ODR	&=	~(1<<pin);
	port->ODR |=	 (output<<pin);
}

int	GPIO_read	(GPIO_TypeDef *port, int pin) {
	int input = (port->IDR) & (1<<pin);
	
	return input;
}

void LED_toggle	() {
	GPIOA->ODR ^= 1UL << 5;
}


//SevenSement
void	SevenSegment_init	() {
	GPIO_init(GPIOA, 5, WRITE); // a
	GPIO_init(GPIOA, 6, WRITE); // b
	GPIO_init(GPIOA, 7, WRITE); // c
	GPIO_init(GPIOB, 6, WRITE); // d
	GPIO_init(GPIOC, 7, WRITE); // e
	GPIO_init(GPIOA, 9, WRITE); // f
	GPIO_init(GPIOA, 8, WRITE);	 // g
	GPIO_init(GPIOB, 10, WRITE); // dp
}

void	SevenSegment_on	(uint8_t num) {
	int ON;
	if(num == 0) {
		ON = 0b00000011;
	}
	else if(num == 1) {
		ON = 0b11001111;
	}
	else if(num == 2) {
		ON = 0b00100101;
	}
	else if(num == 3) {
		ON = 0b10000101;
	}
	else if(num == 4) {
		ON = 0b11001001;
	}
	else if(num == 5) {
		ON = 0b10010001;
	}
	else if(num == 6) {
		ON = 0b00010001;
	}
	else if(num == 7) {
		ON = 0b11000011;
	}
	else if(num == 8) {
		ON = 0b00000001;
	}
	else if(num == 9) {
		ON = 0b10000001;
	}
	
	GPIO_write(GPIOA, 5, (ON >> 7) & 1);
  GPIO_write(GPIOA, 6, (ON >> 6) & 1);
  GPIO_write(GPIOA, 7, (ON >> 5) & 1);
  GPIO_write(GPIOB, 6, (ON >> 4) & 1);
  GPIO_write(GPIOC, 7, (ON >> 3) & 1);
  GPIO_write(GPIOA, 9, (ON >> 2) & 1);
  GPIO_write(GPIOA, 8, (ON >> 1) & 1);
  GPIO_write(GPIOB, 10, ON & 1);
}


/*------- Test1 ---------*/

void	GPIO_config	(GPIO_TypeDef	*port, uint32_t pin, GPIO_init_t GPIO_seting) {
	int value;
	if(port == GPIOA) { //change value as the GPIO port
		value = 0UL;
	} else if(port == GPIOB) {
		value = 1UL;
	} else if (port == GPIOC) {
		value = 2UL;
	}
	
	RCC->AHB1ENR |= (0x1UL << value); //Enable AHB1
	
	port->MODER &= ~(3<<pin*2);
	port->MODER |=	(GPIO_seting.mode<<pin*2);
	
	port->OTYPER &=	~(1<<pin);
	port->OTYPER |=	 (GPIO_seting.omode<<pin);
	
	port->PUPDR &= ~(3<<pin*2);
	port->PUPDR |=	(GPIO_seting.pudr<<pin*2);
	
	port->OSPEEDR &=	~(3<<pin*2);
	port->OSPEEDR |=	 (GPIO_seting.ospeed<<pin*2);
}