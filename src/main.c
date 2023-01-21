#include <avr/io.h>
#include "LED.h"
#include "DELAY.h"

#define BAUD 57600
#define MYUBRR F_CPU/16/BAUD-1
#define I2C_ERROR 0xFF
#define I2C_SUCCESS 0x00

// initialize USART0
void USART_init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data, no parity, 1 stop bit (is default)*/
}

// send single character (unsigned char)
void USART_send_char(unsigned char data)
{
	// wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
		;
	// put data into buffer, sends the data
	UDR0 = data;
}

// USART_string
void USART_send_string(char *str)
{
	while (*str != '\0')
	{
		USART_send_char(*str);
		str++;
	}
}


void I2C_init(void)
{
	// set SCL to 400 kHz
	TWSR = 0; // no prescaler
	TWBR = ((F_CPU / 400000L) - 16) / 2; // 2 is multiplid by prescaler normally
	// enable TWI
	TWCR = (1 << TWEN);
}

void I2C_start(void)
{
	// send start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
}

void I2C_stop(void)
{
	// transmit stop condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	// wait for end of transmission
	while (!(TWCR & (1 << TWSTO)))
		;
}

uint8_t I2C_status(void)
{
	uint8_t status;
	// mask status
	status = TWSR & 0xF8;
	return status;
}

void I2C_write(uint8_t data)
{
	// send data 
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// wait for ACK or NACK
	// gerist sjalfkrafa (TWINT flag set)
}

uint8_t I2C_read_ack(void)
{
	// read data
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return data
	return TWDR;
}

uint8_t I2C_read_nack(void)
{
	// read data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while (!(TWCR & (1 << TWINT)))
		;
	// return data
	return TWDR;
}

uint8_t I2C_ReadRegByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t* cReturn)
{
	uint8_t c;

	I2C_start();
	// check if start was sent
	if ( (c = I2C_status()) != 0x08)
		return I2C_ERROR;

	// pick device (slave) to write to (talk to)
	I2C_write(dev_addr);
	// check for ACK (acknowledge), so we know that the device 
	// knows it is being talked to
	if ( (c = I2C_status()) != 0x18)
		return I2C_ERROR;

	// pick register in slave
	I2C_write(reg_addr);
	if ( (c = I2C_status()) != 0x28)
		return I2C_ERROR;
	I2C_stop();

	delay_msec(100); // 100 ms delay

	I2C_start();
	if ( (c = I2C_status()) != 0x08) // ekki repeted start (start ekki milli annars start og stop)
		return I2C_ERROR;
	
	// send that master wants to read from slave
	I2C_write(dev_addr | 0x01); // SLA + R read er 0x01 og write er 0x00
	if ( (c = I2C_status()) != 0x40) // ath e SLA+R
		return I2C_ERROR;

	// read the data from the register in the slave
	c = I2C_read_nack();
	if ( (I2C_status()) != 0x58)
		return I2C_ERROR;
	I2C_stop();

	*cReturn = c;
	return I2C_SUCCESS;
}

uint8_t I2C_WriteRegByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t cValue)
{
	char c;
	I2C_start();
	// check if start was sent
	if ( (c = I2C_status()) != 0x08)
		return I2C_ERROR;
	
	I2C_write(dev_addr);
	// check for ACK (acknowledge), so we know that the device
	// knows it is being talked to
	if ( (c = I2C_status()) != 0x18)
		return I2C_ERROR;
	
	I2C_write(reg_addr);
	if ( (c = I2C_status()) != 0x28)
		return I2C_ERROR;
	
	I2C_write(cValue);
	if ( (c = I2C_status()) != 0x28)
		return I2C_ERROR;
	I2C_stop();
	return I2C_SUCCESS;
}



int main(void)
{
	LED_init();
	USART_init(MYUBRR);

	//char string_test[] = "Hello World!\n";
	while (1)
	{
		LED_on();
		//USART_send_string("hello from kristjan!!!!\n");
		delay_sec(1);
		LED_off();
		delay_sec(1);
	}

	uint8_t c; //??????

	if (I2C_ReadRegByte(0x68, 0x75, &c) == I2C_SUCCESS)
	{
		USART_send_string("success\n");
	}
	else
	{
		USART_send_string("error\n");
	}
}