/*
        lights.ino: Internal and external ights control with relay shield

        I wrote this software 4-5 years ago, left it undocumented, and now it is barely readable.
        I will upgrade this to work with a digital light sensor in the future, and I might end up rewriting a good part of it.
*/

// photoresistor
#define pinc A0

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

// declare logical switches
bool a, b, c = false, d, e, f = false, wait1 = false, wait2 = false;

unsigned long waitstart1, waitstart2;

void setup() {
        pinMode(a, INPUT);
        pinMode(b, INPUT);
        pinMode(d, INPUT);
        pinMode(e, INPUT);
        pinMode(pinc, INPUT);

        pinMode(lpi, OUTPUT);
        pinMode(lpt, OUTPUT);
        pinMode(pinf, OUTPUT);

        Serial.begin(9600);
}

void loop() {
        a = digitalRead(pina);
        b = digitalRead(pinb);
        d = digitalRead(pind);
        e = digitalRead(pine);

        if(Serial)
                Serial.println(analogRead(pinc));

        // checks the crepuscular thresholds for the corridor and "starts" the timer to change state
        if (analogRead(pinc) <= croff) {
                if (wait1 == false) {
                        waitstart1 = millis();
                        wait1=true;
                } else if (millis() - waitstart1 >= intervalcr && wait1) {
                        c = false;
                        wait1=false;
                }
        } else if (analogRead(pinc) >= cron) {
                if (wait1 == false) {
                        waitstart1 = millis();
                        wait1=true;
                } else if (millis() - waitstart1 >= intervalcr && wait1) {
                        c = true;
                        wait1=false;
                }
        }

        // checks the crepuscular thresholds for the garden and "starts" the timer to change state
        if (analogRead(pinc) <= extoff) {
                if (wait2 == false) {
                        waitstart2 = millis();
                        wait2 = true;
                } else if (millis() - waitstart2 >= intervalcr && wait2) {
                        f = false;
                        wait2=false;
                }
        } else if (analogRead(pinc) >= exton) {
                if (wait2 == false) {
                        waitstart2 = millis();
                        wait2=true;
                } else if (millis() - waitstart2 >= intervalcr && wait2) {
                        f = true;
                        wait2=false;
                }
        }

        // WARNING: Don't try to understand this
        if (!a || d || (e && !b) || (b && c)) {
                digitalWrite(l1, HIGH);
        } else { digitalWrite(l1, LOW); }

        if (!a || (a && !b && (d || e)) || a && b && c) {
                digitalWrite(l2, HIGH);
        } else { digitalWrite(l2, LOW); }

        if (b && (!a || c)) {
                digitalWrite(l3, HIGH);
        } else { digitalWrite(l3, LOW); }

        if (a && !(b && c)) {
                digitalWrite(lpi, HIGH);
        } else { digitalWrite(lpi, LOW); }

        if (a && !b) {
                digitalWrite(lpt, HIGH);
        } else { digitalWrite(lpt, LOW); }

        if (a && b && f) {
                digitalWrite(pinf, HIGH);
        } else { digitalWrite(pinf, LOW); }
}
