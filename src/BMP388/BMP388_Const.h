/**
 * @brief Arduino library for interfacing with the BMP388 barometric pressure sensor.
 * @license LGPL 3.0
 */

#ifndef ARDUINO_BMP388_BMP388_CONST_H_
#define ARDUINO_BMP388_BMP388_CONST_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <stdint.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace BMP388
{

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

enum class Register : uint8_t
{
  CHIP_ID         = 0x00,
  ERR_REG         = 0x02,
  SENS_STATUS_REG = 0x03,
  DATA            = 0x04,
  EVENT           = 0x10,
  INT_STATUS_REG  = 0x11,
  FIFO_LENGTH     = 0x12,
  FIFO_DATA       = 0x14,
  FIFO_WM         = 0x15,
  FIFO_CONFIG_1   = 0x17,
  FIFO_CONFIG_2   = 0x18,
  INT_CTRL        = 0x19,
  IF_CONF         = 0x1A,
  PWR_CTRL        = 0x1B,
  OSR             = 0X1C,
  ODR             = 0X1D,
  CALIB_DATA      = 0x31,
  CMD             = 0x7E,
};

enum class INT_CTRL : uint8_t
{
  DRDY_EN   = 6,
  INT_LEVEL = 1,
  INT_OD    = 0
};

enum class INT_STATUS : uint8_t
{
  DRDY = 3
};

enum class PWR_CTRL : uint8_t
{
  MODE_1   = 5,
  MODE_0   = 4,
  TEMP_EN  = 1,
  PRESS_EN = 0
};

enum class CMD : uint8_t
{
  SOFT_RESET = 0xB6
};

enum class OSR : uint8_t
{
  OSR_T_2 = 5,
  OSR_T_1 = 4,
  OSR_T_0 = 3,
  OSR_P_2 = 2,
  OSR_P_1 = 1,
  OSR_P_0 = 0
};

union CalibrationData
{
  struct __attribute__((packed))
  {
    uint16_t T1;
    uint16_t T2;
    int8_t   T3;
    int16_t  P1;
    int16_t  P2;
    int8_t   P3;
    int8_t   P4;
    uint16_t P5;
    uint16_t P6;
    int8_t   P7;
    int8_t   P8;
    int16_t  P9;
    int8_t   P10;
    int8_t   P11;
  } coefficient;
  uint8_t buf[21];
};

union RawSensorData
{
  struct __attribute__((packed))
  {
    uint8_t pres_msb;
    uint8_t pres_lsb;
    uint8_t pres_xlsb;
    uint8_t temp_msb;
    uint8_t temp_lsb;
    uint8_t temp_xlsb;
  } reg;
  uint8_t buf[6];
};

/**************************************************************************************
 * CONVERSION FUNCTIONS
 **************************************************************************************/

template <typename Enumeration>
constexpr auto to_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

template <typename Enumeration>
constexpr auto bp(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
  return to_integer(value);
}

template <typename Enumeration>
constexpr auto bm(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return (1 << to_integer(value));
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* BMP388 */

#endif /* ARDUINO_BMP388_BMP388_CONST_H_ */
