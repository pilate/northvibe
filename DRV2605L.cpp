#include "DRV2605L.h"
#include "mi2c.h"
#include "TinyNap.h"


void DRV2605L::init() {
  // auto-calibration mode
  I2C_Write(DRV2605L_ADDRESS, 0x01, 0x07);
  // auto-calibration settings

  // Set feedback control register
  uint8_t FCR;
  FCR |= (2 << 4);  // FB_BRAKE_FACTOR 0x2
  FCR |= (2 << 2);  // LOOP_GAIN 0x2
  I2C_Write(DRV2605L_ADDRESS, 0x1A, FCR);

  uint8_t C2;
  C2 |= (3 << 4);  // SAMPLE_TIME
  C2 |= (1 << 2);  // BLANKING_TIME
  C2 |= 1;         // IDISS_TIME
  I2C_Write(DRV2605L_ADDRESS, 0x1A, C2);

  uint8_t C4;
  C4 |= (3 << 4);  // AUTO_CAL_TIME 0x3
  I2C_Write(DRV2605L_ADDRESS, 0x1A, C4);

  // change to library B
  I2C_Write(DRV2605L_ADDRESS, 0x03, 0x02);

  // Go bit
  I2C_Write(DRV2605L_ADDRESS, 0x0C, 0x01);

  nap(128);

  // standby
  I2C_Write(DRV2605L_ADDRESS, 0x01, 1 << 6);
}

void DRV2605L::effect(uint8_t pattern, uint8_t post_delay) {
  // Come out of standby
  I2C_Write(DRV2605L_ADDRESS, 0x01, 0);
  // nap(128);

  // write pattern
  I2C_Write(DRV2605L_ADDRESS, 0x04, pattern);
  // Go
  I2C_Write(DRV2605L_ADDRESS, 0x0C, 0x01);

  // wait for effect
  nap(post_delay);

  // back to standby
  I2C_Write(DRV2605L_ADDRESS, 0x01, 1 << 6);
}