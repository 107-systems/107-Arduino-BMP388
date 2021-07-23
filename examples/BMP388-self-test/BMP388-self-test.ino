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

void onSensorData(double const pressure_hpa, double const temperature_deg);
void runSelfTest ();

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

ArduinoBMP388 bmp388([](){ digitalWrite(BMP388_CS_PIN, LOW); },
                     [](){ digitalWrite(BMP388_CS_PIN, HIGH); },
                     [](uint8_t const d) -> uint8_t { return SPI.transfer(d); },
                     onSensorData);

volatile bool   measurement_done = false;
volatile double measurement_pressure_hpa = 0.0;
volatile double measurement_temperature_deg = 0.0;

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
  Serial.println("Setup interrupt handler on Arduino");
  pinMode(BMP388_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BMP388_INT_PIN), [](){ bmp388.onExternalEventHandler(); }, FALLING);
  Serial.println("Passed!");

  /* Configure BMP388 */
  Serial.println("Initialize BMP388");
  bmp388.begin(BMP388::OutputDataRate::ODR_12_5_Hz);
  Serial.println("Passed!");
}

void loop()
{
  static bool self_test_run = false;
  if (!self_test_run)
  {
    runSelfTest();
    self_test_run = true;
  }
}

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

void onSensorData(double const pressure_hpa, double const temperature_deg)
{
  measurement_pressure_hpa = pressure_hpa;
  measurement_temperature_deg = temperature_deg;
  measurement_done = true;
}

void runSelfTest()
{
  /* Get BMP388 Chip ID*/
  Serial.println("Verify Chip-ID");

  uint8_t chip_id=bmp388.getChipId();
  Serial.print("Chip-ID: 0x");
  Serial.println(chip_id, HEX);
  if(chip_id == 0x50)
    Serial.println("Passed!");
  else
    Serial.println("Failed!");

  Serial.println("Verify trimming data");
  /* not possible now */

  Serial.println("Measure temperature and pressure");
  measurement_done = false;
  while(!measurement_done) { };
  Serial.println("Passed!");

  Serial.println("Check measurement plausibility");
  Serial.print("Temperature:");
  Serial.println(measurement_temperature_deg);
  if ((measurement_temperature_deg > 0.0) &&
      (measurement_temperature_deg < 40.0))
    Serial.println("Passed!");
  else
    Serial.println("Failed!");

  Serial.print("Pressure:");
  Serial.println(measurement_pressure_hpa);
  if ((measurement_pressure_hpa > 900.0) &&
      (measurement_pressure_hpa < 1100.0))
    Serial.println("Passed!");
  else
    Serial.println("Failed!");

  Serial.println("Self-test finished");
}
