#include <JeeLib.h>

void setup() {
	// Setup all ports with weak pull-up to save power.
	for (uint8_t i = 0; i <= 13; i++) {
		pinMode(i, INPUT_PULLUP);
	}
	// Blinking LED.
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	Sleepy::loseSomeTime(32/*ms*/); // loseSomeTime() works just like delay().
	digitalWrite(LED_BUILTIN, LOW);
	Sleepy::loseSomeTime(960/*ms*/);
}

ISR(WDT_vect) {
	Sleepy::watchdogEvent();
}
