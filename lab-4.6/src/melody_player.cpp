#include <Arduino.h>

#include "song_library.h"

/**
 * ==============================================================================
 * -  Key Pad Layout -----------------------------------------------------------
 * ==============================================================================
 *
 *  +---+---+---+---+                 +---+---+---+---+
 *  | 1 | 2 | 3 | A |                 | M | M | - | - |
 *  +---+---+---+---+                 +---+---+---+---+
 *  | 4 | 5 | 6 | B |                 | - | - | - | - |
 *  +---+---+---+---+  ============>  +---+---+---+---+
 *  | 7 | 8 | 9 | C |                 | - | - | - | - |
 *  +---+---+---+---+                 +---+---+---+---+
 *  | * | 0 | # | D |                 | S | - | - | - |
 *  +---+---+---+---+                 +---+---+---+---+
 *
 * ==============================================================================
 */
static constexpr char MAP[4][4] = {{'1', '2', '3', 'A'},
                                   {'4', '5', '6', 'B'},
                                   {'7', '8', '9', 'C'},
                                   {'*', '0', '#', 'D'}};

/**
 * =============================================================================
 * -  Constants  ---------------------------------------------------------------
 * =============================================================================
 */
static constexpr double CLK_FREQ = 1.6e6;      // 1.6 MHz
static constexpr uint16_t DEB_THRESH = 50000;  // 50 ms
static constexpr uint16_t PRESCALE = 1;

static volatile unsigned long lastInterrupt = 0;
static volatile uint8_t prev = 0xF0;     // assume all high
static volatile uint8_t changed = 0x00;  // assume no change
static volatile bool intTriggered = false;
static volatile bool playingSong = false;

/**
 * ==============================================================================
 * -  Interrupt Service Routine ------------------------------------------------
 * ==============================================================================
 */
ISR(PCINT2_vect) {
	unsigned long currInterrupt = micros();

	if (!intTriggered && !playingSong &&
	    currInterrupt - lastInterrupt > DEB_THRESH) {
		uint8_t curr = PINK;
		changed = curr ^ prev;
		prev = curr;

		intTriggered = true;  // do not enable interrupts until processed
		lastInterrupt = currInterrupt;
	}
}

/**
 * ==============================================================================
 * -  Utility Functions --------------------------------------------------------
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
	OCR1A = static_cast<uint16_t>(
	    CLK_FREQ / static_cast<float>(PRESCALE * freq) - 1);
	TCCR1A |= (1 << COM1A0);  // enable hardware toggle on pin 11
}

void stopFreq() {
	TCCR1A &= ~(1 << COM1A0);
	PORTB &= ~(1 << PORTB5);
}

void playSong(const NoteContainer* song, uint8_t length) {
	for (uint8_t i = 0; i < length; i++) {
		uint16_t total = song[i].duration;

		if (song[i].freq > 0) {
			uint16_t actual = static_cast<uint16_t>(total * 0.90);
			playFreq(song[i].freq);
			delay(actual);
			stopFreq();
			delay(total - actual);
		} else {
			delay(total);
		}
	}
}

/**
 * ==============================================================================
 * -  Arduino Built-Ins --------------------------------------------------------
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
	TCNT1 = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS10);
}

void loop() {
	if (intTriggered) {
		cli();
		intTriggered = false;

		uint8_t col = ((prev ^ changed) & changed) >> 4;
		uint8_t row = getKeydown(0) | getKeydown(1) | getKeydown(2) | getKeydown(3);
		uint8_t kmc = log2(col) - 1;  // key map column
		uint8_t kmr = log2(row) - 1;  // key map row
		sei();
		if (kmc < 0 || kmc >= 4 || kmr < 0 || kmr >= 4) {
			stopFreq();
			return;
		}

		playingSong = true;
		char key = MAP[log2(row) - 1][log2(col) - 1];
		switch (key) {
			case '1':
				playSong(SongLibrary::MARIO, SongLibrary::MARIO_LEN);
				break;
			case '2':
				playSong(SongLibrary::MEGA, SongLibrary::MEGA_LEN);
				break;
			case '*':
				playSong(SongLibrary::STAR, SongLibrary::STAR_LEN);
				break;
			case '#':
				playSong(SongLibrary::SECRET, SongLibrary::SECRET_LEN);
				break;
			default:
				break;
		}
		playingSong = false;
	}
}