#include <SPI.h>
#include <SpiDevice.h>

// W25X40CL SPI Flash memory.
SpiDevice<8> spi;

void printBytes(uint8_t* buf, size_t len) {
	for (size_t i = 0; i < len; i++) {
		Serial.print(buf[i] >> 4, HEX);
		Serial.print(buf[i] & 0xF, HEX);
	}
	Serial.println();
}

void setup() {
	Serial.begin(115200);
	spi.master();
	Serial.println("SPI Flash Unique ID");

	// Setup W25X40CL SPI Flash memory to read unique ID number.
	uint8_t data[
		1/*Command Byte*/ + 4/*Dummy Bytes*/ + 8/*Unique ID bytes*/] = { 0 };
	data[0] = 0x4B;

	printBytes(data, sizeof(data));
	spi.transferBulk(data, sizeof(data));
	printBytes(data, sizeof(data));

	uint64_t uniqueId;
	memcpy(&uniqueId, data + 5, sizeof(uint64_t)); // Reverse order?
	//uniqueId = *(uint64_t *)(data + 5); // Same as above.
	Serial.print((uint32_t)(uniqueId >> 32), HEX);
	Serial.println((uint32_t)(uniqueId >> 0xFFFFFFFF), HEX);
}

void loop() {
}
