#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "TSL2561.h"

class Sensor
{
  TSL2561 sensor = TSL2561(TSL2561_ADDR_FLOAT);

  unsigned int lux;

  unsigned int interval;
  unsigned long wait_start;
  bool waiting;

public:
  Sensor(unsigned int read_interval);
  int setup();
  bool loop();

  unsigned int get_lux();
};
#endif
