#include "MMC5983MA.h"


uint8_t MMC5983MA::getProductID()
{
  return I2C_ReadByte(MMC5983MA_ADDRESS, MMC5983MA_PRODUCT_ID);
}