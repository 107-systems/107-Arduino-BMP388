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

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static int const BMP388_CS_PIN  = 2;
static int const BMP388_INT_PIN = 6;

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

};

#endif /* ARDUINO_BMP388_H_ */
