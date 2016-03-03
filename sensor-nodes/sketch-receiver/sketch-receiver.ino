/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Julian Sanin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <SPI.h>
#include <SpiDevice.h>
#include <Rfm69.h>

#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define LED_BUILTIN 9 // On Moteino R4 LED is connected to D9 instead of D13.

struct Packet {
	uint64_t uniqueId;
	uint8_t packetType;
	uint8_t packetVersion;
	uint8_t sequenceNumber;
	int8_t temperature;
} payload;

union UniqueId {
	uint64_t value;
	struct {
		uint32_t lo;
		uint32_t hi;
	} ints;
};

// RFM69HW radio with Slave Select on Arduino pin 10.
Rfm69<SpiDevice<10>, Rfm69ModelHW> rfm69;

uint8_t rxBuffer[64];

void setup() {
	// Blinking LED.
	pinMode(LED_BUILTIN, OUTPUT);

	// RFM69 radio.
	rfm69.init(63, 42, 8686);
	//rfm69.encrypt("mysecret");

	Serial.begin(115200);
	Serial.println("[Rfm69RxDemo]");
}

void loop() {
	int length = rfm69.receive(rxBuffer, sizeof(rxBuffer));
	if (length >= 0) {
		digitalWrite(LED_BUILTIN, HIGH);
		Serial.print(F("OK "));
		Serial.print(length);
		Serial.print(F("byte(s) "));
		for (int i = 0; i < length; ++i) {
			if (i == 2) {
				// Start of payload.
				Serial.print(F(" ["));
			}
			Serial.print(rxBuffer[i] >> 4, HEX);
			Serial.print(rxBuffer[i] & 0xF, HEX);
		}
		Serial.print(F("] ("));
		Serial.print(rfm69.getRssiValue());
		Serial.print(F("dBm"));
		Serial.print(rfm69.afc < 0 ? F("") : F("+"));
		Serial.print(rfm69.afc);
		Serial.print(F(":"));
		Serial.print(rfm69.getLnaGain());
		Serial.println(F("dB)"));

		// Decoded packet.
		Serial.print(F("Packet:"));
		uint8_t parity = ((rxBuffer[0] & 0xC0) >> 6);
		uint8_t header = ((rxBuffer[1] & 0xC0) | (rxBuffer[0] & 0x3F));
		uint8_t sourceId = rxBuffer[1] & 0x3F;
		Serial.print(F(" p="));
		Serial.print(parity, HEX);
		Serial.print(F(" h="));
		Serial.print(header, HEX);
		Serial.print(F(" srcId="));
		Serial.print(sourceId);
		memcpy(&payload, &rxBuffer[2], sizeof(Packet));
		Serial.print(F(" [uniqueId=0x"));
		UniqueId uid;
		uid.value = payload.uniqueId;
		Serial.print(uid.ints.hi, HEX);
		Serial.print(uid.ints.lo, HEX);
		Serial.print(F(" packetType=0x"));
		Serial.print(payload.packetType, HEX);
		Serial.print(F(" packetVersion=0x"));
		Serial.print(payload.packetVersion, HEX);
		Serial.print(F(" sequenceNumber=0x"));
		Serial.print(payload.sequenceNumber, HEX);
		Serial.print(F(" temperature="));
		Serial.print(payload.temperature);
		Serial.print((char)176); // Degree symbol.
		Serial.println(F("C]"));

		digitalWrite(LED_BUILTIN, LOW);
	}
}
