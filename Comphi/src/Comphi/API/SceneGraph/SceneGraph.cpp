#include "cphipch.h"
#include "SceneGraph.h"

namespace Comphi {

	void SceneGraph::addEntity(EntityPtr& entity)
	{

		auto transform = entity->GetComponent<Transform>();
		auto cam = entity->GetComponent<Camera>();
		auto renderer = entity->GetComponent<Renderer>();

		if (cam.get() != nullptr) {

			if (transform.get() != nullptr) {

				RenderCamera camera{
					cam,transform
				};

				cameras.push_back(camera);
			}
		}
		

		if (renderer.get() != nullptr) {

			RenderBatch renderBatch = { 
				renderer->material->parent,
				renderer->material
			};
			
			RenderMeshInstance renderMeshInstance = {
				renderer->meshObject
			};

			auto batch = renderBatches.find(renderBatch);

			//if Not Found create batch & instance
			if (batch == renderBatches.end()) {
				renderMeshInstance.instancedMeshEntities.push_back(entity);
				renderBatch.renderMeshInstances.insert(renderMeshInstance);
				renderBatches.insert(renderBatch);
				return;
			}

			//else Found
			auto& batchID = const_cast<RenderBatch&>(*batch);
			auto meshInstance = batchID.renderMeshInstances.find(renderMeshInstance);

			//if batch found but no instance, add instance to batch
			if (meshInstance == batchID.renderMeshInstances.end()) {
				renderMeshInstance.instancedMeshEntities.push_back(entity);
				batchID.renderMeshInstances.insert(renderMeshInstance);
				return;
			}

			//else (batch + instance) add Mesh to instances of batch
			auto& meshInstanceID = const_cast<RenderMeshInstance&>(*meshInstance);
			meshInstanceID.instancedMeshEntities.push_back(entity);
		}

		//TODO: Add Scripts
	}

	//void Scene::addScene(SceneGraphPtr& entity)
	//{
	//}
}
