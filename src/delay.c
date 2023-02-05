#include "DELAY.h"
#include <avr/io.h>

/*  */

void delay_seconds (unsigned int period_sec)
{
	volatile unsigned long ul_Max, ul_Counter;
	ul_Max = 380000 * period_sec;
	while ( ul_Counter != ul_Max )
	{
		ul_Counter++;
	}
}

void delay_milliseconds (unsigned int period_msec)
{
	volatile unsigned long ul_Max, ul_Counter;
	ul_Max = 380 * period_msec;
	while ( ul_Counter != ul_Max )
	{
		ul_Counter++;
	}
}

void delay_microseconds (unsigned char __micro_delay)
{
	__asm__ volatile (
		"nop" "\n\t" // 1 cycle
		"nop" "\n\t" // 1 cycle
		"nop" "\n\t" // 1 cycle
		"1: dec %0" "\n\t" // 1 cycle
		"brne 1b" // (1 cycle if false, 2 cycles if true)
		: "=r" (__micro_delay) 
		: "0" (__micro_delay) 
	);
}


/* void delay_timer1(unsigned int period_msec) // fix later
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
void delay_msec(unsigned int period_msec) // fix later
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

void delay_sec(unsigned int period_sec) // fix later
{
	delay_msec(period_sec * 1000);
} */