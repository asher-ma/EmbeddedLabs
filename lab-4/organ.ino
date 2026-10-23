/**
* Timer Lab
*/
#include "notes.h"

#define CLK_FREQ 1.6e6    // 1.6 MHz
#define DEB_THRESH 50000  // 50 ms
#define PRESCALE 1

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
  { NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3 },
  { NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3 },
  { NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3 },
  { NOTE_C4, -1, 0, 1 }
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

void playFreq(uint16_t freq) {
  OCR1A = static_cast<uint16_t>(CLK_FREQ / static_cast<float>(2 * PRESCALE * freq) - 1);
  TCCR1A |= (1 << COM1A0);  // enable hardware toggle on pin 11
}

void stopFreq() {
  TCCR1A &= ~(1 << COM1A0);
  PORTB &= ~(1 << PORTB5);
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

  // Set Pin 11 (PB5) as output
  DDRB |= (1 << DDB5);

  // Timer 1 control registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS10);

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
      stopFreq();
      sei();
      return;
    }

    int freq = FREQ_MAP[log2(row) - 1][log2(col) - 1];
    if (abs(freq) <= 1) {
      Serial.print("Control = ");
    } else {
      playFreq(freq);
      Serial.print("Frequency = ");
    }
    Serial.println(freq);

    sei();
  }
}