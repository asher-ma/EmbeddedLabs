// This will run on board 1 (UNO), connected to board 2 only
// Read temperature and light sensors and relay data over UART serial line to board 2 when prompted

const int baud = 9600;

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

    // Create buffer to hold outgoing string
    char txBuffer[32];

    switch (pcChar) {
      case 't':
        {
          int temp = tempReading();
          snprintf(txBuffer, sizeof(txBuffer), "%c%c%i%c", startChar, 't', temp, endChar);
          // Serial.print(startChar);
          // Serial.print("t");
          // Serial.print(tempReading(), 2);
          // Serial.print(endChar);
          Serial.print(txBuffer);
          break;
        }
      case 'b':
        {
          int brightness = brightnessReading();
          snprintf(txBuffer, sizeof(txBuffer), "%c%c%i%c", startChar, 'b', brightness, endChar);
          // Serial.print(startChar);
          // Serial.print("b");
          // Serial.print(brightnessReading(), 0);
          // Serial.print(endChar);
          Serial.print(txBuffer);
          break;
        }
      default:
        Serial.println("It works?");
        break;
    }
  }
}


int tempReading() {
  int reading = analogRead(tempSensorPin);

  float voltage = reading * 5.0;
  voltage /= 1023;

  return int((voltage - 0.5) * 100);
}

int brightnessReading() {
  int reading = analogRead(photocellPin);
  return reading;
}


