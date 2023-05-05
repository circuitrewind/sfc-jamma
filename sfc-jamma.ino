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


int profile = 0;


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


void setup() {
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // SET ALL JAMMA PINS TO "INPUT" MODE
  for (int i=0; i<BUTTON_MAX; i++) {
    if (P1[profile][i] == 0xFF) continue;
    pinMode(P1[profile][i], INPUT);
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
}


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

  return buttons;
}


void loop() {
  // READ THE CURRENT BUTTON STATE
  unsigned int buttons = read_button();


  // CONTROLLER APPEARS UNPLUGGED
  // PROCESS AS NO BUTTONS PRESSED INSTEAD
  if (buttons == 0x0000) buttons = 0xFFFF;
  

  // LIGHT UP LED IF ANY BUTTONS ARE PRESSED
  digitalWrite(LED_BUILTIN, (buttons != 0xFFFF));


  // SEND BUTTON DATA TO JAMMA
  for (int i=0; i<BUTTON_MAX; i++) {
    int btn = P1[profile][i];
    if (btn == 0xFF) continue;
    
    if (buttons & (0x01 << i)) {
      pinMode(btn, INPUT);
    } else {
      pinMode(btn, OUTPUT);
      digitalWrite(btn, LOW);
    }
  }

  // DELAY REQUIRED FOR BROOK ADAPTER
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
