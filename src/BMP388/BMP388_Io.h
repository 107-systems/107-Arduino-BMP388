/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

#ifndef ARDUINO_BMP388_BMP388_IO_H_
#define ARDUINO_BMP388_BMP388_IO_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#undef max
#undef min
#include <functional>

#include "BMP388_Const.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef std::function<void()>                 SpiSelectFunc;
typedef std::function<void()>                 SpiDeselectFunc;
typedef std::function<uint8_t(uint8_t const)> SpiTransferFunc;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class BMP388_Io
{
public:

  BMP388_Io(SpiSelectFunc select, SpiDeselectFunc deselect, SpiTransferFunc transfer);


  uint8_t read  (Register const reg);
  void    read  (Register const reg, uint8_t * data, uint8_t const len);
  void    write (Register const reg, uint8_t const val);
  void    modify(Register const reg, uint8_t const bitmask, uint8_t const val);


private:

  SpiSelectFunc _select;
  SpiDeselectFunc _deselect;
  SpiTransferFunc _transfer;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_IO_H_ */
