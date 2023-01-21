#ifndef DELAY_H
#define DELAY_H
#include <avr/io.h>

void delay_timer1(unsigned int period_msec);
void delay_msec(unsigned int period_msec);
void delay_sec(unsigned int period_sec);

#endif
