#include	"stm32f4xx.h"
#include	"stm32f411xe.h"

#ifndef	GPIO_HEADER
#define	GPIO_HEADER

typedef struct {
	uint32_t	pin;
	uint32_t	mode;
	uint32_t	pudr;
	uint32_t	omode;
	uint32_t	ospeed;
} GPIO_init_t;

//GPIO
void	GPIO_init		(GPIO_TypeDef *port, int pin, int mode);
void	GPIO_mode		(GPIO_TypeDef *port, int pin, int mode);
void	GPIO_otype	(GPIO_TypeDef *port, int pin, int type);
void	GPIO_ospeed	(GPIO_TypeDef *port, int pin, int speed);
void	GPIO_pupd		(GPIO_TypeDef *port, int pin, int pupd);
void	GPIO_write	(GPIO_TypeDef *port, int pin, int output);
int		GPIO_read		(GPIO_TypeDef *port, int pin);

void	LED_toggle	();

//SevenSement
void	SevenSegment_init	();
void	SevenSegment_on		(uint8_t num);

//Test1
void	GPIO_config	(GPIO_TypeDef	*port, uint32_t pin, GPIO_init_t GPIO_seting);
#endif