#include <SPI.h>
#include <SpiDevice.h>
#include <SpiFlash.h>

// W25X40CL SPI Flash memory.
SpiFlash<SpiDevice<8> > flash;

union Id {
	uint64_t val;
	struct {
		uint32_t lo;
		uint32_t hi;
	} ints;
};

void setup() {
	Serial.begin(115200);
	flash.init();
	Id id;
	id.val = flash.getUniqueId();
	Serial.print(id.ints.hi, HEX);
	Serial.print(id.ints.lo, HEX);
}

void loop() {
}
