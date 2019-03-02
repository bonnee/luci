#include "sensor.h"

int Sensor::setup()
{
  if (sensor.begin())
  {
    sensor.setTiming(gain, time, int_time);
    sensor.setPowerUp();
    return 1;
  }

  return 0;
}

boolean Sensor::loop()
{
  double tmp_lux = 0;
  boolean good = false;

  if (millis() - wait_start >= int_time)
  {
    sensor.manualStop();

    if (sensor.getData(data0, data1))
    {
      good = sensor.getLux(gain, int_time, data0, data1, tmp_lux);
    }

    wait_start = millis();
    sensor.manualStart();
  }

  if (good)
  {
    lux = tmp_lux;
    return true;
  }

  return false;
}

double Sensor::getLux()
{
  return lux;
}
