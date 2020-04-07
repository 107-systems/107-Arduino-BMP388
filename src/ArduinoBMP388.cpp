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
                             SpiTransferFunc transfer,
                             OnSensorDataFunc on_sensor_data)
: _io{select, deselect, transfer}
, _config{_io}
, _control{_io}
, _on_sensor_data{on_sensor_data}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::begin(OutputDataRate const odr)
{
  _control.reset();

  BMP388::CalibrationData calib_data;
  _control.readCalibData(calib_data);
  _quant_calib_data = toQuantizedCalibrationData(calib_data);

  _config.configPressureOversampling(PressureOversampling::x32);
  _config.configTemperatureOversampling(TemperatureOversampling::x2);
  _config.configOutputDataRate(odr);

  _config.configIntPinOutputType(IntPinOutputType::OpenDrain);
  _config.configIntPinLevel(IntPinLevel::ActiveLow);
  _config.enableDataReadyInt();

  _config.enablePressure();
  _config.enableTemperature();

  _config.configPowerMode(PowerMode::Normal);
}

void ArduinoBMP388::onExternalEventHandler()
{
  uint8_t const int_status = _io.read(Register::INT_STATUS_REG);

  if(int_status & bm(INT_STATUS::DRDY)) {
    readSensorData();
  }
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoBMP388::readSensorData()
{
  RawSensorData raw_data;
  _control.readRawData(raw_data);

  uint32_t const raw_temperature = toRawTemperature(raw_data);
  uint32_t const raw_pressure    = toRawPressure   (raw_data);

  double const temperature_deg = compensateRawTemperature(raw_temperature, _quant_calib_data);
  double const pressure_hpa    = compensateRawPressure   (raw_pressure, temperature_deg, _quant_calib_data) / 100.0f;

  _on_sensor_data(pressure_hpa, temperature_deg);
}
