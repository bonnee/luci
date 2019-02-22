/**
 *	luci: Internal and external lights control with relay output and TSL2561
 * 	digital light sensor
 *
 *	master.ino: The logic controller code. It waits for slave to send data and
 * 	processes IO.
*/

#include "threshold.h"
#include <RS485Serial.h>

#define DEBUG(x)         \
	if (Serial)          \
	{                    \
		Serial.print(x); \
	}

#define ON LOW
#define OFF HIGH

#define RX 2
#define TX 3
#define TALK 4

// switches
#define SW_AUTO 16   // A2
#define SW_SEASON 17 // A3
#define IR_REAR 14   // A0
#define IR_GROUND 15 // A1

// outputs
#define L_UNUSED_0 5 // Currently unused relays
#define L_UNUSED_1 6
#define L_ENTRANCE_R 7  // rear entrance lights
#define L_GROUND 8		// ground floor lights
#define L_STAIRS 9		// staircase lights
#define PW_IR_REAR 10   // rear entrance ir power
#define PW_IR_GROUND 11 // ground floor ir power
#define L_EXT 12		// external lights

// crepuscular thresholds for the staircase lights
#define CRON 100
#define CROFF 110

// crepuscular thresholds for external lights
#define EXTON 100
#define EXTOFF 110

// interval to wait before changing lights state (to avoid continuous on/off)
#define SWITCH_INT 30000

// Logic toggles
bool tog_stairs = false,
	 tog_garden = false;

RS485Serial sserial(RX, TX, TALK);

Threshold stair_t(CROFF, CRON, SWITCH_INT);
Threshold ext_t(EXTOFF, EXTON, SWITCH_INT);

void setup()
{
	Serial.begin(sserial.getBaud());

	DEBUG("MASTER\n");
	DEBUG("I/O...");
	pinMode(SW_AUTO, INPUT_PULLUP);
	pinMode(SW_SEASON, INPUT_PULLUP);
	pinMode(IR_REAR, INPUT_PULLUP);
	pinMode(IR_GROUND, INPUT_PULLUP);

	pinMode(L_ENTRANCE_R, OUTPUT);
	pinMode(L_GROUND, OUTPUT);
	pinMode(L_STAIRS, OUTPUT);
	pinMode(PW_IR_REAR, OUTPUT);
	pinMode(PW_IR_GROUND, OUTPUT);
	pinMode(L_EXT, OUTPUT);

	sserial.begin();

	DEBUG("Done.\n")
}

void loop()
{
	/* Declare logic vars:
    	sw_ = switch
    	ir_ = ir sensor
	*/
	bool sw_auto = digitalRead(SW_AUTO),
		 sw_season = digitalRead(SW_SEASON),
		 ir_rear = digitalRead(IR_REAR),
		 ir_ground = digitalRead(IR_GROUND);

	//-----------------------

	unsigned int lux = sserial.loop();

	if (lux > 0)
	{
		DEBUG("Lux: ");
		DEBUG(lux);
		DEBUG("\n");

		stair_t.loop(lux);
		ext_t.loop(lux);
	}

	tog_stairs = stair_t.toggled();
	tog_garden = ext_t.toggled();

	// Don't try to understand the rest of this program
	if (!sw_auto || ir_rear || (ir_ground && !sw_season) || (sw_season && tog_stairs))
	{
		digitalWrite(L_ENTRANCE_R, ON);
	}
	else
	{
		digitalWrite(L_ENTRANCE_R, OFF);
	}

	if (!sw_auto || (sw_auto && !sw_season && (ir_rear || ir_ground)) || sw_auto && sw_season && tog_stairs)
	{
		digitalWrite(L_GROUND, ON);
	}
	else
	{
		digitalWrite(L_GROUND, OFF);
	}

	if (sw_season && (!sw_auto || tog_stairs))
	{
		digitalWrite(L_STAIRS, ON);
	}
	else
	{
		digitalWrite(L_STAIRS, OFF);
	}

	if (sw_auto && !(sw_season && tog_stairs))
	{
		digitalWrite(PW_IR_REAR, ON);
	}
	else
	{
		digitalWrite(PW_IR_REAR, OFF);
	}

	if (sw_auto && !sw_season)
	{
		digitalWrite(PW_IR_GROUND, ON);
	}
	else
	{
		digitalWrite(PW_IR_GROUND, OFF);
	}

	if (sw_auto && sw_season && tog_garden)
	{
		digitalWrite(L_EXT, ON);
	}
	else
	{
		digitalWrite(L_EXT, OFF);
	}
}
