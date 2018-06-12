#ifndef SENSOR_H
#include <stdio.h>
#include "TSL2561.h"

class Sensor
{
  TSL2561 sensor = TSL2561(TSL2561_ADDR_FLOAT);

  int read_int;
  unsigned long wait_start;
  bool waiting;

public:
  Sensor(int interval);
  void setup();
  int loop(unsigned long now);
};
#endif