#ifndef THRESHOLD_H
#include <stdio.h>

class Threshold
{
  int low_end, high_end;

  int switch_delay;
  unsigned long wait_start;
  bool waiting;

public:
  Threshold(int low, int high, int wait);
  void setup();
  bool loop(int lux, unsigned long now);
};
#endif