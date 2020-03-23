/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
 */

#ifndef ARDUINO_BMP388_H_
#define ARDUINO_BMP388_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "BMP388/BMP388_Io.h"
#include "BMP388/BMP388_Config.h"
#include "BMP388/BMP388_Control.h"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoBMP388
{
public:

  ArduinoBMP388(BMP388::SpiSelectFunc select,
                BMP388::SpiDeselectFunc deselect,
                BMP388::SpiTransferFunc transfer);


  void begin();


  void onExternalEventHandler();


private:

  BMP388::BMP388_Io _io;
  BMP388::BMP388_Config _config;
  BMP388::BMP388_Control _control;

};

#endif /* ARDUINO_BMP388_H_ */
