#define pinc A0

int pina = 2, pinb = 3, pind = 4, pine = 5, l1 = 8, l2 = 9, l3 = 10, lpi = 11, lpt = 12, pinf = 13, \
    cron = 600, croff = 560, exton = 640, extoff = 620, intervalcr = 30000;

// Declare logical switches
bool a, b, c = false, d, e, f = false, wait1 = false, wait2 = false;


unsigned long waituntil1, waituntil2;

void setup() {
        pinMode(a, INPUT);
        pinMode(b, INPUT);
        pinMode(d, INPUT);
        pinMode(e, INPUT);
        pinMode(pinc, INPUT);

        pinMode(l1, OUTPUT);
        pinMode(l2, OUTPUT);
        pinMode(l3, OUTPUT);
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

        // Checks the crepuscular thresholds for the corridor
        if (analogRead(pinc) <= croff) {
                if (wait1 == false) {
                        waituntil1 = millis();
                        wait1=true;
                } else if (millis() - waituntil1 >= intervalcr && wait1) {
                        c = false;
                        wait1=false;
                }
        } else if (analogRead(pinc) >= cron) {
                if (wait1 == false) {
                        waituntil1 = millis();
                        wait1=true;
                } else if (millis() - waituntil1 >= intervalcr && wait1) {
                        c = true;
                        wait1=false;
                }
        }

        // Checks the crepuscular thresholds for the garden
        if (analogRead(pinc) <= extoff) {
                if (wait2 == false) {
                        waituntil2 = millis();
                        wait2 = true;
                } else if (millis() - waituntil2 >= intervalcr && wait2) {
                        f = false;
                        wait2=false;
                }

        } else if (analogRead(pinc) >= exton) {
                if (wait2 == false) {
                        waituntil2 = millis();
                        wait2=true;
                } else if (millis() - waituntil2 >= intervalcr && wait2) {
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
