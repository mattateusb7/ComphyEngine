#pragma once
#include "Transform.h"
#include "Time.h"
#include "Comphi/API/Mesh.h"

namespace Comphi {

	struct ActionHandle
	{
		//awake 
		std::function<void(Time,void*)> startCallback;
		std::function<void(Time,void*)> updateCallback;
		//LateUpdate
	};

	class GameObject;
	typedef std::shared_ptr<GameObject> GameObjectInstance;

	struct TransformData {
		GameObject* parent;
		Transform transform;
	};

	class GameObject : public IObject, public TransformData, public MeshData
	{
	public:
		GameObject(MeshData meshData = {}, TransformData transformData = {});
		~GameObject() = default;
		virtual void cleanUp() override {}; //TODO : Implement
		
		//GameObject Actions (single Action For now)
		//TODO: may become vector of ActionHandles; 
		//for now trying to keep "options open" (data Driven Maybe?)
		//instead of going for component system arquitecture
		//TODO: Planning to add DLL Loading for Scripting & File IO for Data Driven HotReload
		ActionHandle action; 

	};


}


