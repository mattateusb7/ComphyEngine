#pragma once
#include "Comphi/API/Components/Camera.h"
#include "Comphi/API/Components/Renderer.h"
#include "Comphi/API/Components/Transform.h"
#include "Entity.h"
#include <set>
#include "Comphi/Utils/Random.h"



namespace Comphi {

	//Mesh + Material + MaterialInstancing = Mesh Instancing
	//Material + MaterialInstancing - Mesh = Batch Rendering

	struct RenderMeshInstance {
		MeshObjectPtr& meshObject;

		std::vector<EntityPtr> instancedMeshEntities;

		uint64_t UID = meshObject->UID;

		bool operator==(const RenderMeshInstance& other) const {
			return other.UID == UID;
		}
	};
}

namespace std {
	template<> struct std::hash<Comphi::RenderMeshInstance> {
		size_t operator()(Comphi::RenderMeshInstance const& urid) const {
			return urid.UID;
		}
	};
}

namespace Comphi{
	struct RenderBatch {
		MaterialPtr& material;
		MaterialInstancePtr& materialInstance;
		
		uint64_t UID = Comphi::Random::hash_combine(0, material->UID, materialInstance->UID);
		
		std::unordered_set<RenderMeshInstance> renderMeshInstances;

		bool operator==(const RenderBatch& other) const {
			return other.UID == UID;
		}
	};
}

namespace std {
	template<> struct std::hash<Comphi::RenderBatch> {
		size_t operator()(Comphi::RenderBatch const& urid) const {
			return urid.UID;
		}
	};
}

namespace Comphi {

	struct RenderCamera {
	public:
		CameraPtr camera;
		TransformPtr transform;
		//void updateViewProjectionMx() {
		//	glm::mat4 viewProjectionMx = glm::mat4(camera->getProjectionMatrix() * transform->getViewMatrix());
		//	camera->bufferPMatrix->updateBufferData(&viewProjectionMx);
		//}
		
	};

	class SceneGraph
	{
	public:
		void addEntity(EntityPtr& entity);
		//void addScene(SceneGraphPtr& entity);

		std::unordered_set<RenderBatch> renderBatches;
		std::vector<RenderCamera> cameras;

		//std::vector<BufferDataPtr> globalData;
		/*std::vector<SceneGraphPtr> subScenes;
		std::vector<MaterialPtr> materials;
		std::vector<MaterialInstancePtr> materialInstances;
		std::vector<MeshObjectPtr> meshObjects;
		std::vector<Entity> entityObjects;*/


	};

	typedef std::shared_ptr<SceneGraph> SceneGraphPtr;
}



