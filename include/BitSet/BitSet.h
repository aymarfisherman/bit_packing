#pragma once

#include <vector>
#include "IntPacking/IntPacking.h"
#include "FloatPacking/FloatPacking.h"

namespace bit_packing {
	class BitSet {
	public:
		BitSet();
		BitSet(int bitsToReserve);
		BitSet(const std::string& packedData);

		template<int maxChars> void pushString(const std::string& str) {
			if (str.size() > maxChars) {
				throw std::runtime_error("BitSet: Tried to push string with more chars than maxChars.");
			}
			this->pushInt<0, maxChars>(str.size());
			this->ensureSpace(8*str.size());
			this->pushStringBits(str);
		}
		template<int maxChars> std::string popString() {
			auto size = this->popInt<0, maxChars>();
			std::string str(size, ' ');
			this->popStringBits(str);
			return str;
		}

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

		void pushBitSet(const BitSet& bitSet);
		BitSet popBitSet();

		void pushBool(bool value);
		bool popBool();

		int getSizeInBytes() const;
		int getTotalBitsSet() const;

		std::string getPackedData() const;
		void setPackedData(const std::string& packedData);

		virtual ~BitSet();
		
	private:
		std::vector<char> data;
		int lastBitSet;
		int lastBitGet;

	private:
		void ensureSpace(int bitCount);
		void pushBits(uint32 bits, int bitCount);
		uint32 popBits(int bitCount);
		void pushStringBits(const std::string& str);
		void popStringBits(std::string& str);
	};
}