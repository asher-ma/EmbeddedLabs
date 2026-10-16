static void record_time();  // forward declaration of ISR

/**
* ==============================================================================
* -  CONSTANTS  ----------------------------------------------------------------
* ==============================================================================
*/
constexpr static float DIAMETER = 660.4 / 1e6;           // diameter in kilometers
constexpr static float CIRCUMFERENCE = M_PI * DIAMETER;  // circumference in kilometers
constexpr static float MICRO_PER_HOUR = 3.6 * 1e9;       // conversion factor from microseconds to hours
constexpr static int INT_PIN = 21;                       // interrupt pin

/**
* ==============================================================================
* -  VOLATILE VARIABLES  -------------------------------------------------------
* ==============================================================================
*/
volatile static unsigned long start_time;
volatile static unsigned long end_time;

void setup() {
  Serial.begin(9600);  //serial monitor

  start_time = 0;
  end_time = 0;
  pinMode(INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), record_time, RISING);
}

void loop() {
  Serial.print("Speed: ");
  Serial.print(CIRCUMFERENCE / (end_time - start_time) * MICRO_PER_HOUR);
  Serial.println(" km per hour");

  // reset if no input for 5 seconds
  unsigned long current_time = micros();
  if (current_time - end_time > static_cast<unsigned long>(5 * 1e6)) {
    start_time = 0;
    end_time = 0;
  }
}


/**
* ==============================================================================
* -  Interrupt Service Routing  ------------------------------------------------
* ==============================================================================
*/
static void record_time() {
  noInterrupts();
  unsigned long temp_time = micros();

  // ignore if current reading occurs too early (repeated readings)
  if (temp_time - end_time <= 50 * 10e3) {
    interrupts();
    return;
  }

  start_time = end_time;  // previous reading
  end_time = temp_time;   // most recent reading
  interrupts();
}