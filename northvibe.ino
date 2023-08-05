#include <TinyNap.h>
#include <Wire.h>

#include "MMC5983MA.h"
#include "LSM6DSO.h"
#include "mi2c.h"


const uint8_t LED_PIN = PB4;

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
  delay(100);

  Wire.begin();

  uint8_t whoami = AccelGyro.WhoAmI();
  if (whoami == 0x6C) {
    blink(1);
    AccelGyro.I2CPassthrough();
  }
  else if (whoami == 0xff) {
    blink(3);
  }
  else {
    blink(6);
  }
  delay(5000);  

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x0);
  Wire.write("testing\r\n", 9);
  Wire.endTransmission();

  Wire.end();
}


void loop() {
  // put your main code here, to run repeatedly:
}
