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
  _io.modify(Register::INT_CTRL,
             bm(INT_CTRL::INT_OD),
             to_integer(type));
}

void BMP388_Config::configIntPinLevel(IntPinLevel const level)
{
  _io.modify(Register::INT_CTRL,
             bm(INT_CTRL::INT_LEVEL),
             to_integer(level));
}

void BMP388_Config::enableDataReadyInt()
{
  _io.modify(Register::INT_CTRL,
             bm(INT_CTRL::DRDY_EN),
             bm(INT_CTRL::DRDY_EN));
}

void BMP388_Config::enablePressure()
{
  _io.modify(Register::PWR_CTRL,
             bm(PWR_CTRL::PRESS_EN),
             bm(PWR_CTRL::PRESS_EN));
}

void BMP388_Config::enableTemperature()
{
  _io.modify(Register::PWR_CTRL,
             bm(PWR_CTRL::TEMP_EN),
             bm(PWR_CTRL::TEMP_EN));
}

void BMP388_Config::configPowerMode(PowerMode const mode)
{
  _io.modify(Register::PWR_CTRL,
             bm(PWR_CTRL::MODE_1) | bm(PWR_CTRL::MODE_0),
             to_integer(mode));
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
