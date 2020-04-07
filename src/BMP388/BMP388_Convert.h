/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
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
