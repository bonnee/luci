/**
 * luci: Internal and external lights control with relay output and TSL2561
 * digital light sensor
 * 
 * slave.ino: Sensor-side control. This file reads the temperature from the
 * sensor and sends it through SoftwareSerial.
 * 
 * Pinout of the TSL2561 sensor:
 *  VCC: 3.3v   White/Blue
 *  GND: Ground Blue
 *  SCL: A5     White/Green
 *  SDA: A4     Green
 * 
 * Pinout of the serial cable
 *  VCC: 5v White/Brown
 *  GND: Brown
 *  A:   Orange
 *  B:   White/Orange
 * 
*/

#include <EEPROMWearLevel.h>
#include "sensor.h"
#include <RS485Serial.h>

#define DEBUG(x) Serial.print(x);

#define EEPROM_LAYOUT_VERSION 0
#define AMOUNT_OF_INDEXES 2

#define RX 2
#define TX 3
#define TALK 4

// Integration time of the sensor
#define LIGHT_INT 1200 //1500
#define LIGHT_DELAY 500

#define MAX_LUX 65534

Sensor sensor(LIGHT_INT, LIGHT_DELAY);
RS485Serial sserial(RX, TX, TALK);

void setup()
{
  Serial.begin(sserial.getBaud());

  DEBUG("SLAVE\n");
  DEBUG("I/O...");
  sserial.begin();

  DEBUG("Sensor...");
  if (!sensor.setup())
  {
    while (1)
    {
      DEBUG("SENSOR ERROR\n");
    }
  }

  EEPROMwl.begin(EEPROM_LAYOUT_VERSION, 1);

  DEBUG("Done.\n");
  digitalWrite(13, LOW);
}

void loop()
{
  unsigned int status = sensor.loop();
  unsigned int lux;

  switch (status)
  {
  case true:

    lux = round(sensor.getLux());
    EEPROMwl.put(0, lux);
    sserial.sendLux(lux);

    DEBUG(lux);
    DEBUG('\n');

    break;
  case 2:
    //lux = EEPROMwl.get(0, lux);
    lux = MAX_LUX;
    sserial.sendLux(lux);

    //DEBUG("EEPROM LOAD ")
    DEBUG(lux);
    DEBUG('\n');

    break;
  }
}
