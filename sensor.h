#ifndef SENSOR_H
#include <stdio.h>
#include "TSL2561.h"

class Sensor
{
  int read_int;
  TSL2561 tsl = TSL2561(TSL2561_ADDR_FLOAT);

  unsigned long wait_start;
  bool waiting;

public:
  Sensor(int interval);
  void setup();
  uint16_t loop(unsigned long now);
};
#endif