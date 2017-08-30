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
	static PackType16 getBitCount() {
		float floatStep = getFloatStep<precision>();
		return PackType16(ceilf(logf(1.0f + (float(maxValue) - float(minValue))/floatStep)/logf(2.0f)));
	}
		
	template<int minValue, int maxValue, int precision>
	static PackType32 packFloat(float value, int& bitCount) {
		if (value < float(minValue) || value > float(maxValue)) {
			throw std::runtime_error("packFloat out-of-bounds.");
		}
		float floatStep = getFloatStep<precision>();
		PackType32 result = PackType32(roundf((value - float(minValue))/floatStep));
		bitCount = getBitCount<minValue, maxValue, precision>();
		PackType32 maxUint((1 << bitCount) - 1);
		return result & maxUint;
	}

	template<int minValue, int maxValue, int precision>
	static float unpackFloat(PackType32 value, int& bitCount) {
		bitCount = getBitCount<minValue, maxValue, precision>();
		PackType32 maxUint((1 << bitCount) - 1);
		value &= maxUint;
		float floatStep = getFloatStep<precision>();
		return float(minValue) + floatStep*float(value);
	}
}