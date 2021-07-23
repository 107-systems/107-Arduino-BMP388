/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

#ifndef ARDUINO_BMP388_H_
#define ARDUINO_BMP388_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#undef max
#undef min
#include <functional>

#include "BMP388/BMP388_Io.h"
#include "BMP388/BMP388_Config.h"
#include "BMP388/BMP388_Control.h"
#include "BMP388/BMP388_Convert.h"

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef std::function<void(double const, double const)> OnSensorDataFunc;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoBMP388
{
public:

  ArduinoBMP388(BMP388::SpiSelectFunc select,
                BMP388::SpiDeselectFunc deselect,
                BMP388::SpiTransferFunc transfer,
                OnSensorDataFunc on_sensor_data);


  void begin(BMP388::OutputDataRate const odr);


  void onExternalEventHandler();

  static double convertPressureToAltitude(double const pressure_hpa);

  uint8_t getChipId(void);


private:

  BMP388::BMP388_Io _io;
  BMP388::BMP388_Config _config;
  BMP388::BMP388_Control _control;
  OnSensorDataFunc _on_sensor_data;

  BMP388::QuantizedCalibrationData _quant_calib_data;

  void readSensorData(double & pressure_hpa, double & temperature_deg);

};

#endif /* ARDUINO_BMP388_H_ */
