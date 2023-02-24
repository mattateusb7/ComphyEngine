#pragma once
#include "Comphi/API/Components/Component.h"
#include "Comphi/Utils/Time.h"

namespace Comphi {

	struct ActionHandle
	{
		//awake 
		std::function<void(Time,void*)> startCallback;
		std::function<void(Time,void*)> updateCallback;
		//LateUpdate
	};

	class Entity : public IObject
	{
	public:
		Entity() = default;
		~Entity() = default;
		virtual void cleanUp() override { /*foreach subcomponent cleanup()*/ }; //TODO : Implement cleanupOfSubcomponents
		
		template<typename T>
		std::shared_ptr<T>& AddComponent(std::shared_ptr<T> componentPtr);

		template<typename T>
		T* GetComponent();
		
		std::vector<std::shared_ptr<Component>> componentList;
		//Todo: O(1) >> std::unordered_map<std::shared_ptr<Component>,someHash> componentList;
	};

	typedef std::shared_ptr<Entity> EntityPtr;

}


