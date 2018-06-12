/*
 *      lights.ino: Internal and external lights control with relay output and TSL2561 digital light sensor
 *
 *      Pinout of the TSL2561 sensor:
 *          VCC: 3.3v
 *          GND: Ground
 *          SCL: A5
 *          SDA: A4
 *
 *      I wrote this software in ~2013, left it undocumented, and now it is barely understandable.
 *      This is a digital sensor conversion of the system.
*/

#include "TSL2561.h"

#define DEBUG(x)     \
  if (Serial)        \
  {                  \
    Serial.print(x); \
  }

#define ON LOW
#define OFF HIGH

// switches
#define SW_AUTO 4
#define SW_SEASON 5
#define IR_REAR 2
#define IR_GROUND 3

// outputs
#define L_UNUSED_0 6 // Currently unused relays
#define L_UNUSED_1 7
#define L_ENTRANCE_R 8  // rear entrance lights
#define L_GROUND 9      // ground floor lights
#define L_STAIRS 10     // staircase lights
#define PW_IR_REAR 11   // rear entrance ir power
#define PW_IR_GROUND 12 // ground floor ir power
#define L_EXT 13        // external lights

// crepuscular thresholds for the staircase lights
#define CRON 100
#define CROFF 110

// crepuscular thresholds for external lights
#define EXTON 640
#define EXTOFF 620

// interval to wait before changing lights state (to avoid continuous on/off)
#define INTERVALCR 30000

#define READINT 1000

TSL2561 tsl(TSL2561_ADDR_FLOAT);

// Logic toggles
bool tog_stairs = false,
     tog_garden = false;

bool wait1 = false, wait2 = false;
unsigned long waitstart1, waitstart2, read_wait;

void setup()
{
  Serial.begin(9600);
  DEBUG("Starting...");

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
  DEBUG("I/O...");

  if (tsl.begin())
  {
    tsl.setGain(TSL2561_GAIN_16X);
    tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);
    DEBUG("Sensor...");
  }
  else
  {
    DEBUG("Sensor Error.");
    while (1)
      ;
  }

  DEBUG("Done.\n");
  read_wait = millis();
}

void loop()
{
  /* Declare logic vars
    sw_ = switch
    ir_ = ir sensor */
  bool sw_auto = digitalRead(SW_AUTO),
       sw_season = digitalRead(SW_SEASON),
       ir_rear = digitalRead(IR_REAR),
       ir_ground = digitalRead(IR_GROUND);

  unsigned long now = millis();

  if (now - read_wait >= READINT)
  {
    read_wait = now;
    uint16_t lux = tsl.getLuminosity(TSL2561_VISIBLE);

    if (Serial)
    {
      DEBUG("Lux: ");
      DEBUG(lux);
      DEBUG("\n");
    }

    // Too much light
    if (lux >= CROFF)
    {
      if (!wait1)
      {
        waitstart1 = now;
        wait1 = true;
      }
      else if (now - waitstart1 >= INTERVALCR && wait1)
      {
        tog_stairs = false;
        wait1 = false;
      }
    }
    // It's dark!
    else if (lux <= CRON)
    {
      if (!wait1)
      {
        waitstart1 = now;
        wait1 = true;
      }
      else if (now - waitstart1 >= INTERVALCR && wait1)
      {
        tog_stairs = true;
        wait1 = false;
      }
    }

    if (lux >= EXTOFF)
    {
      if (!wait2)
      {
        waitstart2 = now;
        wait2 = true;
      }
      else if (now - waitstart2 >= INTERVALCR && wait2)
      {
        tog_garden = false;
        wait2 = false;
      }
    }
    else if (lux <= EXTON)
    {
      if (!wait2)
      {
        waitstart2 = now;
        wait2 = true;
      }
      else if (now - waitstart2 >= INTERVALCR && wait2)
      {
        tog_garden = true;
        wait2 = false;
      }
    }
  }

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
