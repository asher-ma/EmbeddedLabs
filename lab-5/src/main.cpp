#include <Arduino.h>
#include <WiFi.h>

// local WiFi hotspot (replace with your own info)
constexpr char* ssid = "USSGSSMASTER-X";
constexpr char* password = "446Kq?08";

void setup() {
	Serial.begin(115200);
	delay(1000);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	Serial.print("Connecting");

	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(100);
	}

	Serial.print("\nConnected to local WiFi network (");
	Serial.print(ssid);
	Serial.println(")");

	Serial.print("Local ESP32 IP: ");
	Serial.println(WiFi.localIP());
}

void loop() {
	// TODO loop
}