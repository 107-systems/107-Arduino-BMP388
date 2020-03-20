/**
 * @brief Basic example demonstrating usage of 107-Arduino-BMP388 library.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <SPI.h>

#include <ArduinoBMP388.h>

/**************************************************************************************
 * FUNCTION DECLARATION
 **************************************************************************************/

void    spi_select     ();
void    spi_deselect   ();
uint8_t spi_transfer   (uint8_t const);
void    onExternalEvent();

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoBMP388 bmp388(spi_select,
                     spi_deselect,
                     spi_transfer);

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
  attachInterrupt(digitalPinToInterrupt(BMP388_INT_PIN), onExternalEvent, FALLING);


  bmp388.begin();
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

void onExternalEvent()
{
  bmp388.onExternalEventHandler();
}