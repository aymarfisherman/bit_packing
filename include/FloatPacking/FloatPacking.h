#pragma once

#include "BitPackingConfig.h"

namespace bit_packing {
	template<int precision>	static float getFloatStep() {
		return 0.1f*getFloatStep<precision - 1>();
	}
	template<> static float getFloatStep<0>() {
		return 1.0f;
	}

	template<int minValue, int maxValue, int precision>
	static uint16 getBitCount() {
		float floatStep = getFloatStep<precision>();
		return uint16(ceilf(logf(1.0f + (float(maxValue) - float(minValue))/floatStep)/logf(2.0f)));
	}
		
	template<int minValue, int maxValue, int precision>
	static uint32 packFloat(float value, int& bitCount) {
		if (value < float(minValue) || value > float(maxValue)) {
			throw std::runtime_error("packFloat out-of-bounds.");
		}
		float floatStep = getFloatStep<precision>();
		uint32 result = uint32(roundf((value - float(minValue))/floatStep));
		bitCount = getBitCount<minValue, maxValue, precision>();
		uint32 maxUint((1 << bitCount) - 1);
		return result & maxUint;
	}

	template<int minValue, int maxValue, int precision>
	static float unpackFloat(uint32 value, int& bitCount) {
		bitCount = getBitCount<minValue, maxValue, precision>();
		uint32 maxUint((1 << bitCount) - 1);
		value &= maxUint;
		float floatStep = getFloatStep<precision>();
		return float(minValue) + floatStep*float(value);
	}
}