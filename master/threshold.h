#ifndef THRESHOLD_H
#define THRESHOLD_H

#include <Arduino.h>

class Threshold
{
  int low, high;
  bool toggle = false;

  unsigned long wait;
  unsigned long prev_time;
  boolean waiting = false;

public:
  Threshold(unsigned int low, unsigned int high, unsigned long wait) : low(low), high(high), wait(wait) {}
  void loop(unsigned int lux);
  boolean Threshold::toggled(unsigned int lux);
};
#endif