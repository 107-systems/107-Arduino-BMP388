/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
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
