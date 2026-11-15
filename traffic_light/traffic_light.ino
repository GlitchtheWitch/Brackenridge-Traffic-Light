// Pedestrian crossing with countdown timer
//
// Normal state:   car red OFF, pedestrian red ON, pedestrian green (blue) OFF
// Button pressed: car red ON, pedestrian red OFF, blue ON, timer counts down 9 -> 0
// Timer ends:     back to normal state

// Pins
const int BUTTON_PIN = 12;
const int CAR_RED    = 2;
const int PED_RED    = 3;
const int PED_GREEN  = 4;

// 7-segment display pins, in order: a, b, c, d, e, f, g
// a = top (8), b = top right (9), c = bottom right (10), d = bottom (6),
// e = bottom left (5), f = top left (7), g = middle (11)
const int SEG_PINS[7] = {8, 9, 10, 6, 5, 7, 11};

// false => common CATHODE (middle pin to GND). true => common ANODE (middle pin to 5V).
const bool COMMON_ANODE = false;

const int COUNTDOWN_START = 9; // seconds the pedestrian gets to cross

// Which segments are lit for each digit 0-9 (a b c d e f g)
const byte DIGITS[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

void showDigit(int n) {
  for (int i = 0; i < 7; i++) {
    bool on = DIGITS[n][i];
    if (COMMON_ANODE) on = !on;
    digitalWrite(SEG_PINS[i], on ? HIGH : LOW);
  }
}

void clearDisplay() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(SEG_PINS[i], COMMON_ANODE ? HIGH : LOW);
  }
}

void normalState() {
  digitalWrite(CAR_RED, LOW);    // cars may go
  digitalWrite(PED_GREEN, LOW);
  digitalWrite(PED_RED, HIGH);   // pedestrians wait
  clearDisplay();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // reads LOW when pressed
  pinMode(CAR_RED, OUTPUT);
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);
  for (int i = 0; i < 7; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
  }
  normalState();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Stop the cars, let pedestrians cross
    digitalWrite(CAR_RED, HIGH);
    digitalWrite(PED_RED, LOW);
    digitalWrite(PED_GREEN, HIGH);

    // Count down on the display
    for (int t = COUNTDOWN_START; t >= 0; t--) {
      showDigit(t);
      delay(1000);
    }

    // Time's up: pedestrians wait, cars go
    normalState();
  }
}
