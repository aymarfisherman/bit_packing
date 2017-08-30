#pragma once

#include "BitPackingConfig.h"

namespace bit_packing {
	template<int minValue, int maxValue>
	static PackType16 getBitCount() {
		return PackType16(ceilf(logf(1.0f + (float(maxValue) - float(minValue)))/logf(2.0f)));
	}

	template<int minValue, int maxValue>
	static PackType32 packInt(int value, int& bitCount) {
		if (value < minValue || value > maxValue) {
			throw std::runtime_error("packInt out-of-bounds.");
		}
		PackType32 result = PackType32(value - minValue);
		bitCount = getBitCount<minValue, maxValue>();
		PackType32 maxUint((1 << bitCount) - 1);
		return result & maxUint;
	}

	template<int minValue, int maxValue>
	static int unpackInt(PackType32 value, int& bitCount) {
		bitCount = getBitCount<minValue, maxValue>();
		PackType32 maxUint((1 << bitCount) - 1);
		value &= maxUint;
		return minValue + int(value);
	}
}