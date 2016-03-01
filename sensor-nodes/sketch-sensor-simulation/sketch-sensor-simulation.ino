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
#include <Rfm69.h>

#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define LED_BUILTIN 9 // On Moteino R4 LED is connected to D9 instead of D13.

// RFM69HW radio with Slave Select on Arduino pin 10.
Rfm69<SpiDevice<10>, Rfm69ModelHW> rfm69;

uint8_t txBuffer[62];

void setup() {
	// Setup all ports with weak pull-up to save power.
	for (uint8_t i = 0; i <= 19; i++) {
		pinMode(i, INPUT_PULLUP);
	}

	// Blinking LED.
	pinMode(LED_BUILTIN, OUTPUT);

	// RFM69 radio.
	rfm69.init(28, 42, 8686);
	//rfm69.encrypt("mysecret");
	//rfm69.setTransmitPower(+17/*dBm*/);
	rfm69.sleep();
	for (size_t i = 0; i < sizeof(txBuffer); i++)
		txBuffer[i] = (uint8_t)i;
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);

	// txBuffer[0] stores the packet counter.
	size_t txLength = ++txBuffer[0] % (sizeof(txBuffer) + 1);

	// txBuffer[1] stores the RFM69 temperature reading.
	int8_t temperature = rfm69.getTemperature();
	txBuffer[1] = temperature;

	rfm69.send(0, txBuffer, txLength);
	rfm69.sleep();

	digitalWrite(LED_BUILTIN, LOW);
	Sleepy::loseSomeTime(960/*ms*/); // loseSomeTime() works just like delay().
}

ISR(WDT_vect) {
	Sleepy::watchdogEvent();
}
