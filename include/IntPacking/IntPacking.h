#pragma once

#include "BitPackingConfig.h"

namespace bit_packing {
	template<int minValue, int maxValue>
	static uint16 getBitCount() {
		return uint16(ceilf(logf(1.0f + (float(maxValue) - float(minValue)))/logf(2.0f)));
	}

	template<int minValue, int maxValue>
	static uint32 packInt(int value, int& bitCount) {
		if (value < minValue || value > maxValue) {
			throw std::runtime_error("packInt out-of-bounds.");
		}
		uint32 result = uint32(value - minValue);
		bitCount = getBitCount<minValue, maxValue>();
		uint32 maxUint((1 << bitCount) - 1);
		return result & maxUint;
	}

	template<int minValue, int maxValue>
	static int unpackInt(uint32 value, int& bitCount) {
		bitCount = getBitCount<minValue, maxValue>();
		uint32 maxUint((1 << bitCount) - 1);
		value &= maxUint;
		return minValue + int(value);
	}
}