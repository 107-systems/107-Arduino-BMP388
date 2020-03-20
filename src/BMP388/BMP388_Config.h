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
  PushPull,
  OpenDrain
};

enum class IntPinLevel
{
  ActiveLow,
  ActiveHigh
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


private:

  BMP388_Io & _io;

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_CONFIG_H_ */
