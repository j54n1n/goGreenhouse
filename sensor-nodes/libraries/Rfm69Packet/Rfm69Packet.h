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

#ifndef RFM_69_PACKET_H
#define RFM_69_PACKET_H

#include <stdint.h>

namespace Rfm69Packet {

	//! Packet type identifier.
	//! \see Packet::packetType
	//! \see Packet_init
	enum PacketType {
		PacketType_NodeTemperature, //!< RFM69 radio chip temperature packet.
		// TODO: Add additional packet types. Maximum allowed types is 255.
		//
		//PacketType_Size //!< Packet types count.
	};

	/*
//! Define packet alignment size. Use at least 16 bytes for encryption.
//! Multiples of uint64_t should be compatible with all architectures from
//! 64-bit PC to 8-bit microcontrollers. It should be large enough such that it
//! can contain all packet type fields.
#ifndef __RFM_69_PACKET_SIZE_ALIGNMENT
#define __RFM_69_PACKET_SIZE_ALIGNMENT (2*sizeof(uint64_t))
#endif
	*/

#define __RFM_69_PACKET_FIELD_INFO  \
	uint64_t uniqueId;              \
	uint8_t packetType;             \
	uint8_t sequenceNumber;

	//! Plain Old Data (POD) base type for data exchange.
	//! It is assumed that every packet type contains at least these fields.
	//! Also it is assumed that the data is ordered in Little Endian encoding.
	struct Packet {
		__RFM_69_PACKET_FIELD_INFO
	};

	//! Initialize a data packet with given packet type.
	//! \param[in,out] packet is the data packet base type.
	//! \param packetType is the packet type that will be applied.
	//! \see PacketType
	//! \see Packet::packetType
	void Packet_init(Packet* packet, PacketType packetType) {
		packet->uniqueId = (uint64_t)0;
		packet->packetType = (uint8_t)packetType;
		packet->sequenceNumber = (uint8_t)0;
	}

	/*
#define __RFM_69_PACKET_STORAGE_PADDING_SIZE (__RFM_69_PACKET_SIZE_ALIGNMENT - 1) //sizeof(Packet)

	//! Packet definition for memory allocation.
	//! Do not use this type for anything else.
	struct Packet_Storage {
		__RFM_69_PACKET_FIELD_INFO
		uint8_t __ps_padding[__RFM_69_PACKET_STORAGE_PADDING_SIZE];
	};
	*/

	//! RFM69 radio chip temperature data packet.
	struct Packet_NodeTemperature {
	__RFM_69_PACKET_FIELD_INFO
		int8_t temperature; //!< RFM69 radio chip temperature in degrees C.
	};
}

#endif // RFM_69_PACKET_H
