#ifndef	DEFINE_HEADER
#define	DEFINE_HEADER

#define	RESET  		0UL

//---------- GPIO ---------//

// GPIO_Mode
#define	READ  		0UL
#define	WRITE 		1UL
#define	INPUT  		0UL
#define	OUTPUT 		1UL
#define	ALTE	 		2UL
#define	ANALOG		3UL
#define	AFL				0
#define	AFH				1

// GPIO_Otype
#define	PUSH			0UL
#define	OPEN			1UL

// GPIO_Ospeed
#define	LOW				0UL
#define	MEDIUM		1UL
#define	FAST			2UL
#define	HIGH			3UL

// GPIO_PUPD
#define	NOPUPD		0UL
#define	PUP				1UL
#define	PDOWN			2UL
#define	RESERVED	3UL

//---------- EXTI ----------//

// EXTI_FTSR
#define	DISABLE		0UL
#define RISE			1UL
#define	FALL			2UL

//---------- ADC -----------//

#define POL       0UL
#define INT       1UL
#define EXT       2UL

#endif