/**
 * @brief Demonstrate how to directly interface with the BMP388 using of 107-Arduino-BMP388 library.
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
 * NAMESPACE
 **************************************************************************************/

using namespace drone;

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoBMP388 bmp388([](){ digitalWrite(BMP388_CS_PIN, LOW); },
                     [](){ digitalWrite(BMP388_CS_PIN, HIGH); },
                     [](uint8_t const d) -> uint8_t { return SPI.transfer(d); },
                     nullptr,
                     nullptr);

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

  /* REead BMP388 Chip ID*/
  Serial.print("Chip-ID: 0x");
  Serial.print(bmp388.io().read(BMP388::Register::CHIP_ID), HEX);
  Serial.print(" (expected: 0x50)");
  Serial.println();
}

void loop()
{

}
