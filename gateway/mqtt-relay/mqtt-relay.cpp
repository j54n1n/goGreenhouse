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

// Demo reception of a topic.

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include <mosquittopp.h>

#include <SpiDevice/SpiDevice.h>
#include <Rfm69/Rfm69.h>

using namespace std;

struct Packet {
	uint64_t uniqueId;
	uint8_t packetType;
	uint8_t packetVersion;
	uint8_t sequenceNumber;
	int8_t temperature;
} payload;


int topicMessageId = 1; // Message of type string.

struct Rfm69Mqtt : public mosqpp::mosquittopp {
	Rfm69Mqtt(void)	:
		mosqpp::mosquittopp("rfm69") {
		mosqpp::lib_init(); // Mandatory initialization for mosquitto library.
	}
	virtual ~Rfm69Mqtt(void) {
		disconnect();
		loop_stop();
		mosqpp::lib_cleanup();
	}
	virtual void on_connect(int returnCode) {
		cout << "Rfm69Mqtt::on_connect(" << returnCode << ")" << endl;
	}
	virtual void on_disconnect(int returnCode) {
		cout << "Rfm69Mqtt::on_disconnect(" << returnCode << ")" << endl;
	}
	virtual void on_message (const struct mosquitto_message* msg) {
		cout << "Rfm69Mqtt::on_message(0x" << hex << msg << dec << ")" << endl;
		cout << "  msg->mid=" << msg->mid /*Message Id*/ << endl;
		cout << "  msg->payload=0x" << hex << msg->payload << dec << endl;
                cout << "  msg->payloadlen=" << msg->payloadlen << endl;
		cout << "<" << string((char *)(msg->payload), msg->payloadlen) << ">" << endl;
	}
};

Rfm69Mqtt mqtt;

// RFM69HW radio with Slave Select on CE0.
Rfm69<SpiDevice<0>> rfm69;

uint8_t rxBuffer[64] = { 0 };

int main(void) {
	// RFM69 radio.
        rfm69.init(63, 42, 8686);

	cout << "[Rfm69MqttRelay]" << endl;
	mqtt.connect("127.0.0.1");
	// For example: mosquitto_pub -t raw/rfm69/8686/42 -m "My Text"
	mqtt.subscribe(
		0 /*Message Id*/,
		"raw/rfm69/8686/42" /*topic @rfm69, @868.6MHz, @group=42*/);
	while (true) {
		// Keep going mosquitto.
		mqtt.loop(1/*ms*/);

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
}
