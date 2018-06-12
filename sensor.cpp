#include "sensor.h"

Sensor::Sensor(int interval)
{
  read_int = interval;
}

void Sensor::setup()
{
  if (tsl.begin())
  {
    tsl.setGain(TSL2561_GAIN_16X);
    tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);
  }
  else
  {
    exit(1);
  }
}

uint16_t Sensor::loop(unsigned long now)
{
  if (now - wait_start >= read_int)
  {
    wait_start = now;
    return tsl.getLuminosity(TSL2561_VISIBLE);
  }
  return -1;
}