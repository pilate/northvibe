#ifndef LSM6DSO_h
#define LSM6DSO_h

#include "Arduino.h"
#include "mi2c.h"

#define LSM6DSO_ADDRESS         0x6b

#define LSM6DSO_SENSOR_HUB_MASTER_CONFIG   0x14

#define LSM6DSO_CTRL1_XL        0x10
#define LSM6DSO_CTRL5_C         0x14
#define LSM6DSO_CTRL6_C         0x15
#define LSM6DSO_OUTX_L_G        0x22 // gyro values
#define LSM6DSO_OUTX_L_A        0x28 // accel values


#define LSM6DSO_CTRL5_C_XL_ULP_EN    0x07
#define LSM6DSO_CTRL6_C_XL_HM_MODE   0x04

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
    void read();
    // void readData(uint32_t * destination);
    // uint8_t readTemperature();
    // void SET();
    // void RESET();
    // void getOffset(float * destination);
    // void powerDown();
    // void powerUp(uint8_t MODR);

    // accel values
    int32_t a_x;
    int32_t a_y;
    int32_t a_z;

    // gyro values
    int32_t g_x;
    int32_t g_y;
    int32_t g_z;

  private:
    uint8_t gyroRawData[6];
    uint8_t accelRawData[6];
    // float _mRes;
    // I2Cdev* _i2c_bus;
};

#endif