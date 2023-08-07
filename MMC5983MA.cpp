#include <Wire.h>

#include "MMC5983MA.h"
#include "mi2c.h"



#define SERIAL_I2C 0x11

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

  uint8_t curStatus;
  while (true) {
    curStatus = status();

    memset(message, 0x00, 100);
    sprintf(message, "mmc status: %x\r\n", curStatus);
    I2C_LogString(message, 100);

    if (curStatus & 1) {
      break;
    }

    delay(100);
  }

  I2C_StartRead(MMC5983MA_ADDRESS, 0, 7);
  for (uint8_t i = 0; i < 7; i++) {
    rawData[i] = (uint8_t)Wire.read();
  }

  for (uint8_t i = 0; i < 7; i++) {
    memset(message, 0x00, 100);
    sprintf(message, "mmc read[%d]: %x\r\n\x00", i, rawData[i]);
    I2C_LogString(message, 100);
  }
}

uint8_t MMC5983MA::getProductID() {
  return I2C_ReadByte(MMC5983MA_ADDRESS, MMC5983MA_PRODUCT_ID);
}