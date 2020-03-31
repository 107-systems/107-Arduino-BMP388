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

int64_t compensateRawTemperature(uint32_t const raw_temperature, int64_t & t_lin, CalibrationData const & calib_data)
{
  uint64_t partial_data1 = raw_temperature - (256 * calib_data.coefficient.T1);
  uint64_t partial_data2 = calib_data.coefficient.T2 * partial_data1;
  uint64_t partial_data3 = partial_data1 * partial_data1;
  int64_t  partial_data4 = static_cast<int64_t>(partial_data3) * calib_data.coefficient.T3;
  int64_t  partial_data5 = (static_cast<int64_t>(partial_data2 * 262144) + partial_data4);
  int64_t  partial_data6 = partial_data5 / 4294967296;
  t_lin = partial_data6;
  int64_t const comp_temp = static_cast<int64_t>((partial_data6 * 25)  / 16384);
  return comp_temp;
}

uint64_t compensateRawPressure(uint32_t const raw_pressure, int64_t const t_lin, CalibrationData const & calib_data)
{
  int64_t partial_data1 = t_lin * t_lin;
  int64_t partial_data2 = partial_data1 / 64;
  int64_t partial_data3 = (partial_data2 * t_lin) / 256;
  int64_t partial_data4 = (calib_data.coefficient.P8 * partial_data3) / 32;
  int64_t partial_data5 = (calib_data.coefficient.P7 * partial_data1) * 16;
  int64_t partial_data6 = (calib_data.coefficient.P6 * t_lin) * 4194304;
  int64_t offset = (calib_data.coefficient.P5 * 140737488355328) + partial_data4 + partial_data5 + partial_data6;

  partial_data2 = (calib_data.coefficient.P4 * partial_data3) / 32;
  partial_data4 = (calib_data.coefficient.P3 * partial_data1) * 4;
  partial_data5 = (calib_data.coefficient.P2 - 16384) * t_lin * 2097152;
  int64_t sensitivity = ((calib_data.coefficient.P1 - 16384) * 70368744177664) + partial_data2 + partial_data4 + partial_data5;

  partial_data1 = (sensitivity / 16777216) * raw_pressure;
  partial_data2 = calib_data.coefficient.P10 * t_lin;
  partial_data3 = partial_data2 + (65536 * calib_data.coefficient.P9);
  partial_data4 = (partial_data3 * raw_pressure) / 8192;
  partial_data5 = (partial_data4 * raw_pressure) / 512;
  partial_data6 = static_cast<int64_t>(static_cast<uint64_t>(raw_pressure) * static_cast<uint64_t>(raw_pressure));
  partial_data2 = (calib_data.coefficient.P11 * partial_data6) / 65536;
  partial_data3 = (partial_data2 * raw_pressure) / 128;
  partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;
  uint64_t comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776);

  return comp_press;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
