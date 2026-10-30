#include <Arduino.h>
#include <WiFi.h>

// local WiFi hotspot (replace with your own info)
constexpr char* ssid = "LAB5_ACCESS_POINT";
constexpr char* password = nullptr;

WiFiClient client;
IPAddress serverIP;

void getNetworkInfo() {
	if (WiFi.status() == WL_CONNECTED) {
		Serial.print("[*] Network information for ");
		Serial.println(ssid);

		Serial.println("[+] BSSID : " + WiFi.BSSIDstr());
		Serial.print("[+] Gateway IP : ");
		Serial.println(WiFi.gatewayIP());
		serverIP = WiFi.gatewayIP();
		Serial.print("[+] Subnet Mask : ");
		Serial.println(WiFi.subnetMask());
		Serial.println((String) "[+] RSSI : " + WiFi.RSSI() + " dB");
		Serial.print("[+] ESP32 IP : ");
		Serial.println(WiFi.localIP());
	}
}

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

	getNetworkInfo();
	client.connect(serverIP, 80);
}

void loop() {
	if(client.connected()) {
		client.print("Message from ");
		client.print(WiFi.localIP());
		client.print("\n");
	}
	delay(5000);
}