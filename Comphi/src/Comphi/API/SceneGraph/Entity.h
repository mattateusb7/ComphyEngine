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
		virtual void cleanUp() override {}; //TODO : Implement cleanupOfSubcomponents
		
		template<typename T>
		std::shared_ptr<T>& AddComponent(std::shared_ptr<T> componentPtr);

		template<typename T>
		std::shared_ptr<T> GetComponent();
		
		std::vector<ComponentPtr> componentList;
		//Todo: O(1) >> std::unordered_map<std::shared_ptr<Component>,someHash> componentList;
	};

	typedef std::shared_ptr<Entity> EntityPtr;


	template<typename T>
	std::shared_ptr<T>& Entity::AddComponent(std::shared_ptr<T> componentPtr)
	{
		//sanity-Check
		static_assert(std::is_base_of<Component, T>::value, "Sub-Component not derived from BaseClass Component!");

		componentList.push_back(componentPtr);
		return componentPtr;
	}

	template<typename T>
	std::shared_ptr<T> Entity::GetComponent()
	{
		for (size_t i = 0; i < componentList.size(); i++)
		{
			//Find castable component O(N)
			T* componentPtr = dynamic_cast<T*>(componentList[i].get());
			if (componentPtr != nullptr) {
				return (std::static_pointer_cast<T>(componentList[i]));
			}
		}

	}
}

