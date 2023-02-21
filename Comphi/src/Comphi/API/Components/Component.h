#pragma once
#include "Comphi/Allocation/IObject.h"

namespace Comphi {
	class Component : public IObject
	{
		virtual void cleanUp() override {}; //can be overrided by subclasses
	};
}