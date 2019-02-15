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

#define DEBUG(x)         \
	if (Serial)          \
	{                    \
		Serial.print(x); \
	}

#define RX 2
#define TX 3
#define TALK 4

#define BAUD 9600

// Time to wait between light readings
#define READ_INT 1000

int last_lux = 0;

Sensor sensor(READ_INT);
RS485Serial sserial(RX, TX, TALK);

void setup()
{
	// Debug serial
	Serial.begin(9600);

	while (!Serial)
	{
	}

	DEBUG("SLAVE\n");
	DEBUG("I/O...");
	sserial.begin(BAUD);

	// UNCOMMENT
	/*DEBUG("Sensor...");
	if (sensor.setup() != 0)
	{
		DEBUG("ERROR.");
		while (1)
			;
	}*/

	DEBUG("Done.\n");
	digitalWrite(13, LOW);
}

void loop()
{
	// UNCOMMENT
	//sensor.loop();
	int val = 768;

	DEBUG("sending ");
	DEBUG(val);
	DEBUG('\n');

	sserial.print(val);

	delay(500);

	// UNCOMMENT
	/*if (sensor.get_lux() >= 0)
	{
		int lux = sensor.get_lux();

		if (last_lux != lux)
		{
			sserial.print(lux);
			last_lux = lux;
			DEBUG(lux);
		}
	}*/
}
