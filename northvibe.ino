#include <Wire.h>
#include "DRV2605L.h"
#include "LSM6DSO.h"
#include "mi2c.h"
#include "MMC5983MA.h"
#include "TinyNap.h"
#include "Fusion.h"

const uint8_t LED_PIN = PB4;
const uint8_t SWITCH_PIN = PB3;
const uint8_t SERIAL_I2C = 0x11;

MMC5983MA Magneto;
LSM6DSO AccelGyro;
DRV2605L Vibe;
#define SAMPLE_RATE (128) // milliseconds to nap between samples

void blink(uint8_t times, uint32_t delay) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    nap(256);
    digitalWrite(LED_PIN, LOW);
    nap(delay);
  }
}

void setup() {
  /*
   * 1. Initialize GPIO
   * 2. Initialize Vibe
   * 3. Initialize Accel/Gyro (LSM6DSO) and enable passthru
   * 4. Initialize Mag (MMC5983)
   * 5. Configure Accel/Gyro power settings and sensitivity
   * 6. Blink and Vibe to signal boot
   */

  uint8_t setup_success = 1;
  /* 1. Initialize GPIO */
  // Disable ADC modules on ATTiny
  ADCSRA = 0;
  // set LED pin to output
  DDRB |= ( 1 << LED_PIN );
  /* 2. Initialize Vibe */
  // Allow I2C
  Wire.begin();
  Vibe.init();
  /* 3. Initialize Accel/Gyro (LSM6DSO) and enable passthru */
  uint8_t whoami = AccelGyro.WhoAmI();
  if (setup_success && whoami == 0x6C) {
    AccelGyro.I2CPassthrough();
    delay(200);
  } else {
    setup_success = 0;
  }
  /* 4. Initialize Mag (MMC5983) */
  uint8_t product_id = Magneto.getProductID();
  if (setup_success && product_id == 0x30) {
    Magneto.getOffsets();
    delay(200);
  } else {
    setup_success = 0;
  }
  /* 5. Configure Accel/Gyro power settings and sensitivity */
  AccelGyro.enable_accel();
  AccelGyro.enable_gyro();

  /* 6. Blink and Vibe to signal boot */
  Vibe.effect(14, 128);  
  blink(5, 256);
  Wire.end();
}

void loop() {
  /* 
   * 1) Poll the sensors 
   * 2) Build sensor data vectors
   * 3) Calculate North-East-Down coodinates
   * 4) Vibe when pointing North 
   * 5) Enable LED toggle when pitched on horizon
   * 6) Sleep until next sample
   */

  /* 1) Poll the sensors */
  Wire.begin();
  Magneto.read();
  AccelGyro.read();

  /* 2) Build sensor data vectors */
  FusionVector gyroscope =     {(float) AccelGyro.g_x, \
                                (float) AccelGyro.g_x, \
                                (float) AccelGyro.g_x};
  FusionVector accelerometer = {(float)AccelGyro.a_x,  \
                                (float)AccelGyro.a_y,  \
                                (float)AccelGyro.a_z};
  FusionVector magnetometer =  {(float)Magneto.x,      \
                                (float)Magneto.y,      \
                                (float)Magneto.z };

  /* 3) Calculate North-East-Down coodinates
   * Orientation Calculation:
   *  - Accelerometer points down at rest, find "up" by inverting
   *  - Magnetometer points north, calculate "west" using cross product
   *    of "up" and magnetometer
   *  - "north" can be found by cross product of "west" and "up" again.
   */
  FusionVector up = FusionVectorMultiplyScalar(accelerometer, -1.0f);
  FusionVector west = FusionVectorNormalise(FusionVectorCrossProduct(up, magnetometer));
  FusionVector north = FusionVectorNormalise(FusionVectorCrossProduct(west, up));

  /* 4) Vibe when pointing North */
  bool pointing_north = false;
  if (FusionVectorMagnitude(west) < 0.2f) {
    pointing_north = true;
    Vibe.effect(14, 128);  
  }

  // Debug print
  char message[100];
  memset(message, 0x00, 100);
  sprintf(message, "%f %f %f\r\n\0", north.axis.x, north.axis.y, north.axis.z);
  I2C_LogString(message, 100);
  Wire.end();

  /* 5) Enable LED toggle when pitched on horizon */
  /* Toggle the LED */
  //digitalWrite(LED_PIN, !digitalRead(LED_PIN));

  /* 6) Sleep until next sample */
  nap(SAMPLE_RATE);
}
