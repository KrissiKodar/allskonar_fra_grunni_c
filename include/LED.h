#ifndef LED_H
#define LED_H
#include <avr/io.h>

#define LEDDIR DDRB
#define LEDPORT PORTB
#define LEDPIN (1<<PB5)

#define LED_INIT LEDDIR |= LEDPIN
#define LED_ON LEDPORT |= LEDPIN
#define LED_OFF LEDPORT &= ~LEDPIN
#define LED_TOGGLE LEDPORT ^= LEDPIN


void LED_init(void);
void LED_on(void);
void LED_off(void);

#endif
