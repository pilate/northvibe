#include "LSM6DSO.h"


uint8_t LSM6DSO::WhoAmI() {
  return I2C_ReadByte(LSM6DSO_ADDRESS, LSM6DSO_WHO_AM_I);
}

void LSM6DSO::I2CPassthrough() {
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_FUNC_CFG_ACCESS, bit(6));  // enable sensor hub access
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_SENSOR_HUB_MASTER_CONFIG, bit(4));    // enable pass-through
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_FUNC_CFG_ACCESS, 0);       // disable sensor hub access
}

void LSM6DSO::read() {
  // Turn ultra-low-power mode on
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL5_C, bit(LSM6DSO_CTRL5_C_XL_ULP_EN));

  // Turn accel high-performance mode OFF
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL6_C, bit(LSM6DSO_CTRL6_C_XL_HM_MODE));

  // Turn accelerometer on
  // b2 = 1.6hz ODR, 2g, second stage filtering
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL1_XL, 0xb2);
}