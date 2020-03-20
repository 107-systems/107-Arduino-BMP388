/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Config.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

BMP388_Config::BMP388_Config(BMP388_Io & io)
: _io{io}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void BMP388_Config::configIntPinOutputType(IntPinOutputType const type)
{
  uint8_t const reg_val = (type == IntPinOutputType::OpenDrain) ? bm(INT_CTRL::INT_OD) : 0;
  uint8_t const reg_mask = bm(INT_CTRL::INT_OD);
  _io.modify(Register::INT_CTRL, reg_mask, reg_val);
}

void BMP388_Config::configIntPinLevel(IntPinLevel const level)
{
  uint8_t const reg_val = (level == IntPinLevel::ActiveHigh) ? bm(INT_CTRL::INT_LEVEL) : 0;
  uint8_t const reg_mask = bm(INT_CTRL::INT_LEVEL);
  _io.modify(Register::INT_CTRL, reg_mask, reg_val);
}

void BMP388_Config::enableDataReadyInt()
{
  _io.modify(Register::INT_CTRL, bm(INT_CTRL::DRDY_EN), bm(INT_CTRL::DRDY_EN));
}

void BMP388_Config::enablePressure()
{
  _io.modify(Register::PWR_CTRL, bm(PWR_CTRL::PRESS_EN), bm(PWR_CTRL::PRESS_EN));
}

void BMP388_Config::enableTemperature()
{
  _io.modify(Register::PWR_CTRL, bm(PWR_CTRL::TEMP_EN), bm(PWR_CTRL::TEMP_EN));
}

void BMP388_Config::configPowerMode(PowerMode const mode)
{
  _io.modify(Register::PWR_CTRL, to_integer(PowerMode::Normal), to_integer(mode));
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
