`107-Arduino-BMP388`
====================
[![Build Status](https://travis-ci.org/107-systems/107-Arduino-BMP388.svg?branch=master)](https://travis-ci.org/107-systems/107-Arduino-BMP388)

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
