#pragma once

#include "BitPackingConfig.h"

namespace bit_packing {
	static std::string ushortToString(ushort value) {
		std::string result;
		result.push_back(char(value & 0xff));
		result.push_back(char((value >> 8) & 0xff));
		return result;
	}

	static ushort stringToUshort(const std::string& value) {
		ushort result;
		result = (value.at(0) & 0xff) | ((value.at(1) & 0xff) << 8);
		return result;
	}

	static std::string uintToString(uint value) {
		std::string result;
		result.push_back(char(value & 0xff));
		result.push_back(char((value >> 8) & 0xff));
		result.push_back(char((value >> 16) & 0xff));
		result.push_back(char((value >> 24) & 0xff));
		return result;
	}

	static uint stringToUint(const std::string& value) {
		uint result;
		result = uint(value.at(0) & 0xff) | (uint(value.at(1) & 0xff) << 8) |
			(uint(value.at(2) & 0xff) << 16) | (uint(value.at(3) & 0xff) << 24);
		return result;
	}

	static std::string uint64ToString(uint64 value) {
		std::string result;
		result.push_back(char(value & 0xff));
		result.push_back(char((value >> 8) & 0xff));
		result.push_back(char((value >> 16) & 0xff));
		result.push_back(char((value >> 24) & 0xff));
		result.push_back(char((value >> 32) & 0xff));
		result.push_back(char((value >> 40) & 0xff));
		result.push_back(char((value >> 48) & 0xff));
		result.push_back(char((value >> 56) & 0xff));
		return result;
	}

	static uint64 stringToUint64(const std::string& value) {
		uint64 result;
		result = uint64(value.at(0) & 0xff) | (uint64(value.at(1) & 0xff) << 8) |
			(uint64(value.at(2) & 0xff) << 16) | (uint64(value.at(3) & 0xff) << 24) |
			(uint64(value.at(4) & 0xff) << 32) | (uint64(value.at(5) & 0xff) << 40) |
			(uint64(value.at(6) & 0xff) << 48) | (uint64(value.at(7) & 0xff) << 56);
		return result;
	}
}