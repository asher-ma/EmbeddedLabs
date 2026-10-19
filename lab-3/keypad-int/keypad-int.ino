// Arduino MEGA
// Using keypad with interrupts
// Using pins PK0-PK7 (D62-D69)


// Pin reference vars
volatile uint8_t& KEY_DDR = DDRK;  // Register to write pin mode (input/output)
// 1 = output, 0 = input
volatile uint8_t& KEY_PRT = PORTK;  // Register to write pin behavior
// When in output mode: sets pin to HIGH or LOW
// When in input mode: enables internal pull up resistor
volatile uint8_t& KEY_PIN = PINK;  // Register to read pin states


// Keypad lookup table
unsigned char keypad[4][4] = {
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};

const uint8_t DEBOUNCE = 20;
volatile uint32_t prevTime = millis();

volatile char keypadOut = 0;

void setup() {
  Serial.begin(9600);

  // Rows are left 4 bits, cols are right 4
  KEY_DDR = 0xF0;  // Set rows to output and cols to input
  KEY_PRT = 0x0F;  // Ground all rows and enable col pull up resistors

  // Set up interrupts
  cli();                 // No interrupts
  PCICR |= (1 << PCIE2); // In the Pin Change Interrupt Control Register, enable PCINT2
  PCMSK2 = 0x0F;         // Allow columns to trigger interrupt
  sei();                 // Enable interrupts
}

void loop() {
  // Print key if pressed
  if (keypadOut != 0){
    Serial.print(keypadOut);
    keypadOut = 0;
    Serial.print(", ");
  }
}

ISR(PCINT2_vect) {
  cli(); // No interrupts
  uint32_t time = millis();
  if (time > (prevTime + DEBOUNCE)) {
    uint8_t interruptCol = 0x0F & ~KEY_PIN; // Read which col caused the interrupt
    if (interruptCol != 0) {
      // Get col location in keypad dict
      uint8_t colloc = 0xFF;
      if (interruptCol & 0x01) {
        colloc = 3;
      } else if (interruptCol & 0x02) {
        colloc = 2;
      } else if (interruptCol & 0x04) {
        colloc = 1;
      } else if (interruptCol & 0x08) {
        colloc = 0;
      }

      // Find which row was pressed
      uint8_t rowloc = 0xFF;
      for (uint8_t i = 0; i < 4; i++) {
        uint8_t curRow = (0x80 >> i);         // Step through rows
        KEY_DDR = curRow;                     // Set ONLY current row to output
        delay(1);                             // Allow pins to set
        uint8_t readBack = (KEY_PIN & 0x0F);  // Read columns
        if (readBack != 0x0F) {               // Row found
          rowloc = i;
          break;                              // Exit loop
        }
      }

      // Record result
      keypadOut = keypad[rowloc][colloc];

      // Restore detection state
      KEY_DDR = 0xF0; // Set rows to output and cols to input
      KEY_PRT = 0x0F; // Ground all rows and enable col pull up resistors
    }
    prevTime = time;
  }
  sei(); // Enable interrupts
}
