#include <Arduino.h>

#include "note_container.h"

/**
 * =============================================================================
 * -  Constants  ---------------------------------------------------------------
 * =============================================================================
 */
static constexpr uint8_t SONG_LENGTH = 12;
static constexpr NoteContainer SONG[] = {
    NOTE_E3(200), NOTE_E3(200), REST(200),    NOTE_E3(200),
    REST(200),    NOTE_C3(200), NOTE_E3(200), REST(200),
    NOTE_G3(200), REST(600),    NOTE_G2(200), REST(600)
};

static constexpr double CLK_FREQ = 1.6e6;
static constexpr uint16_t PRESCALE = 1;
static constexpr uint8_t GAP = 20; // Gap between notes in ms

/**
 * =============================================================================
 * -  Globals  -----------------------------------------------------------------
 * =============================================================================
 */

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
	TCCR3B |= (1 << CS30); // no Prescaling

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
void playFreq(uint16_t freq) {
	OCR3A = static_cast<uint16_t>(
		CLK_FREQ / static_cast<float>(2 * PRESCALE * freq) - 1);
	TCCR3A |= (1 << COM3A0);  // enable hardware toggle to OC3A
}

void stopFreq() {
	TCCR3A &= ~(1 << COM3A0); 	// disconnect timer from pin
	PORTE &= ~(1 << PORTE3); 	// pull pin down
}

void setDelay(uint16_t ms) {
	TCNT1 = 0xFFFF - ((ms*1e3)/16 - 1); // TCNT1 = TOP - counts
  	TCCR1B |= (1 << CS12); 				// set prescaler of 256
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
		gap = false;
	} else {
		if (ni >= SONG_LENGTH) {
			stopFreq(); // stop playing
			ni = -1;	// set note index to notify end of song
			return;		// exit ISR
		}
		playFreq(SONG[ni].freq); 		// play next note
		setDelay(SONG[ni].duration); 	// set duration of note

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
	Serial.println(TCNT1);
	setDelay(1); // start song
	Serial.println(TCNT1);
	// for (int ni = 0; ni < SONG_LENGTH; ni++) {
	// 	uint16_t total = SONG[ni].duration;
	// 	uint16_t actual = static_cast<uint16_t>(total * 0.90);

	// 	if (SONG[ni].freq > 0) {
	// 		playFreq(SONG[ni].freq);
	// 		delay(actual);
	// 		stopFreq();
	// 		delay(total - actual);
	// 	} else {
	// 		delay(total);
	// 	}
	// }
}

void loop() {
	if (gap) {
		Serial.println(ni);
	}
	if (ni == -1) {
		Serial.println("Finished song");
		ni = 0;
	}
}
