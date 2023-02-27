#include "cphipch.h"
#include "SceneGraph.h"
#include "Comphi/API/Components/Renderer.h"
#include "Comphi/API/Components/Camera.h"

namespace Comphi {

	void SceneGraph::addEntity(EntityPtr& entity)
	{

		auto cam = entity->GetComponent<Camera>();
		auto renderer = entity->GetComponent<Renderer>();

		if (cam != nullptr) {
			cameras.push_back(cam);
		}

		if (renderer != nullptr) {

			UniqueRenderID urid = { 
				renderer->material->parent, 
				renderer->material, 
				renderer->meshObject 
			};
			auto elem = sortedEntities.find(urid);

			//Not Found
			if (elem == sortedEntities.end()) {
				urid.entityList.push_back(entity);
				sortedEntities.insert(urid);
				return;
			}

			//Found
			auto& entityList = const_cast<UniqueRenderID&>(*elem);
			entityList.entityList.push_back(entity);
			
		}

		//TODO: Add Scripts
	}

	//void Scene::addScene(SceneGraphPtr& entity)
	//{
	//}
}
