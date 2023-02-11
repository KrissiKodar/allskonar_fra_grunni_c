#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LED.h"

//#include "DELAY.h"
#include "I2C.h"

#include "USART.h"


//8-bit timer 2 used for 2 PWM channels
// Port B, pin 3 is OC2A, D11 on Arduino
// Port D, pin 3 is OC2B, D3 on Arduino
void timer2_pwm_init(void)
{
	DDRD |= (1 << PD3); // set D3 as output
	DDRB |= (1 << PB3); // set D11 as output
	 // these to set it to fast PWM mode, with TOP = 0xFF, update OCR2A on BOTTOM, TOV flag set on MAX
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS20);// these to set it to fast PWM mode, with TOP = 0xFF, update OCR2A on BOTTOM, TOV flag set on MAX, prescaler = 1
}

void set_D11_duty_cycle(unsigned char duty_cycle)
{
	OCR2A = duty_cycle;
}

void set_D3_duty_cycle(unsigned char duty_cycle)
{
	OCR2B = duty_cycle;
}



int main(void)
{	
	unsigned char c;
	LED_INIT;
	asm("sei");
	USART_init(MYUBRR);
	I2C_init();
	//timer2_pwm_init();
	//timer_2_CTC_init();
	//USART_send_string("LETS GO: \r\n");
	_delay_ms(100);
	if (I2C_ReadRegByte(0x1E, 0x0A, &c) == I2C_ERROR)
	{
		LED_ON;
		I2C_stop();
		USART_send_char('e');
	}

	USART_send_string("Success! \r\n");
	USART_send_string("Read from register: ");
	USART_Transmit_Byte_Hex(c);


	return 0;
}