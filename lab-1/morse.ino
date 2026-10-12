void morse(int n) {
  int curr = 10 - n;

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    if ((curr + i) % 10 >= 5) {
      delay(250);
    } else {
      delay(750);
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(750);
  }

  delay(1750);
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

  // we are lab station 5, so we display 19
  morse(1);
  morse(9);
}
