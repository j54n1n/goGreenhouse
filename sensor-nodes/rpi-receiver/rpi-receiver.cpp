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

#include <cstdint>
#include <cstring>
#include <iostream>

#include <SpiDevice.h>
#include <Rfm69.h>

using namespace std;

struct Packet {
	uint64_t uniqueId;
	uint8_t packetType;
	uint8_t packetVersion;
	uint8_t sequenceNumber;
	int8_t temperature;
} payload;

// RFM69HW radio with Slave Select on CE0.
Rfm69<SpiDevice<0>> rfm69;

uint8_t rxBuffer[64] = { 0 };

void setup(void);
void loop(void);

int main(void) {
	setup();

	while (true) {
		loop();
	}
}

void setup(void) {
        // RFM69 radio.
        rfm69.init(63, 42, 8686);
        //rfm69.encrypt("mysecret");

	cout << "[Rfm69RxDemo]" << endl;
}

void loop(void) {
	int length = rfm69.receive(rxBuffer, sizeof(rxBuffer));
	if (length >= 0) {
		cout << "OK " << length << "byte(s) ";
		for (int i = 0; i < length; i++) {
			if (i == 2) {
				// Start of payload.
				cout << " [";
			}
			cout << hex << (int)(rxBuffer[i] >> 4);
			cout << hex << (int)(rxBuffer[i] & 0x0F);
		}
		cout << "] (" << dec << (int)rfm69.getRssiValue() << "dBm";
		cout << ((rfm69.afc < 0) ? "" : "+") << rfm69.afc << ":";
		cout << (int)rfm69.getLnaGain() << "dB)" << endl;

		// Decoded packet.
		cout << "Packet:";
		uint8_t parity = ((rxBuffer[0] & 0xC0) >> 6);
		uint8_t header = ((rxBuffer[1] & 0xC0) | (rxBuffer[0] & 0x3F));
		uint8_t sourceId = rxBuffer[1] & 0x3F;
		cout << " p=" << hex << (int)parity;
		cout << " h=" << hex << (int)header;
		cout << " srcId=" << dec << (int)sourceId;
		memcpy(&payload, &rxBuffer[2], length - 2);
		cout << " [uniqueId=0x" << hex << payload.uniqueId;
		cout << " packetType=0x" << hex << (int)payload.packetType;
		cout << " packetVersion=0x" << hex << (int)payload.packetVersion;
		cout << " sequenceNumber=0x" << hex << (int)payload.sequenceNumber;
		cout << " temperature=" << dec << (int)payload.temperature;
		cout << " degree C]" << endl;
	}
}
