/*
        lights.ino: Internal and external ights control with relay output and TSL2561 digital light sensor

        Pinout of the TSL2561 sensor:
            VCC: 3.3v
            GND: Ground
            SDA: A5
            SCL: A4

        I wrote this software in ~2013, left it undocumented, and now it is barely understandable.
        I'm planning to expand this system and might end up rewriting a good part of it.
*/
#include "TSL2561.h"

// photoresistor
//#define pinc A0
TSL2561 tsl(TSL2561_ADDR_FLOAT);

// switches
#define pina 2
#define pinb 3
#define pind 4
#define pine 5
#define pinf 13

// lights
#define l1 8
#define l2 9
#define l3 10
#define lpi 11
#define lpt 12

// crepuscular thresholds for the staircase lights
#define cron 600
#define croff 560

// crepuscular thresholds for external lights
#define exton 640
#define extoff 620

// interval to wait before changing lights state (to debounce the photoresistor)
#define intervalcr 30000

#define readInt 1000

// declare logical switches
bool a, b, c = false, d, e, f = false, wait1 = false, wait2 = false;

uint16_t lux;

unsigned long waitstart1, waitstart2, readWait;

void setup() {
  Serial.begin(9600);
  Serial.print("Starting...");
  pinMode(a, INPUT);
  pinMode(b, INPUT);
  pinMode(d, INPUT);
  pinMode(e, INPUT);

  pinMode(lpi, OUTPUT);
  pinMode(lpt, OUTPUT);
  pinMode(pinf, OUTPUT);
  Serial.print("I/O OK...");

  if (tsl.begin())
  {
    tsl.setGain(TSL2561_GAIN_16X);
    tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);
    Serial.print("Sensor OK...");
  }
  else
  {
    Serial.print("Sensor Error.");
    while (1);
  }

  Serial.println("Done.");
  readWait = millis();
}

void loop() {
  a = digitalRead(pina);
  b = digitalRead(pinb);
  d = digitalRead(pind);
  e = digitalRead(pine);
  
  if (millis() - readWait >= readInt)
  {
    readWait = millis();
    lux = tsl.getLuminosity(TSL2561_VISIBLE);

    if (Serial) {
      Serial.print("Lux: ");
      Serial.println(lux);
    }

  // checks the crepuscular thresholds for the corridor and "starts" the timer to change state
  if (lux <= croff) {
    if (wait1 == false) {
      waitstart1 = millis();
      wait1 = true;
    } else if (millis() - waitstart1 >= intervalcr && wait1) {
      c = false;
      wait1 = false;
    }
  } else if (lux >= cron) {
    if (wait1 == false) {
      waitstart1 = millis();
      wait1 = true;
    } else if (millis() - waitstart1 >= intervalcr && wait1) {
      c = true;
      wait1 = false;
    }
  }

  // checks the crepuscular thresholds for the garden and "starts" the timer to change state
  if (lux <= extoff) {
    if (wait2 == false) {
      waitstart2 = millis();
      wait2 = true;
    } else if (millis() - waitstart2 >= intervalcr && wait2) {
      f = false;
      wait2 = false;
    }
  } else if (lux >= exton) {
    if (wait2 == false) {
      waitstart2 = millis();
      wait2 = true;
    } else if (millis() - waitstart2 >= intervalcr && wait2) {
      f = true;
      wait2 = false;
    }
  }

  // WARNING: Don't try to understand this
  if (!a || d || (e && !b) || (b && c)) {
    digitalWrite(l1, HIGH);
  } else {
    digitalWrite(l1, LOW);
  }

  if (!a || (a && !b && (d || e)) || a && b && c) {
    digitalWrite(l2, HIGH);
  } else {
    digitalWrite(l2, LOW);
  }

  if (b && (!a || c)) {
    digitalWrite(l3, HIGH);
  } else {
    digitalWrite(l3, LOW);
  }

  if (a && !(b && c)) {
    digitalWrite(lpi, HIGH);
  } else {
    digitalWrite(lpi, LOW);
  }

  if (a && !b) {
    digitalWrite(lpt, HIGH);
  } else {
    digitalWrite(lpt, LOW);
  }

  if (a && b && f) {
    digitalWrite(pinf, HIGH);
  } else {
    digitalWrite(pinf, LOW);
  }
}
