/**
 * luci: Internal and external lights control with relay output and TSL2561
 * digital light sensor
 * 
 * master.ino: The logic controller code. It waits for slave to send data and
 * processes IO.
 * 
 * Pinout of the serial cable
 *  VCC: 5v White/Brown
 *  GND: Brown
 *  A:   Orange
 *  B:   White/Orange
 * 
*/

#include "threshold.h"
#include <RS485Serial.h>

#define DEBUG(x)     \
  if (Serial)        \
  {                  \
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
#define L_ENTRANCE_R 5 // rear entrance lights
#define L_GROUND 6     // ground floor lights
#define L_STAIRS 7     // staircase lights
#define PW_IR_GROUND 8 // ground floor ir power
#define PW_IR_REAR 9   // rear entrance ir power
#define L_NIGHT 10     // perimetrale notte
#define L_BALCON 11    // tavoli + pavimento+ balconi 1+2 e affressco + balconi pt
#define L_EXT 12       // external lights. scritta vetri + sfere + vetri

// crepuscular thresholds for the staircase lights
#define TH_STAIR_ON 110
#define TH_STAIR_OFF 120

// crepuscular thresholds for external lights
#define EXTON 215
#define EXTOFF 220

// soglie balconi tavoli ecc.
#define TH_BALCON_ON 150
#define TH_BALCON_OFF 160

// pin 10
#define TH_NIGHT_ON 20
#define TH_NIGHT_OFF 15

// interval to wait before changing lights state (to avoid continuous on/off)
#define SWITCH_INT 1500

// Logic toggles
bool tog_stairs = false,
     tog_garden = false,
     tog_balconi = false,
     tog_night = false;

RS485Serial sserial(RX, TX, TALK);

Threshold stair_t(TH_STAIR_OFF, TH_STAIR_ON, SWITCH_INT);
Threshold ext_t(EXTOFF, EXTON, SWITCH_INT);

Threshold balconi_t(TH_BALCON_OFF, TH_BALCON_ON, SWITCH_INT);

Threshold night_t(TH_NIGHT_OFF, TH_NIGHT_ON, SWITCH_INT);

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
  pinMode(L_BALCON, OUTPUT);
  pinMode(L_NIGHT, OUTPUT);
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
  bool sw_auto = !digitalRead(SW_AUTO),
       sw_season = !digitalRead(SW_SEASON),
       ir_rear = !digitalRead(IR_REAR),
       ir_ground = !digitalRead(IR_GROUND);

  unsigned int lux = sserial.loop();

  if (lux < 65535)
  {
    DEBUG("Lux: ");
    DEBUG(lux);
    DEBUG("\n");

    stair_t.loop(lux);
    ext_t.loop(lux);
    balconi_t.loop(lux);
    night_t.loop(lux);
  }

  tog_stairs = stair_t.toggled();
  tog_garden = ext_t.toggled();
  tog_balconi = balconi_t.toggled();
  tog_night = night_t.toggled();

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

  //------- new --------

  if (sw_auto && sw_season && tog_balconi)
  {
    digitalWrite(L_BALCON, ON);
  }
  else
  {
    digitalWrite(L_BALCON, OFF);
  }

  if (sw_auto && tog_balconi)
  {
    digitalWrite(L_NIGHT, ON);
  }
  else
  {
    digitalWrite(L_NIGHT, OFF);
  }
}
