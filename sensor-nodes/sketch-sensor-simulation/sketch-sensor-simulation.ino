#include <stddef.h>
#include <stdint.h>

#include <JeeLib.h>

#include <SPI.h>
#include <SpiDevice.h>
#include <Rfm69.h>

#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define LED_BUILTIN 9 // On Moteino R4 LED is connected to D9 instead of D13.

// RFM69 radio with Slave Select on Arduino pin 10.
Rfm69<SpiDevice<10> > rfm69;

uint8_t txBuffer[62];

void setup() {
	// Setup all ports with weak pull-up to save power.
	for (uint8_t i = 0; i <= 13; i++) {
		pinMode(i, INPUT_PULLUP);
	}

	// Blinking LED.
	pinMode(LED_BUILTIN, OUTPUT);

	// RFM69 radio.
	rfm69.init(28, 42, 8686);
	//rfm69.encrypt("mysecret");
	//rfm69.txPower(15); // 0 = min .. 31 = max
	rfm69.sleep();
	for (size_t i = 0; i < sizeof(txBuffer); i++)
		txBuffer[i] = (uint8_t)i;
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);

	size_t txLength = ++txBuffer[0] % (sizeof(txBuffer) + 1);
	rfm69.send(0, txBuffer, txLength);
	rfm69.sleep();

	digitalWrite(LED_BUILTIN, LOW);
	Sleepy::loseSomeTime(960/*ms*/); // loseSomeTime() works just like delay().
}

ISR(WDT_vect) {
	Sleepy::watchdogEvent();
}
