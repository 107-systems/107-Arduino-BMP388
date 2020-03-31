/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "ArduinoBMP388.h"

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
                             OnSensorDataFunc on_sensor_data)
: _io{select, deselect, transfer}
, _config{_io}
, _control{_io}
, _on_sensor_data{on_sensor_data}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::begin()
{
  _control.reset();

  _config.configPowerMode(PowerMode::Normal);

  _config.configIntPinOutputType(IntPinOutputType::OpenDrain);
  _config.configIntPinLevel(IntPinLevel::ActiveLow);
  _config.enableDataReadyInt();

  _config.enablePressure();
  _config.enableTemperature();
}

void ArduinoBMP388::onExternalEventHandler()
{
  RawSensorData raw_data;
  _control.readRawData(raw_data);

  uint32_t const raw_pressure    = toRawPressure   (raw_data);
  uint32_t const raw_temperature = toRawTemperature(raw_data);

  /* TODO - convert to physical units */
  float const pressure_hpa = 0.0f;
  float const temperature_deg = 0.0f;

  if(_on_sensor_data) {
    _on_sensor_data(pressure_hpa, temperature_deg);
  }
}
