#include <stddef.h>
#include <stdint.h>

#include <SPI.h>
#include <SpiDevice.h>
#include <Rfm69.h>

#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define LED_BUILTIN 9 // On Moteino R4 LED is connected to D9 instead of D13.

Rfm69<SpiDevice<10> > rfm69;

uint8_t rxBuffer[64];

void setup() {
	// Blinking LED.
	pinMode(LED_BUILTIN, OUTPUT);

	// RFM69 radio.
	rfm69.init(28, 42, 8686);
	//rfm69.encrypt("mysecret");

	Serial.begin(115200);
	Serial.println("[Rfm69RxDemo]");
}

void loop() {
	int length = rfm69.receive(rxBuffer, sizeof(rxBuffer));
	if (length >= 0) {
		digitalWrite(LED_BUILTIN, HIGH);
		Serial.print("OK ");
		for (int i = 0; i < length; ++i) {
			Serial.print(rxBuffer[i] >> 4, HEX);
			Serial.print(rxBuffer[i] & 0xF, HEX);
		}
		Serial.print(" (");
		Serial.print(rfm69.rssi);
		Serial.print(rfm69.afc < 0 ? "" : "+");
		Serial.print(rfm69.afc);
		Serial.print(":");
		Serial.print(rfm69.lna);
		Serial.println(")");
		digitalWrite(LED_BUILTIN, LOW);
	}
}
