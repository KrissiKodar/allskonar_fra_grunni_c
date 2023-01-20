#include <avr/io.h>
#include "LED.h"
#include "DELAY.h"

#define FOSC 16000000 // Clock Speed
#define BAUD 57600
#define MYUBRR FOSC/16/BAUD-1

// initialize USART0
void USART_init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data, no parity, 1 stop bit (is default)*/
}


// send single character (unsigned char)
void USART_send_char(unsigned char data)
{
	// wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
		;
	// put data into buffer, sends the data
	UDR0 = data;
}

// USART_string
void USART_send_string(char *str)
{
	while (*str != '\0')
	{
		USART_send_char(*str);
		str++;
	}
}

int main(void)
{
	LED_init();
	USART_init(MYUBRR);

	//char string_test[] = "Hello World!\n";
	while (1)
	{
		LED_on();
		USART_send_string("hello from kristjan\n");
		delay1(1000);
		LED_off();
		delay1(1000);
	}
}