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

#include "sensor.h"
#include <RS485Serial.h>

#define DEBUG(x) Serial.print(x);

#define RX 2
#define TX 3
#define TALK 4

// Time to wait between light readings
#define READ_INT 1000

Sensor sensor(READ_INT);
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
    DEBUG("ERROR.");
    while (1)
      ;
  }

  DEBUG("Done.\n");
  digitalWrite(13, LOW);
}

void loop()
{
  if (sensor.loop())
  {
    unsigned int lux = sensor.get_lux();

    DEBUG(lux);
    DEBUG('\n');

    sserial.sendLux(lux);
  }
}
