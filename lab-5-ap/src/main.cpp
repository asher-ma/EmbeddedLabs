#include <Arduino.h>

#include <WiFi.h>

const char* ssid = "LAB5_ACCESS_POINT";
const char* password = nullptr;

// Set web server port number to 80
WiFiServer server(80);


void setup() {
	Serial.begin(115200);

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password);

  server.begin();

  Serial.println("AP setup done");
}

void loop() {
	WiFiClient client = server.accept();

  if (client) {
    Serial.println("New Client");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }
  }
}