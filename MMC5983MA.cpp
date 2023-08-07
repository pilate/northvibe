#include <Wire.h>

#include "MMC5983MA.h"
#include "mi2c.h"
#include "TinyNap.h"


uint8_t MMC5983MA::status() {
  return I2C_ReadByte(MMC5983MA_ADDRESS, MMC5983MA_STATUS);
}

void MMC5983MA::SET() {
  I2C_Write(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, MMC5983MA_CONTROL_0_SET);
}

void MMC5983MA::RESET() {
  I2C_Write(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, MMC5983MA_CONTROL_0_RESET);
}

void MMC5983MA::read() {
  char message[100];

  I2C_Write(MMC5983MA_ADDRESS, MMC5983MA_CONTROL_0, MMC5983MA_CONTROL_0_TM_M);

  // put atting into standby for minimum time instead of polling
  nap(16);

  // uint8_t curStatus;
  // while (true) {
  //   curStatus = status();
  //   if (curStatus & 1) {
  //     break;
  //   }
  //   delay(10);
  // }

  I2C_StartRead(MMC5983MA_ADDRESS, 0, 7);
  for (uint8_t i = 0; i < 7; i++) {
    rawData[i] = (uint8_t)Wire.read();
  }

  x = ((int32_t)rawData[0] << 10) | ((int32_t)rawData[1] << 2);
  x |= rawData[6] >> 6;
  x -= 1 << 17;

  y = ((int32_t)rawData[2] << 10) | ((int32_t)rawData[3] << 2);
  y |= (rawData[6] >> 4) | 0b11;
  y -= 1 << 17;

  z = ((int32_t)rawData[4] << 10) | ((int32_t)rawData[5] << 2);
  z |= (rawData[6] >> 2) | 0b11;
  z -= 1 << 17;
}

uint8_t MMC5983MA::getProductID() {
  return I2C_ReadByte(MMC5983MA_ADDRESS, MMC5983MA_PRODUCT_ID);
}