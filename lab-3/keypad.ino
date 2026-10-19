/**
* Interrupt Lab - Extra Credit (3.1)
*
* Summary
* TODO
*
*
* Rows will be set to HIGH, columns will be set to LOW.
*/


/**
* ==============================================================================
* -  Keypad Pins  --------------------------------------------------------------
* ==============================================================================
*/
#define R1_PIN A8   // PK0 (PCINT16)
#define R2_PIN A9   // PK1 (PCINT17)
#define R3_PIN A10  // PK2 (PCINT18)
#define R4_PIN A11  // PK3 (PCINT19)
#define C1_PIN A12  // PK4 (PCINT20)
#define C2_PIN A13  // PK5 (PCINT21)
#define C3_PIN A14  // PK6 (PCINT22)
#define C4_PIN A15  // PK7 (PCINT23)

#define DEB_THRESH 50000  // 50 ms

/**
* ==============================================================================
* -  Constants  ----------------------------------------------------------------
* ==============================================================================
*/
static const char KEY_MAP[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
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
  cli();
  unsigned long currInterrupt = micros();

  if (currInterrupt - lastInterrupt > DEB_THRESH) {
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
    intTriggered = false;

    uint8_t col = ((prev ^ changed) & changed) >> 4;
    uint8_t row = getKeydown(0) | getKeydown(1) | getKeydown(2) | getKeydown(3);
    uint8_t kmc = log2(col) - 1;  // key map column
    uint8_t kmr = log2(row) - 1;  // key map row
    if (kmc < 0 || kmc >= 4 || kmr < 0 || kmr >= 4) {
      sei();
      return;
    }

    Serial.print("Key changed = ");
    Serial.println(KEY_MAP[log2(row) - 1][log2(col) - 1]);

    sei();
  }
}