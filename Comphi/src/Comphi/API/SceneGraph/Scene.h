#pragma once
#include "Entity.h"

namespace Comphi {

	struct Scene
	{
	public:
		void addEntity(EntityPtr& entity);

	protected:


		std::vector<EntityPtr> renderableObjects;
		std::vector<EntityPtr> cameraObjects;
		//std::vector<MaterialPtr> materials;
		//std::vector<MaterialInstancePtr> materialInstances;
		std::vector<MeshObjectPtr> meshInstances;
		
	};

	struct sceneGraphList {

	};

	typedef std::shared_ptr<Scene> ScenePtr;
	typedef std::vector<ScenePtr> SceneGraph;
}

