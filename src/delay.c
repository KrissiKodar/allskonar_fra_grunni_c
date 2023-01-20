#include "DELAY.h"
#include <avr/io.h>

void delay_timer1(unsigned int period_msec)
{
	// set timer1 to CTC (WGM12= 1, WGM10 = 0)
	TCCR1A = 0;
	TCCR1B = (1 << WGM12);

	// initialize counter
	TCNT1 = 0;

	// ath 16000000/1024-1 = 15624
	OCR1A = 15624.0 * period_msec / 1000.0; // maximum value is 65535

	// start timer with /1024 prescaler
	// can delay 0 to 4.194 seconds
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// wait for timer1 to reach the compare value
	while ((TIFR1 & (1 << OCF1A)) == 0)
		;

	// clear timer1 interrupt flag
	TIFR1 = (1 << OCF1A);
}

// function that calls the delay function, multiple times, if the delay is longer than 4.194 seconds


// call this function to delay for a period of time
void delay1(unsigned int period_msec)
{
	// maximum delay is 4194 seconds
	// if the delay is longer than 4194 seconds, call the delay function multiple times
	while (period_msec > 0)
	{
		if (period_msec > 4194)
		{
			delay_timer1(4194);
			period_msec -= 4194;
		}
		else
		{
			delay_timer1(period_msec);
			period_msec = 0;
		}
	}
}