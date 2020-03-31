/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Control.h"

#include <Arduino.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

BMP388_Control::BMP388_Control(BMP388_Io & io)
: _io{io}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void BMP388_Control::reset()
{
  _io.write(Register::CMD, to_integer(CMD::SOFT_RESET));
  delay(10);
}

void BMP388_Control::readCalibData(CalibrationData & calib_data)
{
  _io.read(Register::CALIB_DATA, calib_data.buf, sizeof(calib_data.buf));
}

void BMP388_Control::readRawData(RawSensorData & data)
{
  _io.read(Register::DATA, data.buf, sizeof(data.buf));
}

/**************************************************************************************
 * FREE FUNCTION DEFINITION
 **************************************************************************************/

uint32_t toRawPressure(RawSensorData const & data)
{
  uint32_t data_xlsb = 0,
	         data_lsb  = 0,
	         data_msb  = 0;

	data_xlsb = static_cast<uint32_t>(data.reg.pres_xlsb) << 0;
	data_lsb  = static_cast<uint32_t>(data.reg.pres_lsb)  << 8;
	data_msb  = static_cast<uint32_t>(data.reg.pres_msb)  << 16;

  uint32_t const raw_pressure = data_msb | data_lsb | data_xlsb;

  return raw_pressure;
}

uint32_t toRawTemperature(RawSensorData const & data)
{
  uint32_t data_xlsb = 0,
	         data_lsb  = 0,
	         data_msb  = 0;

	data_xlsb = static_cast<uint32_t>(data.reg.temp_xlsb) << 0;
	data_lsb  = static_cast<uint32_t>(data.reg.temp_lsb)  << 8;
	data_msb  = static_cast<uint32_t>(data.reg.temp_msb)  << 16;

  uint32_t const raw_temperature = data_msb | data_lsb | data_xlsb;

  return raw_temperature;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
