#ifndef MI2C_h
#define MI2C_h

#include <Arduino.h>

uint8_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t value);
uint8_t I2C_StartRead(uint8_t addr, uint8_t reg, int count);
uint8_t I2C_ReadByte(uint8_t addr, uint8_t reg);

void I2C_LogString(char *message, uint8_t size);

#endif