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
		
		void AddComponent(Component& component) {};
		std::vector<Component*> componentList;
	};

	typedef std::shared_ptr<Entity> EntityPtr;

}


