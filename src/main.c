#include <avr/io.h>
#include <avr/interrupt.h>

#include "LED.h"
#include "DELAY.h"
#include "I2C.h"
#include "USART.h"


int main(void)
{
	LED_INIT;
	
	//I2C_init();

	char test[] = "Hello World!\n\r";
	USART_init(MYUBRR);
	asm("sei");

	while (1)
	{
		LED_ON;
		LED_OFF;
		TxPrintString(test); // check for error?
		delay_milliseconds(1000);
	}
	return 0;
}