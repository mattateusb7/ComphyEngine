#pragma once
#include "Camera.h"
#include "GameObject.h"

namespace Comphi {

	struct Scene
	{
		std::vector<std::shared_ptr<Comphi::GameObject>> sceneObjects;
		std::shared_ptr<Comphi::Camera> sceneCamera; //TODO: Only support one Camera For Now
	};
#define MakeSceneInstance std::make_shared<Comphi::Scene>
#define SceneInstance std::shared_ptr<Comphi::Scene>
#define MultiScene std::vector<std::shared_ptr<Comphi::Scene>>
}

