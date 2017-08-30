#include "BitSet/BitSet.h"

namespace bit_packing {
	BitSet::BitSet() : lastBitSet(0), lastBitGet(0) {}

	BitSet::BitSet(int bitsToReserve) : lastBitSet(0), lastBitGet(0) {
		int toReserve = 1 + bitsToReserve/32;
		this->data.resize(toReserve, 0);
	}

	void BitSet::pushBool(bool value) {
		this->ensureSpace(1);
		PackType32 bits = value ? 1 : 0;
		this->pushBits(bits, 1);
	}

	bool BitSet::popBool() {
		auto bits = this->popBits(1);
		return bool(bits & 0x1);
	}

	int BitSet::getSizeInBytes() const {
		return 4*this->data.size();
	}
	
	int BitSet::getTotalBitsSet() const {
		return this->lastBitSet;
	}

	BitSet::~BitSet() {}

	void BitSet::ensureSpace(int bitCount) {
		int capacity = 32*this->data.size();
		int bitsLeft = capacity - lastBitSet;
		if (bitsLeft >= bitCount) {
			return;
		}
		int toReserve = 1 + (bitCount - bitsLeft)/32;
		this->data.resize(this->data.size() + toReserve);
	}

	void BitSet::pushBits(PackType32 bits, int bitCount) {
		int nextBitToSet = this->lastBitSet + 1;
		int element = nextBitToSet/32;
		int elementLoc = nextBitToSet%32 - 1;
		int bitsLeft = bitCount;
		int bitsLoc = 0;
		do {
			PackType32& ref = this->data[element];
			ref |= (bits >> bitsLoc) << elementLoc;
			int bitsSet = 32 - elementLoc;
			bitsLeft -= bitsSet;
			bitsLoc += bitsSet;
			elementLoc = 0;
			element++;
		} while (bitsLeft > 0);
		this->lastBitSet += bitCount;
	}

	PackType32 BitSet::popBits(int bitCount) {
		int capacity = 32*this->data.size();
		if (this->lastBitGet + bitCount > capacity) {
			throw std::runtime_error("BitSet: Tried to pop more data than it was pushed.");
		}
		PackType32 bits(0);
		int nextBitToGet = this->lastBitGet + 1;
		int element = nextBitToGet/32;
		int elementLoc = nextBitToGet%32 - 1;
		int bitsLeft = bitCount;
		int bitsLoc = 0;
		do {
			PackType32& ref = this->data[element];
			bits |= (ref >> elementLoc) << bitsLoc;
			int bitsGet = 32 - elementLoc;
			bitsLeft -= bitsGet;
			bitsLoc += bitsGet;
			elementLoc = 0;
			element++;
		} while (bitsLeft > 0);
		this->lastBitGet += bitCount;
		return bits;
	}
}