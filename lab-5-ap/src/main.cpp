// ESP32 WiFi Access Point

#include <Arduino.h>

#include <WiFi.h>

const char* ssid = "LAB5_ACCESS_POINT";
const char* password = nullptr;

// Set web server port number to 80
WiFiServer server(80);


void setup() {
	Serial.begin(115200);

	WiFi.mode(WIFI_AP); // set access point mode
	WiFi.softAP(ssid, password); // set ssid and password for other devices to connect

  server.begin(); // begin listening for incoming clients

  Serial.println("AP setup done");
}

void loop() {
	WiFiClient client = server.accept(); // client found

  if (client) {
    Serial.println("New Client");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) { // there is incoming data
        char c = client.read(); // read data
        Serial.write(c); // print data
      }
    }
  }
}