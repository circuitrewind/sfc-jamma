#define DATA_SERIAL   14
#define DATA_CLOCK    15
#define DATA_LATCH    16
#define BUTTON_MAX    16

#define PROFILE_MAX    2

#define P1_COIN        3
#define P1_START       4
#define P1_TEST        0

#define P1_UP          5
#define P1_DOWN        6
#define P1_LEFT        7
#define P1_RIGHT       8

#define P1_LP          9
#define P1_MP         10
#define P1_HP         11

#define P1_LK         17
#define P1_MK         18
#define P1_HK         19


#define ibus_baud     (115200)
#define ibus_sep      (0x40)
#define ibus_channels (2)
#define ibus_len      (4 + (ibus_channels * 2))

int ibus_checksum     = 0;

int profile           = 0;


int P1[PROFILE_MAX][BUTTON_MAX] = {
  {
    P1_LK,
    P1_LP,
    P1_COIN,
    P1_START,
    P1_UP,
    P1_DOWN,
    P1_LEFT,
    P1_RIGHT,
    P1_MK,
    P1_MP,
    P1_HP,
    P1_HK,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
  }, {
    P1_LK,
    P1_LP,
    P1_COIN,
    P1_START,
    P1_UP,
    P1_DOWN,
    P1_LEFT,
    P1_RIGHT,
    P1_HK,
    P1_HP,
    P1_MP,
    P1_MK,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
  }
};




////////////////////////////////////////////////////////////////////////////////
// BEGIN AN IBUS TRANSMISSION
////////////////////////////////////////////////////////////////////////////////
void ibus_start() {
  ibus_checksum = 0xffff - ibus_len - ibus_sep;
  Serial.write(ibus_len);
  Serial.write(ibus_sep);
}




////////////////////////////////////////////////////////////////////////////////
// END AN IBUS TRANSMISSION
////////////////////////////////////////////////////////////////////////////////
void ibus_end() {
  Serial.write(ibus_checksum & 0xff);
  Serial.write(ibus_checksum >> 8);

//  Serial.write('\n');
}




////////////////////////////////////////////////////////////////////////////////
// SEND AN IBUS MESASGE
////////////////////////////////////////////////////////////////////////////////
void ibus_write(unsigned short data) {
  // WRITE FIRST BYTE
  byte b = data & 0xff;
  Serial.write(b);
  ibus_checksum -= b;

  // WRITE SECOND BYTE
  b = data >> 8;
  Serial.write(b);
  ibus_checksum -= b;
}




////////////////////////////////////////////////////////////////////////////////
// MAIN INITIALIZATION CODE
////////////////////////////////////////////////////////////////////////////////
void setup() {
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // SET ALL JAMMA PINS TO "INPUT" MODE
  for (int i=0; i<BUTTON_MAX; i++) {
    if (P1[profile][i] == 0xFF) continue;
    pinMode(P1[profile][i], INPUT_PULLUP);
  }
  
  // Set DATA_CLOCK normally HIGH
  pinMode(DATA_CLOCK, OUTPUT);
  digitalWrite(DATA_CLOCK, HIGH);
  
  // Set DATA_LATCH normally LOW
  pinMode(DATA_LATCH, OUTPUT);
  digitalWrite(DATA_LATCH, LOW);

  // Set DATA_SERIAL normally HIGH
  pinMode(DATA_SERIAL, OUTPUT);
  digitalWrite(DATA_SERIAL, HIGH);
  pinMode(DATA_SERIAL, INPUT_PULLUP);
  digitalWrite(DATA_SERIAL, HIGH);

  // SETUP SERIAL FOR IBUS
  Serial.begin(ibus_baud);
}




////////////////////////////////////////////////////////////////////////////////
// READ DATA FROM THE SFC BUS
////////////////////////////////////////////////////////////////////////////////
unsigned int read_button() {
  unsigned int buttons = 0;
  
  digitalWrite(DATA_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(DATA_LATCH, LOW);
  delayMicroseconds(6);

  for(int i=0; i<BUTTON_MAX; i++){
    digitalWrite(DATA_CLOCK, LOW);
    delayMicroseconds(6);
    buttons |= (!!digitalRead(DATA_SERIAL)) << i;
    digitalWrite(DATA_CLOCK, HIGH);
    delayMicroseconds(6);
  }

  // CONTROLLER APPEARS UNPLUGGED
  // PROCESS AS NO BUTTONS PRESSED INSTEAD
  if (buttons == 0x0000) buttons = 0xFFFF;

  return buttons;
}




////////////////////////////////////////////////////////////////////////////////
// MAIN PROGRAM LOOP
////////////////////////////////////////////////////////////////////////////////
void loop() {
  // READ THE CURRENT BUTTON STATE
  unsigned int buttons  = read_button();
  unsigned int jamma    = 0;


  // LIGHT UP ARDUINO LED IF ANY SFC BUTTONS ARE PRESSED
  digitalWrite(LED_BUILTIN, (buttons != 0xFFFF));


  // PROCESS BUTTON DATA
  for (int i=0; i<BUTTON_MAX; i++) {
    int btn = P1[profile][i];
    if (btn == 0xFF) continue;

    // SFC BUTTON NOT PRESSED, READ JAMMA BUTTON INSTEAD
    if (buttons & (0x01 << i)) {
      pinMode(btn, INPUT_PULLUP);
      if (digitalRead(btn) == LOW) {
        jamma |= (0x01 << i);
      }

    // SEND SFC BUTTON PRESS TO JAMMA
    } else {
      pinMode(btn, OUTPUT);
      digitalWrite(btn, LOW);
    }
  }


  // SEND SFC AND JAMMA BUTTON STATUS TO IBUS
  ibus_start();
  ibus_write(~buttons);
  ibus_write(jamma);
  ibus_end();

  
  // DELAY REQUIRED FOR BROOK ADAPTER COMPATIBILITY
  delay(8);
}




/*
 * red      data
 * yellow   clock
 * green    5v
 * black    ground
 * white    latch
 * 
 */
