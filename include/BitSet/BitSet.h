#pragma once

#include <vector>
#include "IntPacking/IntPacking.h"
#include "FloatPacking/FloatPacking.h"

namespace bit_packing {
	class BitSet {
	public:
		BitSet();
		BitSet(int bitsToReserve);

		template<int minValue, int maxValue, int precision> void pushFloat(float value) {
			int bitCount;
			auto bits = packFloat<minValue, maxValue, precision>(value, bitCount);
			this->ensureSpace(bitCount);
			this->pushBits(bits, bitCount);
		}
		template<int minValue, int maxValue, int precision> float popFloat() {
			int bitCount = getBitCount<minValue, maxValue, precision>();
			auto bits = this->popBits(bitCount);
			return unpackFloat<minValue, maxValue, precision>(bits, bitCount);
		}
		template<int minValue, int maxValue> void pushInt(int value) {
			int bitCount;
			auto bits = packInt<minValue, maxValue>(value, bitCount);
			this->ensureSpace(bitCount);
			this->pushBits(bits, bitCount);
		}
		template<int minValue, int maxValue> int popInt() {
			int bitCount = getBitCount<minValue, maxValue>();
			auto bits = this->popBits(bitCount);
			return unpackInt<minValue, maxValue>(bits, bitCount);
		}
		void pushBool(bool value);
		bool popBool();

		int getSizeInBytes() const;
		int getTotalBitsSet() const;

		virtual ~BitSet();
		
	private:
		std::vector<PackType32> data;
		int lastBitSet;
		int lastBitGet;

	private:
		void ensureSpace(int bitCount);
		void pushBits(PackType32 bits, int bitCount);
		PackType32 popBits(int bitCount);
	};
}