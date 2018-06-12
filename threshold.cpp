#include "threshold.h"

Threshold::Threshold(int low, int high, int wait)
{
  low_end = low;
  high_end = high;
  switch_delay = wait;
  waiting = false;
  toggle = false;
}

bool Threshold::loop(int lux, unsigned long now)
{
  if (!waiting)
  {
    // start the timer if outside range and wrong-switched
    if ((lux <= low_end && !toggle) || (toggle && high_end <= lux))
    {
      //Serial.println("Starting timer.");
      waiting = true;
      wait_start = now;
    }
  }
  // tofggle if timer expired
  if (waiting && (now - wait_start >= switch_delay))
  {
    waiting = false;

    if (lux <= low_end)
      toggle = true;
    if (lux >= high_end)
      toggle = false;
  }
  return toggle;
}