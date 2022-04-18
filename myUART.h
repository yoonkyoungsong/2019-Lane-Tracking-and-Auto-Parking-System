#include "stm32f411xe.h"
#include <stdio.h>
#include "myGPIO.h"

#ifndef __MY_UART_H
#define	__MY_UART_H

#define POL 0
#define INT 1
#define BAUD_9600    (int)9600 
#define BAUD_38400   (int)38400
#define BAUD_57600   (int)57600
#define BAUD_115200  (int)115200

void UART2_init(int baud_rate, int mode);
void UART2_GPIO_init(void);
void UART1_init(int baud_rate, int mode);
void UART1_GPIO_init(void);
void USART_init (USART_TypeDef * USARTx, int baud_rate, int mode);
void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_read (USART_TypeDef * USARTx);
void USART_delay(uint32_t us);
void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter);
void BT_init(uint32_t baud_rate);
void BT_write(uint8_t *buffer, uint32_t nBytes);
uint8_t BT_read();

#endif

