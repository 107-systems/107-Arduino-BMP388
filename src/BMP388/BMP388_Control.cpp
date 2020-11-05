/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Control.h"

#include <Arduino.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

BMP388_Control::BMP388_Control(BMP388_Io & io)
: _io{io}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void BMP388_Control::reset()
{
  _io.write(Register::CMD, to_integer(CMD::SOFT_RESET));
  delay(10);
}

void BMP388_Control::readCalibData(CalibrationData & calib_data)
{
  _io.read(Register::CALIB_DATA, calib_data.buf, sizeof(calib_data.buf));
}

void BMP388_Control::readRawData(RawSensorData & data)
{
  _io.read(Register::DATA, data.buf, sizeof(data.buf));
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
