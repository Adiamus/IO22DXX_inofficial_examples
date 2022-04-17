// The Term Server in modbus is actually a client device
// like a sensor or relay



#include <ArduinoRS485.h>
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
int dispval = 0;
int fps = 0;
bool statusLED = (0);

bool clock_val = 0;
bool latch_val = 0;
bool data_val = 0;

unsigned long c_time = 0;
unsigned long o_time = 0;
int m_cntr = 0;

uint8_t modbus_address = 0x02;
int modbus_speed = 9600;

void maintenance() {
  m_cntr++;
  if (m_cntr >= 4096) {
    digitalWrite(LED_BUILTIN, HIGH);
    //delay(1);
    m_cntr = 0;
    digitalWrite(LED_BUILTIN, LOW);
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

  //digitalWrite(LED_BUILTIN, statusLED);

  //write_display();

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

  /*for (int i = 512; i > 0; i--) {
    dispval = i;
    write_all();
    delay(2);
    }*/

  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(1);
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(1);
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(1);
  //digitalWrite(LED_BUILTIN, LOW);
  // start the Modbus RTU server, with (slave) id 1
  if (!ModbusRTUServer.begin(modbus_address, modbus_speed)) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
  digitalWrite(LED_BUILTIN, LOW);
  //Relay 1 till 4 and all 4 together at 0x04
  ModbusRTUServer.configureCoils(0x00, 4);

  // 7 Segment display
  ModbusRTUServer.configureHoldingRegisters(0x00, 2);

  //Input 1 till 4 and all together at 0x0A
  //Button K1 till K4 and all together at 0x0F
  ModbusRTUServer.configureDiscreteInputs(0x00, 8);

}

void loop() {
  maintenance();
  if (c_time < millis()) {
    o_time = c_time;
    c_time = millis();
    fps = (c_time - o_time);
  }

  //Serial.println(dispval);
  read_all();


  Q[0] = ModbusRTUServer.coilRead(0x00);
  ModbusRTUServer.coilWrite(0x00, Q[0]);
  ModbusRTUServer.poll();
  Q[1] = ModbusRTUServer.coilRead(0x01);
  ModbusRTUServer.coilWrite(0x01, Q[1]);
  ModbusRTUServer.poll();
  Q[2] = ModbusRTUServer.coilRead(0x02);
  ModbusRTUServer.coilWrite(0x02, Q[2]);
  ModbusRTUServer.poll();
  Q[3] = ModbusRTUServer.coilRead(0x03);
  ModbusRTUServer.coilWrite(0x03, Q[3]);
  ModbusRTUServer.poll();

  dispval = ModbusRTUServer.holdingRegisterRead(0x00);
  ModbusRTUServer.holdingRegisterWrite(0x00, dispval);
  ModbusRTUServer.poll();
  fps = ModbusRTUServer.holdingRegisterRead(0x01);
  ModbusRTUServer.holdingRegisterWrite(0x01, fps);
  ModbusRTUServer.poll();

  ModbusRTUServer.discreteInputWrite(0x00, In[0]);
  In[0] = ModbusRTUServer.discreteInputRead(0x00);
  ModbusRTUServer.poll();
  ModbusRTUServer.discreteInputWrite(0x01, In[1]);
  In[1] = ModbusRTUServer.discreteInputRead(0x01);
  ModbusRTUServer.poll();
  ModbusRTUServer.discreteInputWrite(0x02, In[2]);
  In[2] = ModbusRTUServer.discreteInputRead(0x02);
  ModbusRTUServer.poll();
  ModbusRTUServer.discreteInputWrite(0x03, In[3]);
  In[3] = ModbusRTUServer.discreteInputRead(0x03);
  ModbusRTUServer.poll();

  ModbusRTUServer.discreteInputWrite(0x04, Bttn[0]);
  Bttn[0] = ModbusRTUServer.discreteInputRead(0x04);
  ModbusRTUServer.poll();
  ModbusRTUServer.discreteInputWrite(0x05, Bttn[1]);
  Bttn[1] = ModbusRTUServer.discreteInputRead(0x05);
  ModbusRTUServer.poll();
  ModbusRTUServer.discreteInputWrite(0x06, Bttn[2]);
  Bttn[2] = ModbusRTUServer.discreteInputRead(0x06);
  ModbusRTUServer.poll();
  ModbusRTUServer.discreteInputWrite(0x07, Bttn[3]);
  Bttn[3] = ModbusRTUServer.discreteInputRead(0x07);
  ModbusRTUServer.poll();

  write_all();
  ModbusRTUServer.poll();
}
