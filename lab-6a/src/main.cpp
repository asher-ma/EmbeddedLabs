#include <Arduino.h>

static constexpr uint16_t PULSE_MIN = 2000U;
static constexpr uint16_t PULSE_MAX = 4000U;
static constexpr uint16_t PULSE_MID = 3000U;

static inline void initTimer1Servo(void) {
	DDRB = (1 << PB5);
	TCCR1A = (1 << COM1A1) | (1 << WGM11);  // direct output on OC1A, fast PWM
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);  // prescaler 8
	ICR1 = 40000;
}

static inline void showOff(void) {
	Serial.println("Center");
	OCR1A = PULSE_MID;
	delay(1500);

	Serial.println("Clockwise Max");
	OCR1A = PULSE_MIN;
	delay(1500);

	Serial.println("Counterclockwise Max");
	OCR1A = PULSE_MAX;
	delay(1500);

	Serial.println("Center");
	OCR1A = PULSE_MID;
	delay(1500);
}

void setup() {
	OCR1A = PULSE_MID; /* set it to middle position initially */
	initTimer1Servo();
	Serial.begin(115200);

	Serial.println("Welcome to the Servo Demo");
	delay(3000);

	showOff();
}

void loop() {
	// nothing
}