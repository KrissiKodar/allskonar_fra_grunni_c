#include "USART.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int TxBuffer[BUFFER_SIZE]; // ring buffer
int iIn;  // buffer is empty id iIn == iOut
int iOut; // buffer is full next(iIn) == iOut

// increment index and wrap around if necessary
int NextIndex(int index)
{
	index++;
	if (index >= BUFFER_SIZE)
	{
		index = 0;
	}
	return index;
}

//return 1 if buffer is empty, else return 0
char TxBufferEmpty()
{
	return (iIn == iOut);
}

// return 1 if buffer is full, else return 0
char TxBufferFull()
{
	return (NextIndex(iIn) == iOut);
}

//return 1 if successful, else return 0
char TxBufferAddChar(char c)
{
	char retval = 1; //default to success
	if (!TxBufferFull())
	{
		TxBuffer[iIn] = c;
		iIn = NextIndex(iIn); //increment iIn and wrap around if necessary
	}
	else retval = 0; //buffer is full
	return retval;
}


char TxBufferReadChar(char *c)
{
	char retval = 1; //default to success
	if (!TxBufferEmpty())
	{
		*c = TxBuffer[iOut];
		iOut = NextIndex(iOut); //increment iOut and wrap around if necessary
	}
	else retval = 0; //buffer is empty
	return retval;
}

char TxPrintString(char *str)
{
	char cSuccess = 1; //default to success
	char c;
	while(*str != '\0')
	{
		if(TxBufferAddChar(*str))
		; // success, do nothing
		else 
		{
			cSuccess = 0; // failure
			break;
		}
		str++; // point to next character
	}
}

// initialize USART0
void USART_init( unsigned int ubrr)
{
	/*Set USART_BAUD rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<TXCIE0);
	/* Set frame format: 8 bit character size (no parity, 1 stop bit, is default)*/
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	iIn = 0;
	iOut = 0;
}

ISR (USART_TX_vect)
{
	char c;
	if (TxBufferReadChar(&c)) // if buffer not empty
	{
		UDR0 = c; // send next character to USART
	}
}

// klara seinna
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
}