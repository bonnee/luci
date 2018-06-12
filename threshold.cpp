#include "threshold.h"

Threshold::Threshold(int low, int high, int wait)
{
	low_end = low;
	high_end = high;
	switch_delay = wait;
	waiting = false;
}

bool Threshold::loop(int lux, unsigned long now)
{
	static bool tog;

	if (!waiting)
	{
		// start the timer if outside range and wrong-switched
		if ((lux <= low_end && !tog) || (tog && high_end <= lux))
		{
			waiting = true;
			wait_start = now;
		}
	}
	// tofggle if timer expired
	if (waiting && (now - wait_start >= switch_delay))
	{
		waiting = false;

		if (lux <= low_end)
			tog = true;
		if (lux >= high_end)
			tog = false;
	}
	return tog;
}