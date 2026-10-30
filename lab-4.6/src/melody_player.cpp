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

static constexpr double CLK_FREQ = 16e6;      // 16 MHz
static constexpr uint16_t DEB_THRESH = 50000;  // 50 ms
static constexpr uint16_t PRESCALE = 64;
static constexpr uint8_t GAP = 20; // Gap between notes in ms

/**
 * =============================================================================
 * -  Globals  -----------------------------------------------------------------
 * =============================================================================
 */

static volatile unsigned long lastInterrupt = 0;
static volatile uint8_t prev = 0xF0;     // assume all high
static volatile uint8_t changed = 0x00;  // assume no change
static volatile bool intTriggered = false;

volatile bool delayOver = false;
const NoteContainer* curSong;
uint8_t curSongLen;
uint8_t ni = 0; // Note index

enum class PlaybackState {
	Stopped, Playing, Gap
};

volatile PlaybackState state = PlaybackState::Stopped;

/**
 * =============================================================================
 * -  Timer Initializations ----------------------------------------------------
 * =============================================================================
 */

void initFreqTimer() {
	// Setup timer 3 to generate a frequency designated by OCR3A
	// Timer 3 control registers
	TCCR3A = 0;
	TCCR3B = 0;
	TCNT3 = 0;
	TCCR3B |= (1 << WGM32); // mode 4
	TCCR3B |= (1 << CS30) | (1 << CS31); // Set Timer 3 prescaler of 64

	DDRE |= (1 << PE3); // enable pin output to OC3A (PE3) (P5)
}

void initDelayTimer() {
	// Set up timer 1 to trigger interrupt designated by TCNT1
	TIMSK1 |= (1 << TOIE1); // initialize timer interrupt

	TCCR1A = TCCR1B = 0; // mode 0
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

void stopFreq() {
	TCCR3A &= ~(1 << COM3A0); 	// disconnect timer from pin
	PORTE &= ~(1 << PORTE3); 	// pull pin down
}

void playFreq(uint16_t freq) {
	if (freq == 0) {
		stopFreq();
	} else {
		TCNT3 = 0; // set timer counter register to bottom 
		OCR3A = static_cast<uint16_t>(
			CLK_FREQ / static_cast<float>(2 * PRESCALE * freq) - 1);
		TCCR3A |= (1 << COM3A0);  // enable hardware toggle on OC3A
	}
}

// Triggers timer 1 ISR after given delay
// From 0 to 1048 ms
void setDelay(uint16_t ms) {
	TCNT1 = 0xFFFF - ((ms*1e3)/16 - 1); // TCNT1 = TOP - counts
  	TCCR1B |= (1 << CS12); 				// set prescaler of 256
	delayOver = false;
}

void playSong(const NoteContainer* song, uint8_t length) {
	cli();
	TCCR1B &= ~(1 << CS12); // stop delay timer
	stopFreq();
	ni = 0; // reset note index
	curSong = song;
	curSongLen = length;
	state = PlaybackState::Gap;
	delayOver = true;
	sei();
	Serial.println("Playing song");
}

/** 
 * ==============================================================================
 * -  Interrupt Service Routines -----------------------------------------------
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

ISR(TIMER1_OVF_vect) {
	TCCR1B &= ~(1 << CS12); // stop timer 1

	delayOver = true; // set flag to indicate delay is over
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

	// enable pin change interrupts for Port K
	PCICR |= (1 << PCIE2);
	PCMSK2 = 0xF0;

	Serial.begin(9600);

	initFreqTimer();
	initDelayTimer();
	sei();
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
			return;
		}

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
	}

	if (delayOver) {
		delayOver = false;
		switch (state) {
			case PlaybackState::Stopped:
				break;
			case PlaybackState::Gap:
				if (ni >= curSongLen) { // song is over
					stopFreq();
					ni = 0;
					Serial.println("Finished song");
					state = PlaybackState::Stopped;
				} else {
					playFreq(curSong[ni].freq); 	// play next note
					setDelay(curSong[ni].duration); // set duration of note
					ni++; // advance note index
					state = PlaybackState::Playing;
				}
				break;
			case PlaybackState::Playing:
				stopFreq();		// stop note
				setDelay(GAP);	// add gap between notes
				state = PlaybackState::Gap;
				break;
		}
	}
}