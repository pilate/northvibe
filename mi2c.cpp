#include <Wire.h>
#include "mi2c.h"

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
