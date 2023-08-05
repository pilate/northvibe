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
  delay(100);

  Magneto.read();
  I2C_LogString("loopin\r\n", 8);

  Wire.end();

  delay(10000);
}
