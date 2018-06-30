#include "sensor.h"

Sensor::Sensor(int read_interval)
{
  interval = read_interval;
}

int Sensor::setup()
{
  if (sensor.begin())
  {
    sensor.setGain(TSL2561_GAIN_16X);
    sensor.setTiming(TSL2561_INTEGRATIONTIME_13MS);
    return 0;
  }

  return -1;
}

void Sensor::loop()
{
  if (millis() - wait_start >= interval)
  {
    wait_start = millis();
    lux = sensor.getLuminosity(TSL2561_VISIBLE);
  }
}

int Sensor::get_lux()
{
  return lux;
}