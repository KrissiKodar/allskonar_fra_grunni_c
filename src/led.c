#include "LED.h"
#include <avr/io.h>

void LED_init(void)
{
	DDRB |= (1 << DDB5);
}

void LED_on(void)
{
	PORTB |= (1 << PORTB5);
}

void LED_off(void)
{
	PORTB &= ~(1 << PORTB5);
}