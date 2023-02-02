#include "cphipch.h"
#include "IObjectPool.h"

namespace Comphi {
	void IObjectPool::Add(IObject* obj)
	{
		memoryStack.push_back(obj);
	}
	void IObjectPool::remove(IObject* obj)
	{
		memoryStack.erase(std::find(memoryStack.begin(), memoryStack.end(), obj)); //O(n)
	}
	void IObjectPool::cleanUp()
	{
		for (size_t i = 0; i < memoryStack.size(); i++)
		{
			memoryStack[i]->cleanUp();
		}
		memoryStack.clear();
	}
}