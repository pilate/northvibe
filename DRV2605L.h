#ifndef DRV2605L_h
#define DRV2605L_h

#include "Arduino.h"
#include "mi2c.h"

#define DRV2605L_ADDRESS 0x5a


class DRV2605L {
public:
  void init();
  void effect(uint8_t pattern, uint8_t post_delay);
};

#endif