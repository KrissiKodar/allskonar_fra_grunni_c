#ifndef USART_H
#define USART_H
#include <avr/io.h>
#include <avr/interrupt.h>

#define USART_BAUD 57600
#define MYUBRR F_CPU/16/USART_BAUD-1


void USART_send_char(unsigned char data);

void USART_send_string(unsigned char *str);

void USART_init( unsigned int ubrr);

unsigned char USART_receive(void);

void USART_Transmit_Nibble_Hex(unsigned char data);

void USART_Transmit_Byte_Hex(unsigned char data);

void USART_Transmit_Int_Hex(unsigned int data);

void USART_Transmit_Long_Hex(unsigned long data);

// convert incoming ascii number to actual number
unsigned char ascii_to_number(char c);

// function that waits for a newline, then returns the number
unsigned char get_unsigned_char_from_user(void);

#endif
