#include "I2C.h"
#include "USART.h"
//#include "DELAY.h"
#include <util/delay.h>

void I2C_init(void)
{
	// set SCL to 100 kHz
	TWSR = 0; // no prescaler
	TWBR = ((F_CPU / 100000L) - 16) / 2; // 2 is multiplid by prescaler normally
	// enable TWI
	TWCR = (1 << TWEN);
}

void I2C_start(void)
{
	// send start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// wait for end of transmission
	while ((TWCR & (1 << TWINT)) == 0) // eda (!(TWCR & (1 << TWINT)))
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
	while ((TWCR & (1 << TWINT)) == 0) // eda (!(TWCR & (1 << TWINT)))
		;
	// wait for ACK or NACK
	// gerist sjalfkrafa (TWINT flag set)
}

uint8_t I2C_read_ack(void)
{
	// read data
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	// wait for end of transmission
	while ((TWCR & (1 << TWINT)) == 0) // eda (!(TWCR & (1 << TWINT)))
		;
	// return data
	return TWDR;
}

uint8_t I2C_read_nack(void)
{
	// read data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for end of transmission
	while ((TWCR & (1 << TWINT)) == 0) // eda (!(TWCR & (1 << TWINT)))
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

	//delay_milliseconds(100); // 100 ms delay
	_delay_ms(10);
	
	I2C_start();
	if ( (c = I2C_status()) != 0x08) // ekki repeted start (start ekki milli annars start og stop)
		return I2C_ERROR;
	
	// send that master wants to read from slave
	I2C_write((dev_addr) | 0x01); // SLA + R read er 0x01 og write er 0x00
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