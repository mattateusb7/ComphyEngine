#pragma once
#include "Entity.h"

namespace Comphi {

	struct Scene
	{
		std::vector<Entity*> sceneObjects;
		
	};

	//typedef std::vector<SceneInstance> MultiScene;
	typedef std::shared_ptr<Scene> ScenePtr;
}

