/* #include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
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
	 // these to set it to fast PWM mode, with TOP = 0xFF, update OCR2A on BOTTOM, TOV flag set on MAX
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS20);// these to set it to fast PWM mode, with TOP = 0xFF, update OCR2A on BOTTOM, TOV flag set on MAX, prescaler = 1
}

// timer function that clears timer on overflow
// but triggers interrupt on compare match
void timer_2_CTC_init(void)
{
	TCNT2 = 0x00; // clear timer
	TCCR2A = 0x00; // (normal port operation)
	TCCR2B |= (1 << CS20); // set prescaler to 1
	OCR2A = 0x00; // set duty cycle to 0x00
	TIMSK2 |= (1 << OCIE2A); // enable interrupt on compare match 
	TIMSK2 |= (1 << TOIE2); // enable interrupt on overflow
}

void set_timer_2_CTC_duty_cycle(unsigned char duty_cycle)
{
	OCR2A = duty_cycle;
}

void set_D11_duty_cycle(unsigned char duty_cycle)
{
	OCR2A = duty_cycle;
}

void set_D3_duty_cycle(unsigned char duty_cycle)
{
	OCR2B = duty_cycle;
}

ISR(TIMER2_COMPA_vect)
{
	LED_OFF;
}

ISR(TIMER2_OVF_vect)
{
	LED_ON;
}

int main(void)
{	
	unsigned char c;
	LED_INIT;
	asm("sei");
	USART_init(MYUBRR);
	//timer2_pwm_init();
	timer_2_CTC_init();

	while (1)
	{	
		USART_send_string("Input duty cycle (0-255): \r\n");
		c = 0;
		//_delay_ms(100);
		c = get_unsigned_char_from_user();
		USART_send_string("Received: ");
		USART_Transmit_Byte_Hex(c);
		//set_D11_duty_cycle(c);
		set_timer_2_CTC_duty_cycle(c);
		USART_send_string("\r\n");
	}
	return 0;
} */