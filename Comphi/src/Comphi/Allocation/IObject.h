#pragma once
#include "Comphi/Utils/Random.h"
#include "Comphi/Utils/DataHandling.h"
namespace Comphi {

	class IObject
	{
	public:
		uint64 UID = Random::generateUniqueID();
		std::string hexUID = DataHandling::uniqueIDToHexString(UID);
		virtual void cleanUp() = 0;
	};
}
