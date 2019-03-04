#include "threshold.h"

void Threshold::loop(unsigned int lux)
{
  unsigned long now = millis();

  if (!waiting)
  {
    // start the timer if outside range and wrong-switched
    if ((lux <= low && !toggle) || (toggle && high <= lux))
    {
      waiting = true;
      prev_time = now;
    }
  }
}

boolean Threshold::toggled(unsigned int lux)
{
  unsigned long now = millis();

  // toggle if timer expired
  if (waiting && (now - prev_time >= wait))
  {
    waiting = false;

    if (lux <= low)
      toggle = true;
    if (lux >= high)
      toggle = false;
  }

  return toggle;
}