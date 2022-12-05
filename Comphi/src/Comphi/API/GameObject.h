#pragma once
#include "Transform.h"
#include "Time.h"
#include "Comphi/Renderer/IMeshObject.h"

namespace Comphi {

	struct ActionHandle
	{
		//awake 
		std::function<void(Time,void*)> startCallback;
		std::function<void(Time,void*)> updateCallback;
		//LateUpdate
	};

	class GameObject;

#define GameObjectInstance std::shared_ptr<Comphi::GameObject>
#define MakeGameObjectInstance std::make_shared<Comphi::GameObject>

	struct GameObjectData {
		GameObjectInstance parent;
		Transform transform;
	};

	class GameObject : public GameObjectData
	{
	public:

		GameObject(std::shared_ptr<IMeshObject>& mesh, GameObjectData data = {});
		~GameObject() = default;

		//GameObject Data
		std::shared_ptr<IMeshObject> mesh;
	
		//GameObject Actions (single Action For now)
		//TODO: may become vector of behaviours; 
		//for now trying to keep "options open" (data oriented Maybe?)
		//instead of going for a component system arquitecture
		ActionHandle action;

	protected:

		void initializeMeshData();

	};


}


