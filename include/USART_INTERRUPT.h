#ifndef USART_INTERRUPT_H
#define USART_INTERRUPT_H
#include <avr/io.h>
#include <avr/interrupt.h>

#define USART_BAUD 57600
#define MYUBRR F_CPU/16/USART_BAUD-1
#define BUFFER_SIZE 64

// increment index and wrap around if necessary
int NextIndex(int index);

//return 1 if buffer is empty, else return 0
char TxBufferEmpty();

// return 1 if buffer is full, else return 0
char TxBufferFull();

//return 1 if successful, else return 0
char TxBufferAddChar(char c);


char TxBufferReadChar(char *c);


char TxPrintString(char *str);

// initialize USART0
void USART_interrupt_init( unsigned int ubrr);



#endif
