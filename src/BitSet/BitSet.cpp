#include "BitSet/BitSet.h"

namespace bit_packing {
	BitSet::BitSet() : lastBitSet(-1), lastBitGet(-1) {}

	BitSet::BitSet(int bitsToReserve) : lastBitSet(-1), lastBitGet(-1) {
		int toReserve = 1 + bitsToReserve/8;
		this->data.resize(toReserve, 0);
	}

	BitSet::BitSet(const std::string& packedData) : lastBitSet(-1), lastBitGet(-1) {
		this->setPackedData(packedData);
	}

	void BitSet::pushBitSet(const BitSet& bitSet) {
		std::string packedData = bitSet.getPackedData();
		this->pushString<65535>(packedData);
	}

	BitSet BitSet::popBitSet() {
		std::string packedData = this->popString<65535>();
		return BitSet(packedData);
	}

	void BitSet::pushBool(bool value) {
		this->ensureSpace(1);
		uint32 bits = value ? 1 : 0;
		this->pushBits(bits, 1);
	}

	bool BitSet::popBool() {
		auto bits = this->popBits(1);
		return bool(bits & 0x1);
	}

	int BitSet::getSizeInBytes() const {
		return this->data.size();
	}
	
	int BitSet::getTotalBitsSet() const {
		return this->lastBitSet + 1;
	}

	std::string BitSet::getPackedData() const {
		std::string packedData(this->data.size(), ' ');
		memcpy(&packedData[0], &this->data[0], this->data.size());
		return packedData;
	}

	void BitSet::setPackedData(const std::string& packedData) {
		this->data.resize(packedData.size());
		memcpy(&this->data[0], &packedData[0], packedData.size());
		this->lastBitGet = -1;
	}

	BitSet::~BitSet() {}

	void BitSet::ensureSpace(int bitCount) {
		int capacity = 8*this->data.size();
		int bitsLeft = capacity - this->getTotalBitsSet();
		if (bitsLeft >= bitCount) {
			return;
		}
		int toReserve = 1 + (bitCount - bitsLeft)/8;
		this->data.resize(this->data.size() + toReserve);
	}

	void BitSet::pushBits(uint32 bits, int bitCount) {
		int nextBitToSet = this->lastBitSet + 1;
		int element = nextBitToSet/8;
		int elementLoc = nextBitToSet%8;
		int bitsLeft = bitCount;
		int bitsLoc = 0;
		do {
			char& ref = this->data[element];
			ref |= ((bits >> bitsLoc) << elementLoc) & 0xff;
			int bitsSet = 8 - elementLoc;
			bitsLeft -= bitsSet;
			bitsLoc += bitsSet;
			elementLoc = 0;
			element++;
		} while (bitsLeft > 0);
		this->lastBitSet += bitCount;
	}

	uint32 BitSet::popBits(int bitCount) {
		int capacity = 8*this->data.size();
		if (this->lastBitGet + bitCount > capacity) {
			throw std::runtime_error("BitSet: Tried to pop more data than it was pushed.");
		}
		uint32 bits(0);
		int nextBitToGet = this->lastBitGet + 1;
		int element = nextBitToGet/8;
		int elementLoc = nextBitToGet%8;
		int bitsLeft = bitCount;
		int bitsLoc = 0;
		do {
			char& ref = this->data[element];
			bits |= uint32((ref & 0xff) >> elementLoc) << bitsLoc;
			int bitsGet = 8 - elementLoc;
			bitsLeft -= bitsGet;
			bitsLoc += bitsGet;
			elementLoc = 0;
			element++;
		} while (bitsLeft > 0);
		this->lastBitGet += bitCount;
		return bits;
	}

	void BitSet::pushStringBits(const std::string& str) {
		this->lastBitSet += (8 - 1) - this->lastBitSet%8;
		int nextBitToSet = this->lastBitSet + 1;
		int element = nextBitToSet/8;
		memcpy(&this->data[element], &str[0], str.size());
		this->lastBitSet += 8*str.size();
	}

	void BitSet::popStringBits(std::string& str) {
		this->lastBitGet += (8 - 1) - this->lastBitGet%8;
		int nextBitToGet = this->lastBitGet + 1;
		int element = nextBitToGet/8;
		memcpy(&str[0], &this->data[element], str.size());
		this->lastBitGet += 8*str.size();
	}
}