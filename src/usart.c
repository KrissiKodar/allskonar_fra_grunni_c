#include "USART.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void USART_send_char(unsigned char data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}


void USART_send_string(unsigned char *str)
{
	while (*str != '\0')
	{
		USART_send_char(*str);
		str++;
	}
}

// initialize USART0
void USART_init( unsigned int ubrr)
{
	/*Set USART_BAUD rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 bit character size (no parity, 1 stop bit, is default)*/
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

}

// function should waits for a byte to be received and returns it
unsigned char USART_receive(void)
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}


void USART_Transmit_Nibble_Hex(unsigned char data)
{
    unsigned char c;
    c = ' ';
    data &= 0x0F; // mask off upper nibble
    if ((data >= 0) && (data <= 9))
    {
        c = '0' + data;
    }
    else if ((data >= 10) && (data <= 15))
    {
        c = 'A' + (data - 10);
    }
	USART_send_char(c);
}

void USART_Transmit_Byte_Hex(unsigned char data)
{
	USART_Transmit_Nibble_Hex(data >> 4);
	USART_Transmit_Nibble_Hex(data & 0x0F);
}

void USART_Transmit_Int_Hex(unsigned int data)
{
	USART_Transmit_Byte_Hex(data >> 8);
	USART_Transmit_Byte_Hex(data & 0x00FF);
}

void USART_Transmit_Long_Hex(unsigned long data)
{
	USART_Transmit_Int_Hex(data >> 16);
	USART_Transmit_Int_Hex(data & 0x0000FFFF);
}