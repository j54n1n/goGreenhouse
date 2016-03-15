#include <ArduinoJson.h>

namespace Rfm69Packet {

	union UniqueId {
		uint64_t dword;
		struct {
			uint32_t lo;
			uint32_t hi;
		} word;
		UniqueId(uint64_t uniqueId) : dword(uniqueId) {}
		UniqueId(uint32_t uniqueIdHi, uint32_t uniqueIdLo) {
			word.hi = uniqueIdHi;
			word.lo = uniqueIdLo;
		}
	};

	struct Packet {
		static const char* KEY_UNIQUE_ID_WORD_HI;
		static const char* KEY_UNIQUE_ID_WORD_LO;
		static const char* KEY_PACKAGE_TYPE;
		const UniqueId uniqueId;
		const uint16_t packetType;
		Packet(uint64_t nodeId, uint16_t type) : uniqueId(nodeId), packetType(type) {}
	};

	const char* Packet::KEY_UNIQUE_ID_WORD_HI = "id_hi";
	const char* Packet::KEY_UNIQUE_ID_WORD_LO = "id_lo";
	const char* Packet::KEY_PACKAGE_TYPE = "type";

	struct NodeTemperaturePacket : public Packet {
		static const uint32_t TYPE = 1;
		static const char* KEY_TEMPERATURE;
		int8_t temperature;
		explicit NodeTemperaturePacket(uint64_t nodeId) : Packet(nodeId, TYPE) {}
	};

	const char* NodeTemperaturePacket::KEY_TEMPERATURE = "temp";

	// {"id":xxx,"sensor":"temp","value":40.3}
	size_t createPacket(NodeTemperaturePacket* packet, uint8_t* buf, size_t length) {
		const int BUFFER_SIZE = JSON_OBJECT_SIZE(4);
		StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
		JsonObject& object = jsonBuffer.createObject();
		object[Packet::KEY_UNIQUE_ID_WORD_HI] = packet->uniqueId.word.hi;
		object[Packet::KEY_UNIQUE_ID_WORD_LO] = packet->uniqueId.word.lo;
		object[Packet::KEY_PACKAGE_TYPE] = packet->packetType;
		object[NodeTemperaturePacket::KEY_TEMPERATURE] = packet->temperature;
		return object.printTo((char*)buf, length);
	}
	void readPacket(const char* buf, Packet* packet) {
		const int BUFFER_SIZE = JSON_OBJECT_SIZE(11);
		StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
		JsonObject& object = jsonBuffer.parseObject(buf);
		Serial.print("read_success=");
		Serial.println(object.success());
		if (object.success()) {
			const uint16_t type = object[Packet::KEY_PACKAGE_TYPE];
			const UniqueId id(
				object[Packet::KEY_UNIQUE_ID_WORD_HI],
				object[Packet::KEY_UNIQUE_ID_WORD_LO]);
			if (type == NodeTemperaturePacket::TYPE) {
				NodeTemperaturePacket nt(id.dword);
				nt.temperature = object[NodeTemperaturePacket::KEY_TEMPERATURE];
				uint8_t *p = (uint8_t*)&nt;
				for (size_t i = 0; i < sizeof(NodeTemperaturePacket); i++) {
					Serial.print(p[i], HEX); Serial.print(' ');
				}
				Serial.println();
			}
		}
	}

	size_t createPacket(Packet* packet, uint8_t* buf, size_t length) {
		uint64_t id = packet->uniqueId.dword;
		if (packet->packetType == NodeTemperaturePacket::TYPE) {
			return createPacket((NodeTemperaturePacket*)packet, buf, length);
		}
		return 0;
	}

}

using namespace Rfm69Packet;

uint64_t nodeId = 0x7B2269645F686922LL;
NodeTemperaturePacket nt(nodeId);

Packet* packets[2];

uint8_t buffer[62];

uint8_t rx[] = {
	0x7B,0x22,0x69,0x64,0x5F,0x68,0x69,0x22,0x3A,0x2D,0x31,0x2C,0x22,0x69,0x64,
	0x5F,0x6C,0x6F,0x22,0x3A,0x2D,0x31,0x2C,0x22,0x74,0x79,0x70,0x65,0x22,0x3A,
	0x31,0x2C,0x22,0x74,0x65,0x6D,0x70,0x22,0x3A,0x32,0x30,0x7D,0x00 };

void setup() {
	Serial.begin(115200);
	nt.temperature = 20;
	packets[0] = &nt;
	packets[1] = &nt;
	size_t s = createPacket(packets[0], buffer, sizeof(buffer));
	Serial.println(sizeof(buffer));
	Serial.println(s); // if (s == sizeof(buffer)-1) then overflow
	Serial.println((char*)buffer);
	for (size_t i = 0; buffer[i] != 0; i++) {
		Serial.print(buffer[i], HEX);
	}
	Serial.println();
	readPacket((char*)buffer, 0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
