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

enum class PressureOversampling : uint8_t
{
  x1  = 0,
  x2  =                                      bm(OSR::OSR_P_0),
  x4  =                   bm(OSR::OSR_P_1),
  x8  =                   bm(OSR::OSR_P_1) | bm(OSR::OSR_P_0),
  x16 = bm(OSR::OSR_P_2),
  x32 = bm(OSR::OSR_P_2)                   | bm(OSR::OSR_P_0),
};

enum class TemperatureOversampling : uint8_t
{
  x1  = 0,
  x2  =                                      bm(OSR::OSR_T_0),
  x4  =                   bm(OSR::OSR_T_1),
  x8  =                   bm(OSR::OSR_T_1) | bm(OSR::OSR_T_0),
  x16 = bm(OSR::OSR_T_2),
  x32 = bm(OSR::OSR_T_2)                   | bm(OSR::OSR_T_0),
};

enum class OutputDataRate : uint8_t
{
  ODR_200_Hz    = 0x00,
  ODR_100_Hz    = 0x01,
  ODR_50_Hz     = 0x02,
  ODR_25_Hz     = 0x03,
  ODR_12_5_Hz   = 0x04,
  ODR_6_25_Hz   = 0x05,
  ODR_3_1_Hz    = 0x06,
  ODR_1_5_Hz    = 0x07,
  ODR_0_78_Hz   = 0x08,
  ODR_0_39_Hz   = 0x09,
  ODR_0_2_Hz    = 0x0A,
  ODR_0_1_Hz    = 0x0B,
  ODR_0_05_Hz   = 0x0C,
  ODR_0_02_Hz   = 0x0D,
  ODR_0_01_Hz   = 0x0E,
  ODR_0_006_Hz  = 0x0F,
  ODR_0_003_Hz  = 0x10,
  ODR_0_0015_Hz = 0x11
};

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


  void configPressureOversampling   (PressureOversampling const over_sampling);
  void configTemperatureOversampling(TemperatureOversampling const over_sampling);
  void configOutputDataRate         (OutputDataRate const odr);
  void configIntPinOutputType       (IntPinOutputType const type);
  void configIntPinLevel            (IntPinLevel const level);
  void enableDataReadyInt           ();
  void enablePressure               ();
  void enableTemperature            ();
  void configPowerMode              (PowerMode const mode);


private:

  BMP388_Io & _io;

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_CONFIG_H_ */
