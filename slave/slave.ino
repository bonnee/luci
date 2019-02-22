/**
 *	luci: Internal and external lights control with relay output and TSL2561
 *	digital light sensor
 *
 *	slave.ino: Sensor-side control. This file reads the temperature from the
 *	sensor and sends it through SoftwareSerial.
 * 
 *	Pinout of the TSL2561 sensor:
 *		VCC: 3.3v   White/Blue Pin
 *      GND: Ground Blue Pin
 *      SCL: A5     White/Green Pin
 *      SDA: A4     Green Pin
 * 
*/

#include "sensor.h"
#include <RS485Serial.h>

#define DEBUG(x) Serial.print(x);

#define RX 2
#define TX 3
#define TALK 4

// Time to wait between light readings
#define READ_INT 1000

unsigned int last_lux = 0;

Sensor sensor(READ_INT);
RS485Serial sserial(RX, TX, TALK);

void setup()
{
	Serial.begin(sserial.getBaud());

	DEBUG("SLAVE\n");
	DEBUG("I/O...");
	sserial.begin();

	DEBUG("Sensor...");
	if (sensor.setup() != 0)
	{
		DEBUG("ERROR.");
		while (1)
			;
	}

	DEBUG("Done.\n");
	digitalWrite(13, LOW);
}

void loop()
{
	if (sensor.loop()) // dummyLoop()
	{
		unsigned int lux = sensor.get_lux(); //dummy_get_lux();
		if (last_lux != lux)
		{
			sserial.sendLux(lux);
			last_lux = lux;
			DEBUG(lux);
			DEBUG('\n');
		}
	}
}

unsigned long wait_start = 0;
bool dummyLoop()
{
	if (millis() - wait_start >= 1000)
	{
		wait_start = millis();
		return true;
	}
	return false;
}

unsigned int dummy_get_lux()
{
	return random(10, 65535);
}
