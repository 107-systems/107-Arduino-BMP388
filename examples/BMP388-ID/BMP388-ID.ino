/**
 * @brief Get Chip-ID of BMP388 using of 107-Arduino-BMP388 library.
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

void    spi_select     ();
void    spi_deselect   ();
uint8_t spi_transfer   (uint8_t const);
void    onSensorData   (double const pressure_hpa, double const temperature_deg);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoBMP388 bmp388(spi_select,
                     spi_deselect,
                     spi_transfer,
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

  /* Get BMP388 Chip ID*/
  Serial.println("The Chip-ID should be 0x50!");

  uint8_t chip_id=bmp388.getChipId();
  Serial.print("Chip-ID: 0x");
  Serial.println(chip_id, HEX);
}

void loop()
{

}

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void spi_select()
{
  digitalWrite(BMP388_CS_PIN, LOW);
}

void spi_deselect()
{
  digitalWrite(BMP388_CS_PIN, HIGH);
}

uint8_t spi_transfer(uint8_t const data)
{
  return SPI.transfer(data);
}

void onSensorData(double const pressure_hpa, double const temperature_deg)
{
}
