#ifndef NOTE_CONTAINER_H
#define NOTE_CONTAINER_H

#include <Arduino.h>

// Simple note container
struct NoteContainer {
	uint16_t freq;
	uint16_t duration;
};

#define NOTE_C0(x) \
	NoteContainer { .freq = 16, .duration = x }
#define NOTE_CS0(x) \
	NoteContainer { .freq = 17, .duration = x }
#define NOTE_D0(x) \
	NoteContainer { .freq = 18, .duration = x }
#define NOTE_DS0(x) \
	NoteContainer { .freq = 19, .duration = x }
#define NOTE_E0(x) \
	NoteContainer { .freq = 20, .duration = x }
#define NOTE_F0(x) \
	NoteContainer { .freq = 21, .duration = x }
#define NOTE_FS0(x) \
	NoteContainer { .freq = 23, .duration = x }
#define NOTE_G0(x) \
	NoteContainer { .freq = 24, .duration = x }
#define NOTE_GS0(x) \
	NoteContainer { .freq = 25, .duration = x }
#define NOTE_A0(x) \
	NoteContainer { .freq = 27, .duration = x }
#define NOTE_AS0(x) \
	NoteContainer { .freq = 29, .duration = x }
#define NOTE_B0(x) \
	NoteContainer { .freq = 30, .duration = x }
#define NOTE_C1(x) \
	NoteContainer { .freq = 32, .duration = x }
#define NOTE_CS1(x) \
	NoteContainer { .freq = 34, .duration = x }
#define NOTE_D1(x) \
	NoteContainer { .freq = 36, .duration = x }
#define NOTE_DS1(x) \
	NoteContainer { .freq = 38, .duration = x }
#define NOTE_E1(x) \
	NoteContainer { .freq = 41, .duration = x }
#define NOTE_F1(x) \
	NoteContainer { .freq = 43, .duration = x }
#define NOTE_FS1(x) \
	NoteContainer { .freq = 46, .duration = x }
#define NOTE_G1(x) \
	NoteContainer { .freq = 49, .duration = x }
#define NOTE_GS1(x) \
	NoteContainer { .freq = 51, .duration = x }
#define NOTE_A1(x) \
	NoteContainer { .freq = 55, .duration = x }
#define NOTE_AS1(x) \
	NoteContainer { .freq = 58, .duration = x }
#define NOTE_B1(x) \
	NoteContainer { .freq = 61, .duration = x }
#define NOTE_C2(x) \
	NoteContainer { .freq = 65, .duration = x }
#define NOTE_CS2(x) \
	NoteContainer { .freq = 69, .duration = x }
#define NOTE_D2(x) \
	NoteContainer { .freq = 73, .duration = x }
#define NOTE_DS2(x) \
	NoteContainer { .freq = 77, .duration = x }
#define NOTE_E2(x) \
	NoteContainer { .freq = 82, .duration = x }
#define NOTE_F2(x) \
	NoteContainer { .freq = 87, .duration = x }
#define NOTE_FS2(x) \
	NoteContainer { .freq = 92, .duration = x }
#define NOTE_G2(x) \
	NoteContainer { .freq = 98, .duration = x }
#define NOTE_GS2(x) \
	NoteContainer { .freq = 103, .duration = x }
#define NOTE_A2(x) \
	NoteContainer { .freq = 110, .duration = x }
#define NOTE_AS2(x) \
	NoteContainer { .freq = 116, .duration = x }
#define NOTE_B2(x) \
	NoteContainer { .freq = 123, .duration = x }
#define NOTE_C3(x) \
	NoteContainer { .freq = 130, .duration = x }
#define NOTE_CS3(x) \
	NoteContainer { .freq = 138, .duration = x }
#define NOTE_D3(x) \
	NoteContainer { .freq = 146, .duration = x }
#define NOTE_DS3(x) \
	NoteContainer { .freq = 155, .duration = x }
#define NOTE_E3(x) \
	NoteContainer { .freq = 164, .duration = x }
#define NOTE_F3(x) \
	NoteContainer { .freq = 174, .duration = x }
#define NOTE_FS3(x) \
	NoteContainer { .freq = 184, .duration = x }
#define NOTE_G3(x) \
	NoteContainer { .freq = 196, .duration = x }
#define NOTE_GS3(x) \
	NoteContainer { .freq = 207, .duration = x }
#define NOTE_A3(x) \
	NoteContainer { .freq = 220, .duration = x }
#define NOTE_AS3(x) \
	NoteContainer { .freq = 233, .duration = x }
#define NOTE_B3(x) \
	NoteContainer { .freq = 246, .duration = x }
#define NOTE_C4(x) \
	NoteContainer { .freq = 261, .duration = x }
#define NOTE_CS4(x) \
	NoteContainer { .freq = 277, .duration = x }
#define NOTE_D4(x) \
	NoteContainer { .freq = 293, .duration = x }
#define NOTE_DS4(x) \
	NoteContainer { .freq = 311, .duration = x }
#define NOTE_E4(x) \
	NoteContainer { .freq = 329, .duration = x }
#define NOTE_F4(x) \
	NoteContainer { .freq = 349, .duration = x }
#define NOTE_FS4(x) \
	NoteContainer { .freq = 369, .duration = x }
#define NOTE_G4(x) \
	NoteContainer { .freq = 392, .duration = x }
#define NOTE_GS4(x) \
	NoteContainer { .freq = 415, .duration = x }
#define NOTE_A4(x) \
	NoteContainer { .freq = 440, .duration = x }
#define NOTE_AS4(x) \
	NoteContainer { .freq = 466, .duration = x }
#define NOTE_B4(x) \
	NoteContainer { .freq = 493, .duration = x }
#define NOTE_C5(x) \
	NoteContainer { .freq = 523, .duration = x }
#define NOTE_CS5(x) \
	NoteContainer { .freq = 554, .duration = x }
#define NOTE_D5(x) \
	NoteContainer { .freq = 587, .duration = x }
#define NOTE_DS5(x) \
	NoteContainer { .freq = 622, .duration = x }
#define NOTE_E5(x) \
	NoteContainer { .freq = 659, .duration = x }
#define NOTE_F5(x) \
	NoteContainer { .freq = 698, .duration = x }
#define NOTE_FS5(x) \
	NoteContainer { .freq = 739, .duration = x }
#define NOTE_G5(x) \
	NoteContainer { .freq = 784, .duration = x }
#define NOTE_GS5(x) \
	NoteContainer { .freq = 830, .duration = x }
#define NOTE_A5(x) \
	NoteContainer { .freq = 880, .duration = x }
#define NOTE_AS5(x) \
	NoteContainer { .freq = 932, .duration = x }
#define NOTE_B5(x) \
	NoteContainer { .freq = 987, .duration = x }
#define NOTE_C6(x) \
	NoteContainer { .freq = 1046, .duration = x }
#define NOTE_CS6(x) \
	NoteContainer { .freq = 1108, .duration = x }
#define NOTE_D6(x) \
	NoteContainer { .freq = 1174, .duration = x }
#define NOTE_DS6(x) \
	NoteContainer { .freq = 1244, .duration = x }
#define NOTE_E6(x) \
	NoteContainer { .freq = 1318, .duration = x }
#define NOTE_F6(x) \
	NoteContainer { .freq = 1396, .duration = x }
#define NOTE_FS6(x) \
	NoteContainer { .freq = 1479, .duration = x }
#define NOTE_G6(x) \
	NoteContainer { .freq = 1568, .duration = x }
#define NOTE_GS6(x) \
	NoteContainer { .freq = 1661, .duration = x }
#define NOTE_A6(x) \
	NoteContainer { .freq = 1760, .duration = x }
#define NOTE_AS6(x) \
	NoteContainer { .freq = 1864, .duration = x }
#define NOTE_B6(x) \
	NoteContainer { .freq = 1975, .duration = x }
#define NOTE_C7(x) \
	NoteContainer { .freq = 2093, .duration = x }
#define NOTE_CS7(x) \
	NoteContainer { .freq = 2217, .duration = x }
#define NOTE_D7(x) \
	NoteContainer { .freq = 2349, .duration = x }
#define NOTE_DS7(x) \
	NoteContainer { .freq = 2489, .duration = x }
#define NOTE_E7(x) \
	NoteContainer { .freq = 2636, .duration = x }
#define NOTE_F7(x) \
	NoteContainer { .freq = 2793, .duration = x }
#define NOTE_FS7(x) \
	NoteContainer { .freq = 2959, .duration = x }
#define NOTE_G7(x) \
	NoteContainer { .freq = 3136, .duration = x }
#define NOTE_GS7(x) \
	NoteContainer { .freq = 3322, .duration = x }
#define NOTE_A7(x) \
	NoteContainer { .freq = 3520, .duration = x }
#define NOTE_AS7(x) \
	NoteContainer { .freq = 3729, .duration = x }
#define NOTE_B7(x) \
	NoteContainer { .freq = 3951, .duration = x }
#define NOTE_C8(x) \
	NoteContainer { .freq = 4186, .duration = x }
#define NOTE_CS8(x) \
	NoteContainer { .freq = 4434, .duration = x }
#define NOTE_D8(x) \
	NoteContainer { .freq = 4698, .duration = x }
#define NOTE_DS8(x) \
	NoteContainer { .freq = 4978, .duration = x }
#define NOTE_E8(x) \
	NoteContainer { .freq = 5273, .duration = x }
#define NOTE_F8(x) \
	NoteContainer { .freq = 5587, .duration = x }
#define NOTE_FS8(x) \
	NoteContainer { .freq = 5919, .duration = x }
#define NOTE_G8(x) \
	NoteContainer { .freq = 6272, .duration = x }
#define NOTE_GS8(x) \
	NoteContainer { .freq = 6644, .duration = x }
#define NOTE_A8(x) \
	NoteContainer { .freq = 7040, .duration = x }
#define NOTE_AS8(x) \
	NoteContainer { .freq = 7458, .duration = x }
#define NOTE_B8(x) \
	NoteContainer { .freq = 7902, .duration = x }
#define REST(x) \
	NoteContainer { .freq = 0, .duration = x }

#endif  // NOTE_CONTAINER_H