// This will run on board 2, connected in series between board 1 and pc
// Purpose to take pc input, prompted board 1 for sensor reading, then relay reading to pc
const long baud = 115200;

// Temperature setting var
bool celsius = true;

// Incoming message reading
bool messageReady = false;
String currentMessage = String("");
bool messageStarted = false;

// Establish message framing
const char startChar = '<';
const char endChar = '>';


void setup() {
  // PC communication
  Serial.begin(baud);
  // Board 1 communication
  Serial1.begin(baud);
}


void loop() {
  // Receive from pc
  if (Serial.available() > 0){
    // Read single char
    char pcChar = Serial.read();
    switch(pcChar){
      // If 'c' or 'f' just change board 2 global var
      case 'c': {
        celsius = true;
        Serial.println("Temperature reading in celsius");
        break;
      }
      case 'f': {
        celsius = false;
        Serial.println("Temperature reading in fahrenheit");
        break;
      }
      // If 't' or 'b' send to board 1
      case 't': {
          Serial1.print(pcChar);
        break;
      }
      case 'b': {
          Serial1.print(pcChar);
        break;
      }
    }
  }

  // Receive from board 1
  receiveMessage();
  if (messageReady) {
    if (currentMessage.charAt(0) == 't') {
      // Get temperature as float from currentMessage - first char
      float temp = currentMessage.substring(1).toFloat();
      
      // Convert to F if necessary
      if (celsius == false) {
        temp = (temp*9/5) + 32;
      }
      Serial.print("Temperature: ");
      Serial.print(temp, 0);
      if (celsius == true) {
        Serial.println(" degrees celsius");
      } else {
        Serial.println(" degrees fahrenheit");
      }
    } else if (currentMessage.charAt(0) == 'b') {
      // Get brightness as float from currentMessage - first char
      float brightness = currentMessage.substring(1).toFloat();
      Serial.print("Brightness: ");
      Serial.println(brightness, 0);
    }
    // Reset message
    messageReady = false;
    messageStarted = false;
    currentMessage = "";
  }
}


void receiveMessage(){
  // If there is no finished message, read input
  while (Serial1.available() > 0 && messageReady == false) {
    char cur = Serial1.read();
    if (messageStarted == true) { // If the message has started, read it
      if (cur == endChar) { // If end character, message is ready
        messageReady = true;
      } else { // Transcribe to currentMessage buffer
        currentMessage.concat(cur);
      }
    } else if (cur == startChar) { // If start character, start message
      messageStarted = true;
    }
  }
}







