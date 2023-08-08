#include <Wire.h>

#include "DRV2605L.h"
#include "LSM6DSO.h"
#include "mi2c.h"
#include "MMC5983MA.h"
#include "TinyNap.h"


const uint8_t LED_PIN = PB4;
const uint8_t SWITCH_PIN = PB3;

const uint8_t SERIAL_I2C = 0x11;

MMC5983MA Magneto;
LSM6DSO AccelGyro;
DRV2605L Vibe;

void blink(uint8_t times) {
  blink(times, 256);
}

void blink(uint8_t times, uint32_t delay) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    nap(256);
    digitalWrite(LED_PIN, LOW);
    nap(delay);
  }
}


void setup() {
  pinMode(LED_PIN, OUTPUT);

  Wire.begin();

  // Vibe.init();
  // delay(500);
  // Vibe.effect(14, 128);    

  uint8_t whoami = AccelGyro.WhoAmI();
  if (whoami == 0x6C) {
    blink(2);
    AccelGyro.I2CPassthrough();
  }

  nap(16);
  Magneto.getOffsets();

  delay(2000);
  Wire.end();
}


void loop() {
  Wire.begin();
  I2C_LogString("looping\r\n\0", 10);
  
  delay(100);

  // AccelGyro.enable_accel();
  // Magneto.enable_gyro();

  Magneto.read();

  // AccelGyro.read();

  // AccelGyro.disable_accel();

  char message[100];

  memset(message, 0x00, 100);
  sprintf(message, "mag x: %i\r\n\0", Magneto.x);
  I2C_LogString(message, 100);
  
  memset(message, 0x00, 100);
  sprintf(message, "mag y: %i\r\n\0", Magneto.y);
  I2C_LogString(message, 100);

  memset(message, 0x00, 100);
  sprintf(message, "mag z: %i\r\n\0", Magneto.z);
  I2C_LogString(message, 100);

  delay(1000);

  // memset(message, 0x00, 100);
  // sprintf(message, "accel x: %i\r\n\0", AccelGyro.a_x);
  // I2C_LogString(message, 100);
  
  // memset(message, 0x00, 100);
  // sprintf(message, "accel y: %i\r\n\0", AccelGyro.a_y);
  // I2C_LogString(message, 100);

  // memset(message, 0x00, 100);
  // sprintf(message, "accel z: %i\r\n\0", AccelGyro.a_z);
  // I2C_LogString(message, 100);

  // delay(1000);
  // memset(message, 0x00, 100);
  // sprintf(message, "gyro x: %i\r\n\0", AccelGyro.g_x);
  // I2C_LogString(message, 100);
  
  // memset(message, 0x00, 100);
  // sprintf(message, "gyro y: %i\r\n\0", AccelGyro.g_y);
  // I2C_LogString(message, 100);

  // memset(message, 0x00, 100);
  // sprintf(message, "gyro z: %i\r\n\0", AccelGyro.g_z);
  // I2C_LogString(message, 100);

  // memset(message, 0x00, 100);
  // sprintf(message, "mag x offset: %i\r\n\0", Magneto.x_offset);
  // I2C_LogString(message, 100);

  // memset(message, 0x00, 100);
  // sprintf(message, "mag y offset: %i\r\n\0", Magneto.y_offset);
  // I2C_LogString(message, 100);

  // memset(message, 0x00, 100);
  // sprintf(message, "mag z offset: %i\r\n\0", Magneto.z_offset);
  // I2C_LogString(message, 100);

  // delay(100);

  I2C_LogString("---\r\n\0", 6);

  Wire.end();

  if (Magneto.z >= 0) {
    uint32_t abs_x = abs(Magneto.x);
    if (abs_x < 2000) {
      blink(3, abs_x / 10);
    }

  }

  delay(1000);
}
