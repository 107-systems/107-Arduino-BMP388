/**
 * @brief BMP388 self-test using 107-Arduino-BMP388 library.
 * 
 * according to application note https://www.bosch-sensortec.com/media/boschsensortec/downloads/application_notes_1/bst-mps-an006.pdf
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
void    onExternalEvent();
void    onSensorData   (double const pressure_hpa, double const temperature_deg);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoBMP388 bmp388(spi_select,
                     spi_deselect,
                     spi_transfer,
                     onSensorData);
volatile bool measurement_done;
volatile double measurement_pressure_hpa;
volatile double measurement_temperature_deg;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(9600);
  while(!Serial) { }

  /* Setup SPI access */
  Serial.println("Setup SPI access on Arduino");  
  SPI.begin();
  pinMode(BMP388_CS_PIN, OUTPUT);
  digitalWrite(BMP388_CS_PIN, HIGH);
  Serial.println("Passed!");  

  /* Attach interrupt handler */
  Serial.println("Setup interrupt hanlder on Arduino");  
  pinMode(BMP388_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BMP388_INT_PIN), onExternalEvent, FALLING);
  Serial.println("Passed!");  

  /* Configure BMP388 */
  Serial.println("Initialize BMP388");  
  bmp388.begin(BMP388::OutputDataRate::ODR_12_5_Hz);
  Serial.println("Passed!");  
}

void loop()
{
  uint8_t chip_id;
  /* Get BMP388 Chip ID*/
  Serial.println("Verify Chip-ID");

  chip_id=bmp388.getChipId();
  Serial.print("Chip-ID: 0x");
  Serial.println(chip_id, HEX);
  if(chip_id==0x50) Serial.println("Passed!");  
  else Serial.println("Failed!");  

  Serial.println("Verify trimming data");
  /* not possible now */

  Serial.println("measure temperature and pressure");
  measurement_done=0;
  while(measurement_done==0);
  Serial.println("Passed!");  

  Serial.println("check measurement plausability");
  Serial.print("temperature:");
  Serial.println(measurement_temperature_deg);
  if((measurement_temperature_deg>0)&&(measurement_temperature_deg<40)) Serial.println("Passed!");  
  else Serial.println("Failed!");  
  Serial.print("pressure:");
  Serial.println(measurement_pressure_hpa);
  if((measurement_pressure_hpa>900)&&(measurement_pressure_hpa<1100)) Serial.println("Passed!");  
  else Serial.println("Failed!");  

  Serial.println("self-test finished");
  Serial.println();
  Serial.println();
  delay(3000);
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

void onSensorData(double const pressure_hpa, double const temperature_deg)
{
//  Serial.print(pressure_hpa);
//  Serial.print(" hPa / ");
//  Serial.print(temperature_deg);
//  Serial.println(" °C");
  measurement_pressure_hpa=pressure_hpa;
  measurement_temperature_deg=temperature_deg;
  measurement_done=1;
}
