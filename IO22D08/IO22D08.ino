

#define In1 2
#define In2 3
#define In3 4
#define In4 5
#define In5 6
#define In6 A0
#define In7 12
#define In8 11
#define K1 7
#define K2 8
#define K3 9
#define K4 10
#define clock_pin A3
#define latch_pin A2
#define OE_595 A1
#define data_pin 13


bool In[8]    = {0};
bool Q[8]     = {0};
bool Bttn[4]  = {0};
int dispval = 0;

bool clock_val = 0;
bool latch_val = 0;
bool data_val = 0;
bool OE = 0;

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
  In[4] = digitalRead(In5);
  In[5] = digitalRead(In6);
  In[6] = digitalRead(In7);
  In[7] = digitalRead(In8);

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
  pinMode(In5, INPUT);
  pinMode(In6, INPUT);
  pinMode(In7, INPUT);
  pinMode(In8, INPUT);

  pinMode(clock_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(OE_595, OUTPUT);

  digitalWrite(OE_595, LOW);

  for (int i = 0; i < 512; i++) {
    dispval = i;
    write_display();
    delay(10);
  }
  delay(1000);
}

void loop() {
  maintenance();
 /* if (c_time < millis()) {
    o_time = c_time;
    c_time = millis();
    dispval = (c_time - o_time);
  }*/

  Serial.println(dispval);
  read_all();


  Q[0] = Bttn[0];
  Q[1] = Bttn[1];
  Q[2] = Bttn[2];
  Q[3] = Bttn[3];

  dispval = dispval + Q[0];
  dispval = dispval - Q[3];


  write_display();
  write_all();
}
