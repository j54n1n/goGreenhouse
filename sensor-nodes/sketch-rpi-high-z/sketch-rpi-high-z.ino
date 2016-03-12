void setup() {
  // Setup all ports with weak pull-up inputs.
  // Usefull to connect the radio module to the Raspberry.
  for (uint8_t i = 0; i <= 19; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  // Enable LED.
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
}

void loop() {
}
