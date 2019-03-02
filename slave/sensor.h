#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <SparkFunTSL2561.h>

class Sensor
{
  const unsigned char time = 3;
  const boolean gain = true;

  SFE_TSL2561 sensor;

  unsigned int data0, data1;

  double lux;

  unsigned int int_time;
  unsigned long wait_start;

public:
  Sensor(unsigned int int_time) : int_time(int_time){};
  int setup();
  boolean loop();

  double getLux();
};
#endif
