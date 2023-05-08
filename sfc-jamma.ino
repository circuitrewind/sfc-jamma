

////////////////////////////////////////////////////////////////////////////////
// SFC PROFILE CONFIGURATION
////////////////////////////////////////////////////////////////////////////////
#define BUTTON_MAX    (16)
#define PROFILE_MAX    (2)
int profile             = 0;
unsigned int  held_btns = 0;
unsigned long held_time = 0;
bool          held_test = false;




////////////////////////////////////////////////////////////////////////////////
// SFC PIN CONFIGURATION
////////////////////////////////////////////////////////////////////////////////
#define DATA_SERIAL   (14)
#define DATA_CLOCK    (15)
#define DATA_LATCH    (16)




////////////////////////////////////////////////////////////////////////////////
// JAMMA BUTTON 
////////////////////////////////////////////////////////////////////////////////
#define P1_TEST        (2)
#define P1_COIN        (3)
#define P1_START       (4)
#define P1_UP          (5)
#define P1_DOWN        (6)
#define P1_LEFT        (7)
#define P1_RIGHT       (8)
#define P1_LP          (9)
#define P1_MP         (10)
#define P1_HP         (11)
#define P1_LK         (17)
#define P1_MK         (18)
#define P1_HK         (19)




////////////////////////////////////////////////////////////////////////////////
// IBUS CONFIGURATION FOR SENDING BUTTONS BACK TO
// PC OVER SERIAL/USB INTERFACE
////////////////////////////////////////////////////////////////////////////////
#define ibus_baud     (115200)
#define ibus_sep      (0x40)
#define ibus_channels (3)
#define ibus_len      (4 + (ibus_channels * 2))
int ibus_checksum     = 0;




////////////////////////////////////////////////////////////////////////////////
// CONSTANT TIME DELAY CODE
// BROOK SNES ADAPTERS ARE UNSTABLE AT 6MS, BUT STABLE AT 7MS
// ADD AN EXTRA 0.5MS TO ENSURE A MARGIN OF ERROR
////////////////////////////////////////////////////////////////////////////////
unsigned long time;
#define time_delay    (7500)




////////////////////////////////////////////////////////////////////////////////
// BUTTON MAPPING PROFILES
////////////////////////////////////////////////////////////////////////////////
const byte P1[PROFILE_MAX][BUTTON_MAX] = {
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
// READ DATA FROM THE SFC SERIAL BUS
////////////////////////////////////////////////////////////////////////////////
unsigned int read_button() {
  unsigned int buttons = 0x0000;

  // INITIATE A LATCHING SEQUENCE
  digitalWrite(DATA_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(DATA_LATCH, LOW);
  delayMicroseconds(6);

  // LOOP THROUGH EACH BUTTON AND READ ITS VALUE
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
// TOGGLE TEST/SETTINGS BUTTON WHEN START+SELECT HELD FOR 2 SECONDS
////////////////////////////////////////////////////////////////////////////////
void test_button() {
  if (micros() - held_time < 2000000) return;

  held_btns = 0x0000;
  held_test = !held_test;

  if (held_test) {
    pinMode(P1_TEST, OUTPUT);
    digitalWrite(P1_TEST, LOW);
  } else {
    pinMode(P1_TEST, INPUT_PULLUP);
  }
}




////////////////////////////////////////////////////////////////////////////////
// MAIN INITIALIZATION CODE
////////////////////////////////////////////////////////////////////////////////
void setup() {
  
  // initialize pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // INITIALIZE PIN FOR TEST MODE
  pinMode(P1_TEST, INPUT_PULLUP);

  // SET ALL JAMMA PINS TO "INPUT" MODE
  for (int i=0; i<BUTTON_MAX; i++) {
    if (P1[profile][i] == 0xFF) continue;
    pinMode(P1[profile][i], INPUT_PULLUP);
  }
  
  // SET DATA_CLOCK NORMALLY HIGH
  pinMode(DATA_CLOCK, OUTPUT);
  digitalWrite(DATA_CLOCK, HIGH);
  
  // SET DATA_LATCH NORMALLY LOW
  pinMode(DATA_LATCH, OUTPUT);
  digitalWrite(DATA_LATCH, LOW);

  // SET DATA_SERIAL NORMALLY HIGH
  pinMode(DATA_SERIAL, OUTPUT);
  digitalWrite(DATA_SERIAL, HIGH);
  pinMode(DATA_SERIAL, INPUT_PULLUP);

  // SETUP SERIAL FOR IBUS
  Serial.begin(ibus_baud);

  // GET INITIAL TIMEFRAME
  time = micros();
}




////////////////////////////////////////////////////////////////////////////////
// MAIN PROGRAM LOOP
////////////////////////////////////////////////////////////////////////////////
void loop() {
  // READ THE CURRENT BUTTON STATE
  unsigned int    buttons = read_button();
  unsigned int    jamma   = 0;
  unsigned short  jamma_x = 0x8000;
  unsigned short  jamma_y = 0x8000;


  // LIGHT UP ARDUINO LED IF ANY SFC BUTTONS ARE PRESSED
  digitalWrite(LED_BUILTIN, (buttons != 0xFFFF));


  // BUTTON STATE CHANGED FROM LAST POLL TIME
  if (held_btns != buttons) {
    held_btns = buttons;
    held_time = micros();
  }


  // SWITCH BETWEEN JAMMA BUTTON MAPPING PROFILES
  switch (~buttons) {
    case 0x0014:  profile = 0;    break;
    case 0x0024:  profile = 1;    break;

    // TEST BUTTON TOGGLE
    case 0x000C:  test_button();  break;
  }


  // PROCESS BUTTON DATA
  for (int i=0; i<BUTTON_MAX; i++) {
    int btn = P1[profile][i];
    if (btn == 0xFF) continue;


    // SFC BUTTON NOT PRESSED, READ JAMMA BUTTON INSTEAD
    if (buttons & (0x01 << i)) {
      pinMode(btn, INPUT_PULLUP);
      if (digitalRead(btn) == LOW) {
        jamma |= (0x01 << i);

        // PROCESS ARCADE JOYSTICK
        switch (btn) {
          case P1_LEFT:   jamma_x = 0x0000; break;
          case P1_RIGHT:  jamma_x = 0xffff; break;
          case P1_UP:     jamma_y = 0x0000; break;
          case P1_DOWN:   jamma_y = 0xffff; break;
        }
      }


    // SEND SFC BUTTON PRESS TO JAMMA
    } else {
      pinMode(btn, OUTPUT);
      digitalWrite(btn, LOW);

      // PROCESS SFC D-PAD
      switch (btn) {
        case P1_LEFT:   jamma_x = 0x0000; break;
        case P1_RIGHT:  jamma_x = 0xffff; break;
        case P1_UP:     jamma_y = 0x0000; break;
        case P1_DOWN:   jamma_y = 0xffff; break;
      }
    }
  }


  // SEND SFC AND JAMMA BUTTON STATUS TO IBUS
  ibus_start();
  ibus_write(jamma | (~buttons));
  ibus_write(jamma_x);
  ibus_write(jamma_y);
  ibus_end();

  
  // DELAY REQUIRED FOR BROOK ADAPTER COMPATIBILITY
  // USE A CONSTANT TIME DELAY REGARDLESS OF PROCESSING ABOVE
  while ((micros() - time) < time_delay) {}
  time += time_delay;
}




/*
 * red      data
 * yellow   clock
 * green    5v
 * black    ground
 * white    latch
 * 
 */
