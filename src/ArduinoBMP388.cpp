/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "ArduinoBMP388.h"

#include <math.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

using namespace BMP388;

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoBMP388::ArduinoBMP388(SpiSelectFunc select,
                             SpiDeselectFunc deselect,
                             SpiTransferFunc transfer,
                             OnPressureDataUpdateFunc on_pressure_data_update,
                             OnTemperatureDataUpdateFunc on_temperature_data_update)
: drone::PressureSensorBase("BMP388/pressure",
                            300.0  * drone::unit::pascal,
                            1250.0 * drone::unit::pascal,
                            1.0    * drone::unit::pascal,
                            0.0    * drone::unit::hertz,
                            on_pressure_data_update)
, drone::TemperatureSensorBase("BMP388/temperature",
                               233.15 * drone::unit::kelvin, /* -40 °C */
                               358.15 * drone::unit::kelvin, /* +85 °C */
                               0.1    * drone::unit::kelvin,
                               0.0    * drone::unit::hertz,
                               on_temperature_data_update)
, _io{select, deselect, transfer}
, _config{_io}
, _control{_io}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::begin(OutputDataRate const odr)
{
  _control.reset();

  BMP388::CalibrationData calib_data;
  _control.readCalibData(calib_data);
  _quant_calib_data = toQuantizedCalibrationData(calib_data);

  _config.configPressureOversampling(PressureOversampling::x32);
  _config.configTemperatureOversampling(TemperatureOversampling::x2);
  _config.configOutputDataRate(odr);

  _config.configIntPinOutputType(IntPinOutputType::OpenDrain);
  _config.configIntPinLevel(IntPinLevel::ActiveLow);
  _config.enableDataReadyInt();

  _config.enablePressure();
  _config.enableTemperature();

  _config.configPowerMode(PowerMode::Normal);
}

void ArduinoBMP388::onExternalEventHandler()
{
  uint8_t const int_status = _io.read(Register::INT_STATUS_REG);

  if(int_status & bm(INT_STATUS::DRDY))
  {
    double pressure_hpa = 0.0,
           temperature_deg = 0.0;

    readSensorData(pressure_hpa, temperature_deg);

    _pressure = pressure_hpa * 100.0 * drone::unit::pascal;
    _temperature = (temperature_deg + 273.15) * drone::unit::kelvin;

    drone::PressureSensorBase::onSensorValueUpdate(_pressure);
    drone::TemperatureSensorBase::onSensorValueUpdate(_temperature);
  }
}

drone::unit::Length ArduinoBMP388::convertPressureToAltitude(drone::unit::Pressure const pressure)
{
  /* This formula assumes the international standard atmosphere (standard
   * temperature 15 °C = 288.15 K, static pressure = 1013.25 hPa, standard
   * temperature lapse rate 0.65 K / 100 m) and can be considered valid up
   * until 11 km.
   */
  static auto constexpr Tb  = 218.15 * drone::unit::kelvin;
  static auto constexpr Lb  = 0.0065 * drone::unit::kelvin / drone::unit::meter;
  static auto constexpr Pb  = 1013.25 * 100.0 * drone::unit::pascal;
  static auto constexpr exp = 1.0 / 5.255;
  static auto constexpr fac = Tb / Lb;

  drone::unit::Length const altitude_m = fac * (1 - pow((pressure / Pb), exp));

  return altitude_m;
}

uint8_t ArduinoBMP388::getChipId(void)
{
  return _io.read(Register::CHIP_ID);
}

size_t ArduinoBMP388::printTo(Print & p) const
{
  size_t n = 0;
  n += drone::PressureSensorBase::printTo(p);
  n += drone::TemperatureSensorBase::printTo(p);
  return n;
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::readSensorData(double & pressure_hpa, double & temperature_deg)
{
  RawSensorData raw_data;
  _control.readRawData(raw_data);

  uint32_t const raw_pressure    = toRawPressure   (raw_data);
  uint32_t const raw_temperature = toRawTemperature(raw_data);

  pressure_hpa    = compensateRawPressure   (raw_pressure, temperature_deg, _quant_calib_data) / 100.0f;
  temperature_deg = compensateRawTemperature(raw_temperature, _quant_calib_data);
}
