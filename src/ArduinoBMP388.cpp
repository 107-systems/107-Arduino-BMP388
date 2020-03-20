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
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::begin()
{
  _config.configIntPinOutputType(IntPinOutputType::OpenDrain);
  _config.configIntPinLevel(IntPinLevel::ActiveLow);
}

void ArduinoBMP388::onExternalEventHandler()
{

}
