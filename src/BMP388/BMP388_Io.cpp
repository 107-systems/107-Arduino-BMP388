/**
 * This software is distributed under the terms of the MIT License.
 * Copyright (c) 2020 LXRobotics.
 * Author: Alexander Entinger <alexander.entinger@lxrobotics.com>
 * Contributors: https://github.com/107-systems/107-Arduino-BMP388/graphs/contributors.
 */

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
 * CTOR/DTOR
 **************************************************************************************/

BMP388_Io::BMP388_Io(SpiSelectFunc select, SpiDeselectFunc deselect, SpiTransferFunc transfer)
: _select{select}
, _deselect{deselect}
, _transfer{transfer}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

uint8_t BMP388_Io::read(Register const reg)
{
  uint8_t const reg_addr = 0x80 | to_integer(reg);

  _select();
                          _transfer(reg_addr);
                          _transfer(0); /* Dummy Byte */
  uint8_t const reg_val = _transfer(0);
  _deselect();
  return reg_val;
}

void BMP388_Io::read(Register const reg, uint8_t * data, uint8_t const len)
{
  uint8_t const reg_addr = 0x80 | to_integer(reg);

  _select();
  _transfer(reg_addr);
  _transfer(0); /* Dummy Byte */
  for(uint8_t b = 0; b < len; b++) {
    data[b] = _transfer(0);
  }
  _deselect();
}

void BMP388_Io::write(Register const reg, uint8_t const val)
{
  uint8_t const reg_addr = to_integer(reg);

  _select();
  _transfer(reg_addr);
  _transfer(val);
  _deselect();
}

void BMP388_Io::modify(Register const reg, uint8_t const bitmask, uint8_t const val)
{
  uint8_t reg_val = read(reg);
  reg_val &= ~(bitmask);
  reg_val |= (val & bitmask);
  write(reg, reg_val);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */
