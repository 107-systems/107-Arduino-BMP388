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
                             SpiTransferFunc transfer)
: _io{select, deselect, transfer}
, _config{_io}
, _control{_io}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::begin()
{
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
  /* TODO - convert to physical units */
}
