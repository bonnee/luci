#include "RS485Serial.h"

void RS485Serial::begin(int baud_rate)
{
	ndx = 0;

	pinMode(talk_pin, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	sserial.begin(baud_rate);
}

int RS485Serial::print(unsigned int value)
{
	digitalWrite(LED_BUILTIN, HIGH);
	digitalWrite(talk_pin, HIGH);

	sserial.println(value, DEC);
	sserial.flush();

	digitalWrite(talk_pin, LOW);
	digitalWrite(LED_BUILTIN, LOW);

	return 0;
}

unsigned int RS485Serial::loop()
{
	if (sserial.available() > 0)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		char rc = sserial.read();
		digitalWrite(LED_BUILTIN, LOW);

		if (rc != endMarker)
		{
			receivedChars[ndx] = rc;
			ndx++;

			if (ndx >= numChars)
			{
				ndx = numChars - 1;
			}
		}
		else
		{
			ndx = 0;
			return atoi(receivedChars);
		}
	}
	return 0;
}