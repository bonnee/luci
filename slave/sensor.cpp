#include "sensor.h"

boolean Sensor::setup()
{
  Serial.println(int_time);
  if (sensor.begin())
  {
    unsigned int t;
    sensor.setTiming(gain, time, t);
    sensor.setPowerUp();
    return true;
  }

  return false;
}

boolean Sensor::loop()
{
  double tmp_lux;
  boolean good = false;

  if (waiting && millis() - wait_start >= wait_time) {
    waiting = false;
    
    int_start = millis();
    sensor.manualStart();
  }

  if (!waiting && millis() - int_start >= int_time)
  {
    sensor.manualStop();
    unsigned int real_time=millis() - int_start;

    unsigned int data0, data1;

    if (sensor.getData(data0, data1))
    {
      Serial.print(data0);
      Serial.print('\t');
      Serial.println(data1);
      
      good = sensor.getLux(gain, real_time, data0, data1, tmp_lux);

    } else {
      byte error = sensor.getError();
    }

    waiting = true;
    wait_start = millis();

    if (good)
    {
      lux = tmp_lux;
      return true;
    }
  }



  return false;
}

double Sensor::getLux()
{
  return lux;
}
