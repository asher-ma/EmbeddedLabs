#include <Arduino.h>

#include "note_container.h"

/**
 * =============================================================================
 * -  Constants  ---------------------------------------------------------------
 * =============================================================================
 */

static constexpr NoteContainer MARIO[] = {
    NOTE_E3(200), NOTE_E3(200), REST(200),    NOTE_E3(200),
    REST(200),    NOTE_C3(200), NOTE_E3(200), REST(200),
    NOTE_G3(200), REST(600),    NOTE_G2(200), REST(600)
};

static constexpr double CLK_FREQ = 16e6;
static constexpr uint16_t PRESCALE = 64;
static constexpr uint8_t GAP = 20; // Gap between notes in ms

/**
 * =============================================================================
 * -  Globals  -----------------------------------------------------------------
 * =============================================================================
 */

const NoteContainer* curSong = MARIO;
uint8_t curLen = sizeof(MARIO) / sizeof(NoteContainer);
volatile uint8_t ni = 0; // Note index
volatile bool gap = true;

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
	// Set up timer 1 to trigger interrupt designated by TCNT0
	TIMSK1 |= (1 << TOIE1); // initialize timer interrupt

	TCCR1A = TCCR1B = 0; // mode 0
}

/**
 * =============================================================================
 * -  Forward Declarations  ----------------------------------------------------
 * =============================================================================
 */


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
// From 0 to 1000 ms
void setDelay(uint16_t ms) {
	TCNT1 = 0xFFFF - ((ms*1e3)/16 - 1); // TCNT1 = TOP - counts
  	TCCR1B |= (1 << CS12); 				// set prescaler of 256
}

// Plays one note in song then starts delay for note duration
// Designed to be used by delay ISR
void playNote(const NoteContainer* song) {
	playFreq(song[ni].freq); 		// play next note
	setDelay(song[ni].duration); 	// set duration of note
}

/**
 * ==============================================================================
 * -  Interrupt Service Routines ------------------------------------------------
 * ==============================================================================
 */

ISR(TIMER1_OVF_vect) {
	TCCR1B &= ~(1 << CS12); // stop timer 1

	if (gap) {
		stopFreq();		// stop note
		setDelay(GAP);	// add gap between notes
		gap = false;	// clear gap to play note after delay
	} else {
		if (ni >= curLen) {
			stopFreq(); // stop playing
			ni = 255;	// set note index to notify end of song
			return;		// exit ISR
		}
		playNote(curSong);

		ni++; // advance note index
		gap = true; // set gap for next note
	}
}

/**
 * ==============================================================================
 * -  Arduino Built-Ins --------------------------------------------------------
 * ==============================================================================
 */

void setup() {
	Serial.begin(9600);

	initFreqTimer();
	initDelayTimer();

	Serial.println("Playing song");
	setDelay(0); // Start song
}

void loop() {
	if (ni == 255) {
		Serial.println("Finished song");
		ni = 0;
	}
}
