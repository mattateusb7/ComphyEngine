#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Comphi {
	class DataHandling
	{
	public:
		//convert an uint64_t value into a string representation of its hexadecimal value 
		//with a width of 16 characters, padding with zeros on the left if necessary.
		// ex : "1104a0b1a2c3d4e5"
		static std::string uniqueIDToHexString(uint64_t uniqueId) {
			std::stringstream stream;
			stream << std::setfill('0') << std::setw(16) << std::hex << uniqueId;
			std::string hexStr = stream.str();
			return hexStr;
		}

	};
}