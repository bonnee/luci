#include "threshold.h"

void Threshold::loop(int lux)
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

  // toggle if timer expired
  if (waiting && (now - prev_time >= delay))
  {
    waiting = false;

    if (lux <= low)
      toggle = true;
    if (lux >= high)
      toggle = false;
  }
}

bool Threshold::toggled()
{
  return toggle;
}