/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Convert.h"

#include <math.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * FUNCTION DEFINITION
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

QuantizedCalibrationData toQuantizedCalibrationData(CalibrationData const & calib_data)
{
  QuantizedCalibrationData quant_calib_data;

  quant_calib_data.T1 = (static_cast<double>(calib_data.coefficient.T1) / 0.00390625f);
  quant_calib_data.T2 = (static_cast<double>(calib_data.coefficient.T2) / 1073741824.0f);
  quant_calib_data.T3 = (static_cast<double>(calib_data.coefficient.T3) / 281474976710656.0f);
  quant_calib_data.P1 = (static_cast<double>(calib_data.coefficient.P1 - 16384) / 1048576.0f);
  quant_calib_data.P2 = (static_cast<double>(calib_data.coefficient.P2 - 16384) / 536870912.0f);
  quant_calib_data.P3 = (static_cast<double>(calib_data.coefficient.P3) / 4294967296.0f);
  quant_calib_data.P4 = (static_cast<double>(calib_data.coefficient.P4) / 137438953472.0f);
  quant_calib_data.P5 = (static_cast<double>(calib_data.coefficient.P5) / 0.125f);
  quant_calib_data.P6 = (static_cast<double>(calib_data.coefficient.P6) / 64.0f);
  quant_calib_data.P7 = (static_cast<double>(calib_data.coefficient.P7) / 256.0f);
  quant_calib_data.P8 = (static_cast<double>(calib_data.coefficient.P8) / 32768.0f);
  quant_calib_data.P9 = (static_cast<double>(calib_data.coefficient.P9) / 281474976710656.0f);
  quant_calib_data.P10 = (static_cast<double>(calib_data.coefficient.P10) / 281474976710656.0f);
  quant_calib_data.P11 = (static_cast<double>(calib_data.coefficient.P11) / 36893488147419103232.0f);

  return quant_calib_data;
}

double compensateRawTemperature(uint32_t const raw_temperature, QuantizedCalibrationData & quant_calib_data)
{
  double const partial_data1 = (double)(raw_temperature - quant_calib_data.T1);
  double const partial_data2 = (double)(partial_data1 * quant_calib_data.T2);

  return (partial_data2 + (partial_data1 * partial_data1) * quant_calib_data.T3);
}

double compensateRawPressure(uint32_t const raw_pressure, double const temperature_deg, QuantizedCalibrationData & quant_calib_data)
{
  double       partial_data1 = quant_calib_data.P6 * temperature_deg;
  double       partial_data2 = quant_calib_data.P7 * pow(temperature_deg, 2);
  double       partial_data3 = quant_calib_data.P8 * pow(temperature_deg, 3);
  double const partial_out1 = quant_calib_data.P5 + partial_data1 + partial_data2 + partial_data3;

               partial_data1 = quant_calib_data.P2 * temperature_deg;
               partial_data2 = quant_calib_data.P3 * pow(temperature_deg, 2);
               partial_data3 = quant_calib_data.P4 * pow(temperature_deg, 3);
  double const partial_out2  = static_cast<double>(raw_pressure) * (quant_calib_data.P1 + partial_data1 + partial_data2 + partial_data3);

               partial_data1 = pow(static_cast<double>(raw_pressure), 2);
               partial_data2 = quant_calib_data.P9 + quant_calib_data.P10 * temperature_deg;
               partial_data3 = partial_data1 * partial_data2;
  double const partial_out3  = partial_data3 + pow(static_cast<double>(raw_pressure), 3) * quant_calib_data.P11;

  return (partial_out1 + partial_out2 + partial_out3);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
