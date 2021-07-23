/**
 * @brief Basic example demonstrating usage of 107-Arduino-BMP388 library.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <SPI.h>

#include <ArduinoBMP388.h>

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static int const BMP388_CS_PIN  = 2;
static int const BMP388_INT_PIN = 6;

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

void onSensorData(double const pressure_hpa, double const temperature_deg);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoBMP388 bmp388([](){ digitalWrite(BMP388_CS_PIN, LOW); },
                     [](){ digitalWrite(BMP388_CS_PIN, HIGH); },
                     [](uint8_t const d) -> uint8_t { return SPI.transfer(d); },
                     onSensorData);

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(9600);
  while(!Serial) { }

  /* Setup SPI access */
  SPI.begin();
  pinMode(BMP388_CS_PIN, OUTPUT);
  digitalWrite(BMP388_CS_PIN, HIGH);

  /* Attach interrupt handler */
  pinMode(BMP388_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BMP388_INT_PIN), [](){ bmp388.onExternalEventHandler(); }, FALLING);

  /* Configure BMP388 */
  bmp388.begin(BMP388::OutputDataRate::ODR_12_5_Hz);
}

void loop()
{

}

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void onSensorData(double const pressure_hpa, double const temperature_deg)
{
  Serial.print(pressure_hpa);
  Serial.print(" hPa / ");
  Serial.print(temperature_deg);
  Serial.print(" °C / ");
  Serial.print(ArduinoBMP388::convertPressureToAltitude(pressure_hpa));
  Serial.println(" m");
}
