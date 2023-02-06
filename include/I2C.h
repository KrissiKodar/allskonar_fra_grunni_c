#ifndef I2C_H
#define I2C_H
#include <avr/io.h>
#include <util/delay.h>

#define I2C_ERROR 0
#define I2C_SUCCESS 1

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
uint8_t I2C_status(void);
void I2C_write(uint8_t data);
uint8_t I2C_read_ack(void);
uint8_t I2C_read_nack(void);
uint8_t I2C_ReadRegByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t* cReturn);
uint8_t I2C_WriteRegByte(uint8_t dev_addr, uint8_t reg_addr, uint8_t cValue);

#endif