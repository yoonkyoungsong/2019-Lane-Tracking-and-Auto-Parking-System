#include "myUART.h"
#include <stdio.h>
#include <math.h>

// Implement a dummy __FILE struct, which is called with the FILE structure.
struct __FILE {
    int dummy;
};

FILE __stdout;
FILE __stdin;

// Retarget printf() to USART2
int fputc(int ch, FILE *f) { 
  uint8_t c;
  c = ch & 0x00FF;
  USART_write(USART2, (uint8_t *)&c, 1);
  return(ch);
}

// Retarget getchar()/scanf() to USART2
int fgetc(FILE *f) {  
  uint8_t rxByte;
  rxByte = USART_read(USART2);
  return rxByte;
}


void UART2_init(int baud_rate, int mode){
   // Enable the clock of USART2
   RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART 2 clock (APB1 clock: AHB clock / 2 = 42MHz)
   
   UART2_GPIO_init();
   USART_init(USART2, baud_rate, mode);
   
   if( mode == INT){
      NVIC_SetPriority(USART2_IRQn, 1);      // Set Priority to 1
      NVIC_EnableIRQ(USART2_IRQn);             // Enable interrupt of USART2 peripheral
   }
}

void UART1_init(int baud_rate, int mode){
   // Enable the clock of USART2
   RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  // Enable USART 2 clock (APB2 clock: AHB clock = 84MHz)
   
   UART1_GPIO_init();
   USART_init(USART1, baud_rate, mode);
   
   if( mode == INT){
      NVIC_SetPriority(USART1_IRQn, 7);      // Set Priority to 1
      NVIC_EnableIRQ(USART1_IRQn);             // Enable interrupt of USART2 peripheral
   }
}


void UART2_GPIO_init(void){
   
   // Enable the peripheral clock of GPIO Port
   RCC->AHB1ENR |=   RCC_AHB1ENR_GPIOAEN;   
   
   // ********************** USART 2 ***************************
   // PA2 = USART2_TX
   // PA3 = USART2_RX
   // Alternate function(AF7), High Speed, Push pull, Pull up
   // **********************************************************
   int TX_pin = 2;
   
   GPIOA->MODER   &= ~(0xF << (2*TX_pin));   // Clear bits
   GPIOA->MODER   |=   0xA << (2*TX_pin);  // Alternate Function(10)          
   
   GPIOA->AFR[0]  |=   0x77<< (4*TX_pin);  // AF7 - USART2        
   
   GPIOA->OSPEEDR |=   0xF<<(2*TX_pin);       // High speed (11)             
      
   GPIOA->PUPDR   &= ~(0xF<<(2*TX_pin));
   GPIOA->PUPDR   |=   0x5<<(2*TX_pin);    // Pull-up (01)         
   
   GPIOA->OTYPER  &=  ~(0x3<<TX_pin) ;       // push-pull (0, reset)
}

void UART1_GPIO_init(void){
   
   // Enable the peripheral clock of GPIO Port
   RCC->AHB1ENR |=   RCC_AHB1ENR_GPIOBEN;   
   
   // ********************** USART 1 ***************************
   // PA9,PB6 = USART1_TX
   // PA10,PB3 = USART1_RX
   // Alternate function(AF7), High Speed, Push pull, Pull up
   // **********************************************************
//   int TX_pin = 9;
//   
//   GPIOA->MODER   &= ~(0xF << (2*TX_pin));   // Clear bits
//   GPIOA->MODER   |=   0xA << (2*TX_pin);  // Alternate Function(10)          
//   
//   GPIOA->AFR[1]  |=   0x77<< (4*(TX_pin-8));  // AF7 - USART1        
//   
//   GPIOA->OSPEEDR |=   0xF<<(2*TX_pin);       // High speed (11)             
//      
//   GPIOA->PUPDR   &= ~(0xF<<(2*TX_pin));
//   GPIOA->PUPDR   |=   0x5<<(2*TX_pin);    // Pull-up (01)         
//   
//   GPIOA->OTYPER  &=  ~(0x3<<TX_pin) ;       // push-pull (0, reset)
	 
	    int TX_pin = 6;
			int RX_pin = 3;
   
   GPIOB->MODER   &=   ~(3 << TX_pin);   // Clear bits
   GPIOB->MODER   |=    2 << TX_pin;  // Alternate Function(10)         
  
   GPIOB->MODER   &=   ~(3 << RX_pin);   // Clear bits
   GPIOB->MODER   |=    2 << RX_pin;  // Alternate Function(10)   	 
   
   GPIOB->AFR[0]  |=   7<< (4*TX_pin);  // AF7 - USART1  
   GPIOB->AFR[0]  |=   7<< (4*RX_pin);  // AF7 - USART1 	 
   
	 GPIOB->OSPEEDR &=   ~(3<<TX_pin);
   GPIOB->OSPEEDR |=   3 <<TX_pin;       // High speed (11)    

	 GPIOB->OSPEEDR &=   ~(3<<RX_pin);
   GPIOB->OSPEEDR |=   3 <<RX_pin;       // High speed (11) 
      
   GPIOB->PUPDR   &=    ~(3<<TX_pin);
   GPIOB->PUPDR   |=     1<<TX_pin;    // Pull-up (01)      

   GPIOB->PUPDR   &=    ~(3<<RX_pin);
   GPIOB->PUPDR   |=     1<<RX_pin;    // Pull-up (01)  
   
   GPIOB->OTYPER  &=   ~(1<<TX_pin) ;       // push-pull (0, reset)
	 GPIOB->OTYPER  &=   ~(1<<RX_pin) ;       // push-pull (0, reset)
	 
}

void USART_init (USART_TypeDef * USARTx, int baud_rate, int mode){ 
   // No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit      
   USARTx->CR1 &= ~USART_CR1_UE;  // Disable USART
   
   // Configure word length to 8 bit
   USARTx->CR1 &= ~USART_CR1_M;       // M: 0 = 8 data bits, 1 start bit   
   USARTx->CR1 &= ~USART_CR1_PCE;   // No parrity bit 
   USARTx->CR2 &= ~USART_CR2_STOP;  // 1 stop bit    
   
   // Configure oversampling mode (to reduce RF noise)
   USARTx->CR1 &= ~USART_CR1_OVER8;  // 0 = oversampling by 16   
                                 
   // CSet Baudrate to 9600 using APB frequency (42MHz)
   // If oversampling by 16, Tx/Rx baud = f_CK / (16*USARTDIV),  
   // If oversampling by 8,  Tx/Rx baud = f_CK / (8*USARTDIV)
   // USARTDIV = 42MHz/(16*9600) = 237.4375
   
   //USARTx->BRR  = 42000000/ baud_rate;
   float Hz;
   if(USARTx==USART2){
       Hz=42000000;
    }else if(USARTx==USART1){
       Hz=84000000;
    }
    
   float USARTDIV = (float)Hz/16/baud_rate;
   uint32_t MNT = (uint32_t)USARTDIV;
   uint32_t FRC = round((USARTDIV - MNT) * 16);
   if (FRC > 15) {
      MNT += 1;
      FRC = 0;
   }
   USARTx->BRR  |= (MNT << 4) | FRC;
   
   USARTx->CR1  |= (USART_CR1_RE | USART_CR1_TE);   // Transmitter and Receiver enable

    
  if (mode == INT){   
      USARTx->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
      USARTx->CR1 |= USART_CR1_RXNEIE;                       // Received Data Ready to be Read Interrupt
   }
   if (mode == POL){      
      USARTx->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
   }
   
   USARTx->CR1  |= USART_CR1_UE; // USART enable
}


void USART_write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
   int i;
   // TXE is cleared by a write to the USART_DR register.
   // TXE is set by hardware when the content of the TDR 
   // register has been transferred into the shift register.
   for (i = 0; i < nBytes; i++) {
      while (!(USARTx->SR & USART_SR_TXE));  // wait until TXE (TX empty) bit is set
      // Writing USART_DR automatically clears the TXE flag    
      USARTx->DR = buffer[i] & 0xFF;
      USART_delay(300);
   }
   while (!(USARTx->SR & USART_SR_TC));      // wait until TC bit is set
   USARTx->SR &= ~USART_SR_TC;                     // TC bit clear
}  

uint8_t USART_read (USART_TypeDef * USARTx) {
   // SR_RXNE (Read data register not empty) bit is set by hardware
   while (!(USARTx->SR & USART_SR_RXNE));  // Wait until RXNE (RX not empty) bit is set
   // USART resets the RXNE flag automatically after reading DR
   return ((uint8_t)(USARTx->DR & 0xFF));
   // Reading USART_DR automatically clears the RXNE flag 
}


void USART_delay(uint32_t us) {
   uint32_t time = 100*us/7;    
   while(--time);   
}


void BT_init(uint32_t baud_rate){
   
	UART1_init(baud_rate,INT);
	
}

uint8_t BT_read(){
  
	return USART_read(USART1);
	
}

void BT_write(uint8_t *buffer,uint32_t nBytes){
	
   //USART_write(USART1, (uint8_t*) "Hello Bluetooth\r\n" , nBytes);
    
   int i;
   // TXE is cleared by a write to the USART_DR register.
   // TXE is set by hardware when the content of the TDR 
   // register has been transferred into the shift register.
   for (i = 0; i < nBytes; i++) {
      while (!(USART1->SR & USART_SR_TXE));  // wait until TXE (TX empty) bit is set
      // Writing USART_DR automatically clears the TXE flag    
      USART1->DR = buffer[i] & 0xFF;
      USART_delay(300);
   }
   while (!(USART1->SR & USART_SR_TC));      // wait until TC bit is set
   USART1->SR &= ~USART_SR_TC;                     // TC bit clear
 }