#include "cphipch.h"
#include "Scene.h"
#include "Comphi/API/Components/Renderer.h"
#include "Comphi/API/Components/Camera.h"

namespace Comphi {

	void Scene::addEntity(EntityPtr& entity)
	{
		auto* cam = entity->GetComponent<Camera>();
		auto* renderer = entity->GetComponent<Renderer>();

		if (cam != nullptr) {
			cameraObjects.push_back(entity);
		}

		if (renderer != nullptr) {
			//a way to 
			renderer->material->parent;
			renderer->meshObject

			renderableObjects.push_back(entity);
		}

		//TODO: Add Scripts
	}
}
