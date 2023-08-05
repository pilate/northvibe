#ifndef LSM6DSO_h
#define LSM6DSO_h

#include "Arduino.h"
#include "mi2c.h"

#define LSM6DSO_ADDRESS         0x6b

#define LSM6DSO_MASTER_CONFIG   0x14
#define LSM6DSO_FUNC_CFG_ACCESS 0x01
#define LSM6DSO_WHO_AM_I        0x0f  // Should be 0x6C

class LSM6DSO {
public:
  // MMC5983MA(I2Cdev* i2c_bus);
  uint8_t WhoAmI();
  void I2CPassthrough();
  // void init(uint8_t MODR, uint8_t MBW, uint8_t MSET);
  // void offsetBias(float * dest1, float * dest2);
  // void reset();
  // uint8_t status();
  // void clearInt();
  // void selfTest();
  // void readData(uint32_t * destination);
  // uint8_t readTemperature();
  // void SET();
  // void RESET();
  // void getOffset(float * destination);
  // void powerDown();
  // void powerUp(uint8_t MODR);

  // private:
  // float _mRes;
  // I2Cdev* _i2c_bus;
};

#endif