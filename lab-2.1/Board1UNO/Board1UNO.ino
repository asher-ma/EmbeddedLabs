// This will run on board 1 (UNO), connected to board 2 only
// Read temperature and light sensors and relay data over UART serial line to board 2 when prompted

const long baud = 115200;

// Sensor pins
const int tempSensorPin = 0;
const int photocellPin = 1;

// Message framing
const char startChar = '<';
const char endChar = '>';


void setup() {
  Serial.begin(baud);
}

void loop() {
  if (Serial.available() > 0) {
    char pcChar = Serial.read();

    switch (pcChar) {
      case 't':
        {
          float temp = tempReading();
          Serial.print(startChar);
          Serial.print('t');
          Serial.print(tempReading(), 2);
          Serial.print(endChar);
          break;
        }
      case 'b':
        {
          int brightness = brightnessReading();
          Serial.print(startChar);
          Serial.print('b');
          Serial.print(brightnessReading());
          Serial.print(endChar);
          break;
        }
    }
  }
}


float tempReading() {
  int reading = analogRead(tempSensorPin);

  float voltage = reading * 5.0;
  voltage /= 1023;

  return (voltage - 0.5) * 100;
}

int brightnessReading() {
  int reading = analogRead(photocellPin);
  return reading;
}


