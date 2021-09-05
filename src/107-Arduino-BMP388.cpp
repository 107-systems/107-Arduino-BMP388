/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "107-Arduino-BMP388.h"

#include <math.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace drone
{

using namespace BMP388;

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoBMP388::ArduinoBMP388(SpiSelectFunc select,
                             SpiDeselectFunc deselect,
                             SpiTransferFunc transfer,
                             OnPressureDataUpdateFunc on_pressure_data_update,
                             OnTemperatureDataUpdateFunc on_temperature_data_update)
: PressureSensorBase("BMP388/pressure",
                     300.0  * unit::pascal,
                     1250.0 * unit::pascal,
                     0.0    * unit::hertz,
                     on_pressure_data_update)
, TemperatureSensorBase("BMP388/temperature",
                        233.15 * unit::kelvin, /* -40 °C */
                        358.15 * unit::kelvin, /* +85 °C */
                        0.0    * unit::hertz,
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
  setSensorUpdateRate(odr);

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

    _pressure = pressure_hpa * 100.0 * unit::pascal;
    _temperature = (temperature_deg + 273.15) * unit::kelvin;

    PressureSensorBase::onSensorValueUpdate(_pressure);
    TemperatureSensorBase::onSensorValueUpdate(_temperature);
  }
}

unit::Length ArduinoBMP388::convertPressureToAltitude(unit::Pressure const pressure)
{
  /* This formula assumes the international standard atmosphere (standard
   * temperature 15 °C = 288.15 K, static pressure = 1013.25 hPa, standard
   * temperature lapse rate 0.65 K / 100 m) and can be considered valid up
   * until 11 km.
   */
  static auto constexpr Tb  = 218.15 * unit::kelvin;
  static auto constexpr Lb  = 0.0065 * unit::kelvin / unit::meter;
  static auto constexpr Pb  = 1013.25 * 100.0 * unit::pascal;
  static auto constexpr exp = 1.0 / 5.255;
  static auto constexpr fac = Tb / Lb;

  unit::Length const altitude_m = fac * (1 - pow((pressure / Pb), exp));

  return altitude_m;
}

size_t ArduinoBMP388::printTo(Print & p) const
{
  size_t n = 0;
  n += PressureSensorBase::printTo(p);
  n += TemperatureSensorBase::printTo(p);
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

void ArduinoBMP388::setSensorUpdateRate(BMP388::OutputDataRate const odr)
{
  auto fn = [this](unit::Frequency const update_rate)
            {
              PressureSensorBase::setUpdateRate(update_rate);
              TemperatureSensorBase::setUpdateRate(update_rate);
            };

  switch(odr)
  {
    case BMP388::OutputDataRate::ODR_200_Hz    : fn(200.00   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_100_Hz    : fn(100.00   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_50_Hz     : fn( 50.00   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_25_Hz     : fn( 25.00   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_12_5_Hz   : fn( 12.50   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_6_25_Hz   : fn(  6.25   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_3_1_Hz    : fn(  3.10   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_1_5_Hz    : fn(  1.50   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_78_Hz   : fn(  0.78   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_39_Hz   : fn(  0.39   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_2_Hz    : fn(  0.20   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_1_Hz    : fn(  0.10   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_05_Hz   : fn(  0.05   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_02_Hz   : fn(  0.02   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_01_Hz   : fn(  0.01   * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_006_Hz  : fn(  0.006  * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_003_Hz  : fn(  0.003  * unit::hertz); break;
    case BMP388::OutputDataRate::ODR_0_0015_Hz : fn(  0.0015 * unit::hertz); break;
    default                                    : fn(  0.0000 * unit::hertz); break;
  }
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* drone */
