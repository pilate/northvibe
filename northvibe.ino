#include <Wire.h>

#include "LSM6DSO.h"
#include "mi2c.h"
#include "MMC5983MA.h"
#include "TinyNap.h"


const uint8_t LED_PIN = PB4;
const uint8_t SERIAL_I2C = 0x11;

MMC5983MA Magneto;
LSM6DSO AccelGyro;

void blink(uint8_t times) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    nap(256);
    digitalWrite(LED_PIN, LOW);
    nap(256);
  }
}


void setup() {
  Wire.begin();

  uint8_t whoami = AccelGyro.WhoAmI();
  if (whoami == 0x6C) {
    blink(2);
    AccelGyro.I2CPassthrough();
  }

  delay(2000);
  Wire.end();
}


void loop() {
  blink(3);

  Wire.begin();
  I2C_LogString("looping\r\n\0", 10);
  
  delay(100);

  Magneto.read();
  AccelGyro.read();

  char message[150];

  memset(message, 0x00, 150);
  sprintf(message, "mag x: %i\r\n\0", Magneto.x);
  I2C_LogString(message, 150);
  
  memset(message, 0x00, 150);
  sprintf(message, "mag y: %i\r\n\0", Magneto.y);
  I2C_LogString(message, 150);

  memset(message, 0x00, 150);
  sprintf(message, "mag z: %i\r\n\0", Magneto.z);
  I2C_LogString(message, 150);

  memset(message, 0x00, 150);
  sprintf(message, "accel x: %i\r\n\0", AccelGyro.a_x);
  I2C_LogString(message, 150);
  
  memset(message, 0x00, 150);
  sprintf(message, "accel y: %i\r\n\0", AccelGyro.a_y);
  I2C_LogString(message, 150);

  memset(message, 0x00, 150);
  sprintf(message, "accel z: %i\r\n\0", AccelGyro.a_z);
  I2C_LogString(message, 150);

  memset(message, 0x00, 150);
  sprintf(message, "gyro x: %i\r\n\0", AccelGyro.g_x);
  I2C_LogString(message, 150);
  
  memset(message, 0x00, 150);
  sprintf(message, "gyro y: %i\r\n\0", AccelGyro.g_y);
  I2C_LogString(message, 150);

  memset(message, 0x00, 150);
  sprintf(message, "gyro z: %i\r\n\0", AccelGyro.g_z);
  I2C_LogString(message, 150);

  I2C_LogString("---\r\n\0", 6);

  Wire.end();

  delay(10000);
}
