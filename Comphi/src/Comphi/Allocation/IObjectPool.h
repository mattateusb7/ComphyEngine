#pragma once
#include "IObject.h"

namespace Comphi {

	class IObjectPool
	{
	public:
		void Add(IObject* obj);
		void remove(IObject* obj);
		void cleanUp();
	private:
		std::vector <IObject*> memoryStack;

	};
}