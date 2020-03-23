/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
 */

#ifndef ARDUINO_BMP388_BMP388_CONFIG_H_
#define ARDUINO_BMP388_BMP388_CONFIG_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Io.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

enum class IntPinOutputType
{
  PushPull  = 0,
  OpenDrain = bm(INT_CTRL::INT_OD)
};

enum class IntPinLevel
{
  ActiveLow  = 0,
  ActiveHigh = bm(INT_CTRL::INT_LEVEL)
};

enum class PowerMode : uint8_t
{
  Sleep  = 0,
  Normal = bm(PWR_CTRL::MODE_1) | bm(PWR_CTRL::MODE_0)
};

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class BMP388_Config
{
public:

  BMP388_Config(BMP388_Io & io);


  void configIntPinOutputType(IntPinOutputType const type);
  void configIntPinLevel     (IntPinLevel const level);
  void enableDataReadyInt    ();
  void enablePressure        ();
  void enableTemperature     ();
  void configPowerMode       (PowerMode const mode);


private:

  BMP388_Io & _io;

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_CONFIG_H_ */
