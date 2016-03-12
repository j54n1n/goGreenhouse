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

#include <JeeLib.h>

#include <SPI.h>
#include <SpiDevice.h>
#include <SpiFlash.h>
#include <Rfm69.h>
#include <Rfm69Packet.h>

#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define LED_BUILTIN 9 // On Moteino R4 LED is connected to D9 instead of D13.

// RFM69HW radio with Slave Select on Arduino pin 10.
Rfm69<SpiDevice<10>, Rfm69ModelHW> rfm69;

// W25X40CL flash memory with Slave Select on Arduino pin 8.
SpiFlash<SpiDevice<8> > flash;

using namespace Rfm69Packet;
Packet_NodeTemperature nodeTemperature;

void setup() {
	// Setup all ports with weak pull-up to save power.
	for (uint8_t i = 0; i <= 19; i++) {
		pinMode(i, INPUT_PULLUP);
	}

	// Blinking LED.
	pinMode(LED_BUILTIN, OUTPUT);

	// RFM69 radio.
	rfm69.init(61, 42, 8686);
	//rfm69.encrypt("mysecret");
	//rfm69.setTransmitPower(+17/*dBm*/);
	rfm69.sleep();

	// Data packets.
	Packet_init((Packet*)&nodeTemperature, PacketType_NodeTemperature);

	// Flash memory.
	flash.init();
	nodeTemperature.uniqueId = flash.getUniqueId();
	flash.sleep();
}

void loop() {
	const uint32_t timePeriodMillis = 1000; // Transmit at least every second.
	const uint32_t timeStart = millis();
	digitalWrite(LED_BUILTIN, HIGH);
	nodeTemperature.temperature = rfm69.getTemperature();
	rfm69.send(0, &nodeTemperature, sizeof(Packet_NodeTemperature));
	rfm69.sleep();
	nodeTemperature.sequenceNumber++;
	digitalWrite(LED_BUILTIN, LOW);
	// Calculate minimum time needed to maintain 1% transmission duty cycle.
	const uint32_t time1Percent = (millis() - timeStart) * 100; // ms.
	// Set low power mode and maintain duty cycle or transmit every time period.
	Sleepy::loseSomeTime(
		(time1Percent > timePeriodMillis) ? time1Percent : timePeriodMillis);
}

ISR(WDT_vect) {
	Sleepy::watchdogEvent();
}
