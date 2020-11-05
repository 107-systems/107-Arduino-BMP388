/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

#ifndef ARDUINO_BMP388_BMP388_CONVERT_H_
#define ARDUINO_BMP388_BMP388_CONVERT_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Const.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

uint32_t                 toRawTemperature          (RawSensorData const & data);
uint32_t                 toRawPressure             (RawSensorData const & data);
QuantizedCalibrationData toQuantizedCalibrationData(CalibrationData const & calib_data);
double                   compensateRawTemperature  (uint32_t const raw_temperature, QuantizedCalibrationData & quant_calib_data);
double                   compensateRawPressure     (uint32_t const raw_pressure, double const temperature_deg, QuantizedCalibrationData & quant_calib_data);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_CONVERT_H_ */
