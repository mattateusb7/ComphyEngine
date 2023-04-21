#include "cphipch.h"
#include "SceneGraph.h"
#include "Comphi/API/Components/Renderer.h"
#include "Comphi/API/Components/Camera.h"

namespace Comphi {

	void SceneGraph::addEntity(EntityPtr& entity)
	{

		auto cam = entity->GetComponent<Camera>();
		auto renderer = entity->GetComponent<Renderer>();

		if (cam.get() != nullptr) {
			cameras.push_back(cam);
		}

		if (renderer.get() != nullptr) {

			BatchRenderID brID = { 
				renderer->material->parent,
				renderer->material
			};
			
			MeshInstancingRenderID mirID = {
				renderer->meshObject
			};

			auto batch = batchRenderIDs.find(brID);

			//if Not Found create batch & instance
			if (batch == batchRenderIDs.end()) {
				mirID.instancedMeshEntities.push_back(entity);
				brID.meshInstancingRenderIDs.insert(mirID);
				batchRenderIDs.insert(brID);
				return;
			}

			//else Found
			auto& batchID = const_cast<BatchRenderID&>(*batch);
			auto meshInstance = batchID.meshInstancingRenderIDs.find(mirID);

			//if batch found but no instance, add instance to batch
			if (meshInstance != batchID.meshInstancingRenderIDs.end()) {
				mirID.instancedMeshEntities.push_back(entity);
				batchID.meshInstancingRenderIDs.insert(mirID);
				return;
			}

			//else (batch + instance) add Mesh to instances of batch
			auto& meshInstanceID = const_cast<MeshInstancingRenderID&>(*meshInstance);
			meshInstanceID.instancedMeshEntities.push_back(entity);
		}

		//TODO: Add Scripts
	}

	//void Scene::addScene(SceneGraphPtr& entity)
	//{
	//}
}
