#ifndef DELAY_H
#define DELAY_H
#include <avr/io.h>

// simple delays
// keep the processor busy for a specified number of seconds, milliseconds, or microseconds
// not using external timers
void delay_seconds (unsigned int period_sec);
void delay_milliseconds (unsigned int period_msec);
void delay_microseconds (unsigned char __micro_delay);



// using external timers below

//void delay_timer1(unsigned int period_msec);
//void delay_msec(unsigned int period_msec);
//void delay_sec(unsigned int period_sec);

#endif
