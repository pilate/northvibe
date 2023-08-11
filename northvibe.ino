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
#define SAMPLE_RATE (128) // replace this with actual sample rate

void blink(uint8_t times, uint32_t delay) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    nap(256);
    digitalWrite(LED_PIN, LOW);
    nap(delay);
  }
}

void crossProduct(float v_A[], float v_B[], float c_P[]) {
   c_P[0] = v_A[1] * v_B[2] - v_A[2] * v_B[1];
   c_P[1] = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
   c_P[2] = v_A[0] * v_B[1] - v_A[1] * v_B[0];
}

void setup() {
  /*
   * 1. Initialize GPIO
   * 2. Initialize Vibe
   * 3. Initialize Accel/Gyro (LSM6DSO) and enable passthru
   * 4. Initialize Mag (MMC5983)
   * 5. Configure Accel/Gyro power settings and sensitivity
   * 6. Initialize position
   * 7. Blink and Vibe to signal boot
   */

  uint8_t setup_success = 1;
  /* 1. Initialize GPIO */
  // Disable ADC modules on ATTiny
  ADCSRA = 0;
  // set LED pin to output
  DDRB |= ( 1 << LED_PIN );
  /* 2. Initialize Vibe */
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

  /* 6. Initialize position (assume laying flat on table?) */
  //Quaternion_setIdentity(&g_cur_pos);

  /* 7. Blink and Vibe to signal boot */
  Vibe.effect(14, 128);  
  blink(5, 256);
}

void loop() {
  /* 
   *
   * 1) Poll the sensors 
   * 2) Create Vectors
   * 3) Calculate North-East-Down coodinates
   * 4) Vibe when pointing North 
   */

  Magneto.read();
  AccelGyro.read();

  // Acquire latest sensor data
  FusionVector gyroscope = {AccelGyro.g_x,  AccelGyro.g_x,  AccelGyro.g_x};
  FusionVector accelerometer = {AccelGyro.a_x,  AccelGyro.a_y,  AccelGyro.a_z};
  FusionVector magnetometer = {Magneto.x, Magneto.y, Magneto.z };

  const FusionVector up = FusionVectorMultiplyScalar(accelerometer, -1.0f);
  const FusionVector west = FusionVectorNormalise(FusionVectorCrossProduct(up, magnetometer));
  const FusionVector north = FusionVectorNormalise(FusionVectorCrossProduct(west, up));

  char message[100];
  memset(message, 0x00, 100);
  sprintf(message, "%f %f %fr\n\0", north.axis.x, north.axis.y, north.axis.z);
  I2C_LogString(message, 100);

  nap(SAMPLE_RATE);

  /* Toggle the LED */
  //digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
