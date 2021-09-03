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

#include <107-Arduino-Sensor.hpp>

#undef max
#undef min
#include <functional>

#include "BMP388/BMP388_Io.h"
#include "BMP388/BMP388_Config.h"
#include "BMP388/BMP388_Control.h"
#include "BMP388/BMP388_Convert.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace drone
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoBMP388 : virtual public PressureSensorBase,
                      virtual public TemperatureSensorBase
{
public:

  typedef std::function<void(unit::Pressure const)> OnPressureDataUpdateFunc;
  typedef std::function<void(unit::Temperature const)> OnTemperatureDataUpdateFunc;


  ArduinoBMP388(BMP388::SpiSelectFunc select,
                BMP388::SpiDeselectFunc deselect,
                BMP388::SpiTransferFunc transfer,
                OnPressureDataUpdateFunc on_pressure_data_update,
                OnTemperatureDataUpdateFunc on_temperature_data_update);


  void begin(BMP388::OutputDataRate const odr);


  void onExternalEventHandler();


  static unit::Length convertPressureToAltitude(unit::Pressure const pressure);

  virtual void get      (unit::Pressure & p) const override { p = _pressure; }
  virtual void get      (unit::Temperature & t) const override { t = _temperature; }
  virtual size_t printTo(Print & p) const override;


  inline BMP388::BMP388_Io & io() { return _io; }


private:

  BMP388::BMP388_Io _io;
  BMP388::BMP388_Config _config;
  BMP388::BMP388_Control _control;
  unit::Pressure _pressure;
  unit::Temperature _temperature;

  BMP388::QuantizedCalibrationData _quant_calib_data;

  void readSensorData(double & pressure_hpa, double & temperature_deg);
  void setSensorUpdateRate(BMP388::OutputDataRate const odr);

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* drone */

#endif /* ARDUINO_BMP388_H_ */
