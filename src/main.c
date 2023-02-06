#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LED.h"
//#include "DELAY.h"
//#include "I2C.h"
#include "USART.h"




//8-bit timer 2 used for 2 PWM channels
// Port B, pin 3 is OC2A, D11 on Arduino
// Port D, pin 3 is OC2B, D3 on Arduino
void timer2_pwm_init(void)
{
	DDRD |= (1 << PD3); // set D3 as output
	DDRB |= (1 << PB3); // set D11 as output
	TCCR2A = 0b10100011;
	TCCR2B = 0b00000001; // these to set it to fast PWM mode, with TOP = 0xFF, update OCR2A on BOTTOM, TOv flag set on MAX, prescaler = 1
}

void set_D11_duty_cycle(unsigned char duty_cycle)
{
	OCR2A = duty_cycle;
}

void set_D3_duty_cycle(unsigned char duty_cycle)
{
	OCR2B = duty_cycle;
}

// convert incoming ascii number to actual number
unsigned char ascii_to_number(char c)
{
	return c - '0';
}

// function that waits for a newline, then returns the number
unsigned char get_unsigned_char_from_user(void)
{
	unsigned char c;
	unsigned char number = 0;
	while (1)
	{
		c = USART_receive();
		if (c == '\r')
		{
			return number;
		}
		else
		{
			number = number * 10 + ascii_to_number(c);
		}
	}
}
int main(void)
{	
	unsigned char c;
	LED_INIT;
	asm("sei");
	USART_init(MYUBRR);
	timer2_pwm_init();


	while (1)
	{	
		USART_send_string("Input duty cycle (0-255): \r\n");
		c = 0;
		_delay_ms(100);
		c = get_unsigned_char_from_user();
		USART_send_string("Received: ");
		USART_Transmit_Byte_Hex(c);
		set_D11_duty_cycle(c);
		USART_send_string("\r\n");
	}
	return 0;
}