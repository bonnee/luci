#include "sensor.h"

Sensor::Sensor(int read_interval)
{
  interval = read_interval;
}

void Sensor::setup()
{
  if (sensor.begin())
  {
    sensor.setGain(TSL2561_GAIN_16X);
    sensor.setTiming(TSL2561_INTEGRATIONTIME_13MS);
  }
  else
  {
    exit(1);
  }
}

int Sensor::loop(unsigned long now)
{
  if (now - wait_start >= interval)
  {
    wait_start = now;
    return (int)sensor.getLuminosity(TSL2561_VISIBLE);
  }
  return -1;
}