#pragma once
#include "GameObject.h"
#include "Camera.h"

namespace Comphi {

	struct Scene
	{
		std::vector<GameObjectInstance> sceneObjects;
		CameraInstance sceneCamera; //TODO: Only support one Camera For Now
	};

	typedef std::shared_ptr<Scene> SceneInstance;
	typedef std::vector<SceneInstance> MultiScene;
}

