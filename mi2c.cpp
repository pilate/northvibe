#include <Wire.h>
#include "mi2c.h"


#define SERIAL_I2C 0x11

uint8_t I2C_Write(uint8_t addr, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission();
}

uint8_t I2C_StartRead(uint8_t addr, uint8_t reg, int count) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom((int)addr, (int)count);
}

uint8_t I2C_ReadByte(uint8_t addr, uint8_t reg) {
  I2C_StartRead(addr, reg, 1);
  return (uint8_t)Wire.read();
}

void I2C_LogString(char* message, uint8_t size) {
  Wire.beginTransmission(SERIAL_I2C);
  Wire.write(0x0);
  Wire.write(message, size);
  Wire.endTransmission();
}
