#ifndef SENSOR_H
#include <stdio.h>
#include <Arduino.h>
#include "TSL2561.h"

class Sensor
{
  TSL2561 sensor = TSL2561(TSL2561_ADDR_FLOAT);

  int lux;

  unsigned int interval;
  unsigned long wait_start;
  bool waiting;

public:
  Sensor(int read_interval);
  int setup();
  void loop();

  int get_lux();
};
#endif