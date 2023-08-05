#include "LSM6DSO.h"


uint8_t LSM6DSO::WhoAmI() {
  return I2C_ReadByte(LSM6DSO_ADDRESS, LSM6DSO_WHO_AM_I);
}

void LSM6DSO::I2CPassthrough() {
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_FUNC_CFG_ACCESS, bit(6));  // enable sensor hub access
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_MASTER_CONFIG, bit(4));    // enable pass-through
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_FUNC_CFG_ACCESS, 0);       // disable sensor hub access
}