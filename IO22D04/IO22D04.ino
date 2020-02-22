#include <ArduinoModbus.h>


#define In1 A1
#define In2 A0
#define In3 A3
#define In4 A2
#define K1 2
#define K2 3
#define K3 4
#define K4 5
#define J1 10
#define J2 11
#define J3 12
#define J4 6
#define clock_pin 9
#define latch_pin 8
#define data_pin 7

bool In[4]    = {0};
bool Q[4]     = {0};
bool Bttn[4]  = {0};
uint16_t dispval = 0;
bool statusLED = (0);

bool clock_val = 0;
bool latch_val = 0;
bool data_val = 0;

unsigned long c_time = 0;
unsigned long o_time = 0;
int m_cntr = 0;

void maintenance() {
  m_cntr++;
  if (m_cntr >= 1024) {
    delay(100);
    m_cntr = 0;
  }
}

void read_all() {
  In[0] = digitalRead(In1);
  In[1] = digitalRead(In2);
  In[2] = digitalRead(In3);
  In[3] = digitalRead(In4);

  Bttn[0] = digitalRead(K1);
  Bttn[1] = digitalRead(K2);
  Bttn[2] = digitalRead(K3);
  Bttn[3] = digitalRead(K4);

  Bttn[0] = !Bttn[0];
  Bttn[1] = !Bttn[1];
  Bttn[2] = !Bttn[2];
  Bttn[3] = !Bttn[3];

}

void write_all() {
  digitalWrite(J1, Q[0]);
  digitalWrite(J2, Q[1]);
  digitalWrite(J3, Q[2]);
  digitalWrite(J4, Q[3]);

  digitalWrite(LED_BUILTIN, statusLED);

  // funktion fuer 7 segment anzeige aufrufen

}

void write_display() {
  //schreibe dispval


  //ground latchPin and hold low for as long as you are transmitting
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, dispval);
  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latch_pin, HIGH);


}

void setup() {
  Serial.begin(19200);

  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  pinMode(K3, INPUT_PULLUP);
  pinMode(K4, INPUT_PULLUP);
  pinMode(In1, INPUT);
  pinMode(In2, INPUT);
  pinMode(In3, INPUT);
  pinMode(In4, INPUT);

  pinMode(J1, OUTPUT);
  pinMode(J2, OUTPUT);
  pinMode(J3, OUTPUT);
  pinMode(J4, OUTPUT);

  pinMode(clock_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 257; i > 0; i--) {
    dispval = i;
    write_display();
    delay(25);
  }
  delay(1000);
}

void loop() {
  maintenance();
  if (c_time < millis()) {
    o_time = c_time;
    c_time = millis();
    dispval = (c_time - o_time);
  }

  Serial.println(dispval);
  read_all();


  Q[0] = Bttn[0];
  Q[1] = Bttn[1];
  Q[2] = Bttn[2];
  Q[3] = Bttn[3];



  write_display();
  write_all();
}
