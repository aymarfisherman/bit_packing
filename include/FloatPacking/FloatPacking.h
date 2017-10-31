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
			std::stringstream stream;
			stream << "packFloat out of bounds: minValue=" << minValue << "; maxValue=" << maxValue << "; precision=" << precision << "; value = " << value << ";";
			throw std::runtime_error(stream.str());
		}
		bitCount = getBitCount<minValue, maxValue, precision>();
		if (bitCount >= 32) { //fallback: bitCount too high, give up packing data;
			std::cout << "Warning: Following minValue, maxValue, precision combination: " <<
				minValue << ", " << maxValue << ", " << precision << " generates a bit count too high.\n";
			std::cout << "Float bit count: 32. Computed bit count: " << bitCount << ".\n";
			std::cout << "Try bringing minValue and maxValue closer to each other and/or reducing the precision.\n";
			bitCount = 32;
			uint32 result;
			memcpy(&result, &value, 4);
			return result;
		}
		float floatStep = getFloatStep<precision>();
		uint32 result = uint32(roundf((value - float(minValue))/floatStep));
		uint32 maxUint((1 << bitCount) - 1);
		return result & maxUint;
	}

	template<int minValue, int maxValue, int precision>
	static float unpackFloat(uint32 value, int& bitCount) {
		bitCount = getBitCount<minValue, maxValue, precision>();
		if (bitCount >= 32) { //fallback: bitCount too high, it gave up packing data;
			bitCount = 32;
			float result;
			memcpy(&result, &value, 4);
			return result;
		}
		uint32 maxUint((1 << bitCount) - 1);
		value &= maxUint;
		float floatStep = getFloatStep<precision>();
		return float(minValue) + floatStep*float(value);
	}
}