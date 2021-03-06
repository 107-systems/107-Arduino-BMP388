`107-Arduino-BMP388`
====================
[![Compile Examples](https://github.com/107-systems/107-Arduino-BMP388/workflows/Compile%20Examples/badge.svg)](https://github.com/107-systems/107-Arduino-BMP388/actions?workflow=Compile+Examples)
[![Extra Library Checks](https://github.com/107-systems/107-Arduino-BMP388/workflows/Extra%20Library%20Checks/badge.svg)](https://github.com/107-systems/107-Arduino-BMP388/actions?workflow=Extra+Library+Checks)
[![General Formatting Checks](https://github.com/107-systems/107-Arduino-BMP388/workflows/General%20Formatting%20Checks/badge.svg)](https://github.com/107-systems/107-Arduino-BMP388/actions?workflow=General+Formatting+Checks)
[![Spell Check](https://github.com/107-systems/107-Arduino-BMP388/workflows/Spell%20Check/badge.svg)](https://github.com/107-systems/107-Arduino-BMP388/actions?workflow=Spell+Check)

<p align="center">
  <a href="https://github.com/107-systems/Viper-Firmware"><img src="https://github.com/107-systems/.github/raw/main/logo/viper.jpg" width="40%"></a>
</p>

Arduino library for interfacing with the BMP388 barometric pressure sensor which can be used for inferring altitude information.

## Example
```C++
#include <SPI.h>
#include <ArduinoBMP388.h>
/* ... */
static int const BMP388_CS_PIN  = 2;
static int const BMP388_INT_PIN = 6;
/* ... */
void    spi_select     ()                   { digitalWrite(BMP388_CS_PIN, LOW); }
void    spi_deselect   ()                   { digitalWrite(BMP388_CS_PIN, HIGH); }
uint8_t spi_transfer   (uint8_t const data) { return SPI.transfer(data); }
void    onExternalEvent()                   { bmp388.onExternalEventHandler(); }
void    onSensorData   (double const pressure_hpa, double const temperature_deg) {
  Serial.print(pressure_hpa);
  Serial.print(" hPa / ");
  Serial.print(temperature_deg);
  Serial.print(" °C / ");
  Serial.print(ArduinoBMP388::convertPressureToAltitude(pressure_hpa));
  Serial.println(" m");
}
/* ... */
ArduinoBMP388 bmp388(spi_select, spi_deselect, spi_transfer, onSensorData);
/* ... */
void setup() {
  Serial.begin(9600);
  while(!Serial) { }

  SPI.begin();
  pinMode(BMP388_CS_PIN, OUTPUT);
  digitalWrite(BMP388_CS_PIN, HIGH);

  pinMode(BMP388_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BMP388_INT_PIN), onExternalEvent, FALLING);

  bmp388.begin(BMP388::OutputDataRate::ODR_12_5_Hz);
}

void loop() {

}
```
