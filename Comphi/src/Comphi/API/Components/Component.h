#pragma once
#include "Comphi/Allocation/IObject.h"

namespace Comphi {
	
	class Component : public IObject
	{
		virtual void cleanUp() override {}; //can be overrided by subclasses

	protected:
		Component() = default;
	};

	typedef std::shared_ptr<Component> ComponentPtr;
}