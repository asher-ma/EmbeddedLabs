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
    NOTE_G3(200), REST(600),    NOTE_G2(200), REST(600)};

static constexpr double CLK_FREQ = 1.6e6;
static constexpr uint16_t PRESCALE = 1;

/**
 * =============================================================================
 * -  Forward Declarations  ----------------------------------------------------
 * =============================================================================
 */
void playFreq(uint16_t freq) {
	OCR1A = static_cast<uint16_t>(
	    CLK_FREQ / static_cast<float>(2 * PRESCALE * freq) - 1);
	TCCR1A |= (1 << COM1A0);  // enable hardware toggle on pin 11
}

void stopFreq() {
	TCCR1A &= ~(1 << COM1A0);
	PORTB &= ~(1 << PORTB5);
}

/**
 * ==============================================================================
 * -  Arduino Built-Ins --------------------------------------------------------
 * ==============================================================================
 */
void setup() {
	Serial.begin(9600);

	DDRB |= (1 << DDB5);

	// Timer 1 control registers
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS10);

	Serial.println("Playing song");
	for (int ni = 0; ni < SONG_LENGTH; ni++) {
		uint16_t total = SONG[ni].duration;
		uint16_t actual = static_cast<uint16_t>(total * 0.90);

		if (SONG[ni].freq > 0) {
			playFreq(SONG[ni].freq);
			delay(actual);
			stopFreq();
			delay(total - actual);
		} else {
			delay(total);
		}
	}
	Serial.println("Finished song");
}

void loop() {}
