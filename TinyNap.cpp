#include "TinyNap.h"
#include <avr/sleep.h>
#include <avr/wdt.h>


void tn_wdt_setup(uint8_t wdp)
{
  // WDP3 is 6th bit? cool
  if (wdp & 0b00001000)
  {
    wdp |= bit(WDP3);
  }

  // interrupt flag, interupt enable, change enable, enable
  wdp |= bit(WDIF) | bit(WDIE) | bit(WDCE) | bit(WDE);

  WDTCR = wdp;
}

void nap(uint32_t nap_time)
{
  uint16_t timeout;
  uint8_t wdp;

  // Save ADC state
  uint8_t old_adcsra = ADCSRA;

  // Turn off ADC
  ADCSRA = 0;

  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  for (timeout = 8192, wdp = 9; timeout >= 16; timeout /= 2, wdp--)
  {
    while (nap_time >= timeout)
    {
      tn_wdt_setup(wdp);
      sei();
      sleep_cpu();
      nap_time -= timeout;
    }
  }

  sleep_disable();

  // Restore ADC state
  ADCSRA = old_adcsra;
}

// void nap(uint16_t nap_time)
// {
//   uint16_t i;
//   uint8_t s;

//   // Save ADC state
//   uint8_t old_adcsra = ADCSRA;

//   // Turn off ADC
//   ADCSRA = 0;

//   sleep_enable();
//   set_sleep_mode(SLEEP_MODE_PWR_DOWN);

//   for (i = 8192, s = 9; i >= 16; i /= 2, s--)
//   {
//     while (nap_time > i)
//     {
//       tn_wdt_setup(s);
//       sei();
//       sleep_cpu();
//       nap_time -= i;
//     }
//   }

//   sleep_disable();

//   // Restore ADC state
//   ADCSRA = old_adcsra;
// }

ISR(WDT_vect)
{
  wdt_disable();
}
