/**
 * A wrapper for SoftwareSerial with support for a "talk" pin.
 * 
 * Receive logic: http://forum.arduino.cc/index.php?topic=396450.0
*/
#ifndef RS485SERIAL_H
#define RS485SERIAL_H

#include <Arduino.h>
#include <SoftwareSerial.h>

enum
{
	BAUD_RATE = 1200
};

class RS485Serial
{
	const static byte numChars = 6; // 5 digits + \0
	const static char endMarker = '\n';

	unsigned int dataNumber = 0;

	char receivedChars[numChars]; // an array to store the received data

  private:
	SoftwareSerial sserial;
	int talk_pin;
	byte ndx;

  public:
	RS485Serial(int rx, int tx, int talk) : sserial(rx, tx), talk_pin(talk){};
	void begin();
	void sendLux(unsigned int value);
	unsigned int loop();
	static unsigned int getBaud();
};

#endif
