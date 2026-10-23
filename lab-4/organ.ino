/**
* Timer Lab
*/
#include "notes.h"

#define DEB_THRESH 50000  // 50 ms

/**
* ==============================================================================
* -  Key Pad Layout  -----------------------------------------------------------
* ==============================================================================
*
*  +---+---+---+---+                 +---+---+---+---+
*  | 1 | 2 | 3 | A |                 | C | # | D | # |
*  +---+---+---+---+                 +---+---+---+---+
*  | 4 | 5 | 6 | B |                 | E | F | # | G |
*  +---+---+---+---+  ============>  +---+---+---+---+
*  | 7 | 8 | 9 | C |                 | # | A | # | B |
*  +---+---+---+---+                 +---+---+---+---+
*  | * | 0 | # | D |                 | C | < | = | > |
*  +---+---+---+---+                 +---+---+---+---+
*
* ==============================================================================
*/
static const int FREQ_MAP[4][4] = {
  { NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4 },
  { NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4 },
  { NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4 },
  { NOTE_C5, -1, 0, 1 }
};

static volatile unsigned long lastInterrupt = 0;
static volatile uint8_t prev = 0xF0;     // assume all high
static volatile uint8_t changed = 0x00;  // assume no change
static volatile bool intTriggered = false;

/**
* ==============================================================================
* -  Interrupt Service Routine  ------------------------------------------------
* ==============================================================================
*/
ISR(PCINT2_vect) {
  unsigned long currInterrupt = micros();

  if (!intTriggered && currInterrupt - lastInterrupt > DEB_THRESH) {
    uint8_t curr = PINK;
    changed = curr ^ prev;
    prev = curr;

    intTriggered = true;  // do not enable interrupts until processed
    lastInterrupt = currInterrupt;
  }
}

/**
* ==============================================================================
* -  Utility Functions  --------------------------------------------------------
* ==============================================================================
*/
uint8_t log2(uint8_t n) {
  uint8_t ret = 0;
  while (n > 0) {
    n >>= 1;
    ret++;
  }
  return ret;
}

uint8_t getKeydown(uint8_t offset) {
  if (changed & (1 << (4 + offset))) {
    PORTK = 0x00;  // turn off rows
    for (int i = 0; i < 4; i++) {
      PORTK = (1 << i);
      if (PINK & (1 << (4 + offset))) {
        uint8_t ret = (1 << i);
        PORTK = 0xF0;
        return ret;
      }
    }
    PORTK = 0xF0;  // turn back on rows
  }
  return 0x00;
}

/**
* ==============================================================================
* -  Arduino Built-Ins  --------------------------------------------------------
* ==============================================================================
*/
void setup() {
  cli();
  DDRK = 0x0F;   // configures direction (input/output)
  PORTK = 0xF0;  // writes output state OR enables internal pull-up resistors
  sei();

  // enable pin change interrupts for Port K
  PCICR |= (1 << PCIE2);
  PCMSK2 = 0xF0;

  Serial.begin(9600);
}

void loop() {
  if (intTriggered) {
    cli();
    intTriggered = false;

    uint8_t col = ((prev ^ changed) & changed) >> 4;
    uint8_t row = getKeydown(0) | getKeydown(1) | getKeydown(2) | getKeydown(3);
    uint8_t kmc = log2(col) - 1;  // key map column
    uint8_t kmr = log2(row) - 1;  // key map row
    if (kmc < 0 || kmc >= 4 || kmr < 0 || kmr >= 4) {
      sei();
      return;
    }

    int freq = FREQ_MAP[log2(row) - 1][log2(col) - 1];
    if (abs(freq) <= 1) {
      Serial.print("Control = ");
    } else {
      Serial.print("Frequency = ");
    }
    Serial.println(freq);

    sei();
  }
}