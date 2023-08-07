#include <Wire.h>

#include "LSM6DSO.h"
#include "mi2c.h"


uint8_t LSM6DSO::WhoAmI() {
  return I2C_ReadByte(LSM6DSO_ADDRESS, LSM6DSO_WHO_AM_I);
}

void LSM6DSO::I2CPassthrough() {
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_FUNC_CFG_ACCESS, bit(6));           // enable sensor hub access
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_SENSOR_HUB_MASTER_CONFIG, bit(4));  // enable pass-through
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_FUNC_CFG_ACCESS, 0);                // disable sensor hub access
}

void LSM6DSO::read() {
  // Turn ultra-low-power mode on
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL5_C, bit(LSM6DSO_CTRL5_C_XL_ULP_EN));

  // Turn accel high-performance mode OFF
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL6_C, bit(LSM6DSO_CTRL6_C_XL_HM_MODE));

  // Turn accelerometer on
  // b2 = 1.6hz ODR, 2g, second stage filtering
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL1_XL, 0b10110010);

  // Turn gyro on
  // 250dps
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL2_G, 0b00010000);

  delay(10);

  I2C_StartRead(LSM6DSO_ADDRESS, LSM6DSO_OUTX_L_G, 12);
  for (uint8_t i = 0; i < 6; i++) {
    gyroRawData[i] = (uint8_t)Wire.read();
  }
  for (uint8_t i = 0; i < 6; i++) {
    accelRawData[i] = (uint8_t)Wire.read();
  }

  g_x = (gyroRawData[1] << 8) | gyroRawData[0];
  g_y = (gyroRawData[3] << 8) | gyroRawData[2];
  g_z = (gyroRawData[5] << 8) | gyroRawData[4];

  a_x = (accelRawData[1] << 8) | accelRawData[0];
  a_y = (accelRawData[3] << 8) | accelRawData[2];
  a_z = (accelRawData[5] << 8) | accelRawData[4];

  // Turn accelerometer off
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL1_XL, 0x0);

  // Turn gyro off
  I2C_Write(LSM6DSO_ADDRESS, LSM6DSO_CTRL2_G, 0x0);
}