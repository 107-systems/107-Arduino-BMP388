/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

#ifndef ARDUINO_BMP388_BMP388_CONTROL_H_
#define ARDUINO_BMP388_BMP388_CONTROL_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388_Io.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class BMP388_Control
{
public:

  BMP388_Control(BMP388_Io & io);

  void reset        ();
  void readCalibData(CalibrationData & calib_data);
  void readRawData  (RawSensorData & data);


private:

  BMP388_Io & _io;

};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_CONTROL_H_ */
