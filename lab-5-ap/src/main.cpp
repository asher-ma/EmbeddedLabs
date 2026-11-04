// ESP32 WiFi Access Point

#include <Arduino.h>
#include <WiFi.h>

#include "song_library.h"

/**
 * =============================================================================
 * -  WiFi AP setup  -----------------------------------------------------------
 * =============================================================================
 */

const char* ssid = "LAB5_ACCESS_POINT";
const char* password = nullptr;

// Set web server port number to 80
WiFiServer server(80);

/**
 * =============================================================================
 * -  Organ setup  -------------------------------------------------------------
 * =============================================================================
 */

static constexpr uint8_t GAP = 20; // gap between notes in ms
static constexpr uint8_t SPEAKER_PIN = 13;

esp_timer_handle_t delayTimer = nullptr;

volatile bool delayOver = false;
const NoteContainer* curSong;
uint8_t curSongLen;
uint8_t ni = 0; // note index

enum class PlaybackState {
	Stopped, Playing, Gap
};

PlaybackState state = PlaybackState::Stopped;

/**
 * =============================================================================
 * -  Timer Initialization -----------------------------------------------------
 * =============================================================================
 */

void initFreqTimer() {
	ledcAttach(SPEAKER_PIN, 1000, 10); // attach pin to LEDC timer
	ledcWriteTone(SPEAKER_PIN, 0); // set timer to 0 hz (off)
}

static void endDelay(void* arg) { // callback function - called when delayTimer ends
	delayOver = true;
}

void initDelayTimer() {
	const esp_timer_create_args_t delayTimerArgs = {
		.callback = &endDelay,
		.name = "delay_timer"
	};

	esp_timer_create(&delayTimerArgs, &delayTimer);
}

/**
 * =============================================================================
 * -  Utility Functions --------------------------------------------------------
 * =============================================================================
 */

void stopFreq() {
	ledcWriteTone(SPEAKER_PIN, 0);
}

void playFreq(uint16_t freq) {
	ledcWriteTone(SPEAKER_PIN, freq);
}

void setDelay(uint16_t ms) {
	const uint64_t us = static_cast<uint64_t>(ms) * 1000; // convert to microseconds
	esp_timer_start_once(delayTimer, us);
}

void playSong(const NoteContainer* song, uint8_t length) {
	if (esp_timer_is_active(delayTimer)) { // if timer running
		esp_timer_stop(delayTimer); // stop timer
	}
	stopFreq();
	ni = 0; // reset note index
	curSong = song;
	curSongLen = length;
	state = PlaybackState::Gap;
	delayOver = true;
	Serial.println("Playing song");
}

/** 
 * ==============================================================================
 * -  Built-In Functions --------------------------------------------------------
 * ==============================================================================
 */

void setup() {
	Serial.begin(115200);

	WiFi.mode(WIFI_AP); // set access point mode
	WiFi.softAP(ssid, password); // set ssid and password for other devices to connect

	server.begin(); // begin listening for incoming clients

	Serial.println("AP setup done");

	initDelayTimer();
	initFreqTimer();

	// delay(2000);
	// playSong(SongLibrary::MARIO, SongLibrary::MARIO_LEN);
}

void loop() {
	WiFiClient client = server.accept(); // client found

	if (client) { // if client exists
		Serial.println("New Client");
		uint8_t numIn = 0; // string to store incoming message

		while (client.connected()) {
			if (client.available()) { // there is incoming data
				char c = client.read(); // read data
				// Serial.write(c); // print data
				numIn += static_cast<uint8_t>(c); 
			}
		}

		if (numIn != 0) {
			switch (numIn) {
				case 1:
					playSong(SongLibrary::MARIO, SongLibrary::MARIO_LEN);
					Serial.println("Playing Mario");
					break;
				case 2:
					playSong(SongLibrary::MEGA, SongLibrary::MEGA_LEN);
					Serial.println("Playing Mega");
					break;
				case 3:
					playSong(SongLibrary::SECRET, SongLibrary::SECRET_LEN);
					Serial.println("Playing Secret");
					break;
				case 4:
					playSong(SongLibrary::STAR, SongLibrary::STAR_LEN);
					Serial.println("Playing Star");
					break;
			}

			numIn = 0;
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