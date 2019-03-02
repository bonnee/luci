#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <SparkFunTSL2561.h>

class Sensor
{
  const unsigned char time = 3;
  const boolean gain = 1;

  SFE_TSL2561 sensor;

  double lux;

  unsigned int wait_time;
  unsigned int int_time;

  boolean waiting = true;

  unsigned long int_start;
  unsigned long wait_start;

public:
  Sensor(unsigned int int_time, unsigned int wait_time) : int_time(int_time), wait_time(wait_time){};
  boolean setup();
  boolean loop();

  double getLux();
};
#endif
