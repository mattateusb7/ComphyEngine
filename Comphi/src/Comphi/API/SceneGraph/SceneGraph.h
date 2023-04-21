#pragma once
#include "Comphi/API/Components/Camera.h"
#include "Comphi/API/Components/Renderer.h"
#include "Entity.h"
#include <set>
#include "Comphi/Utils/Random.h"



namespace Comphi {

	//Mesh + Material + MaterialInstancing = Mesh Instancing
	//Material + MaterialInstancing - Mesh = Batch Rendering

	struct MeshInstancingRenderID {
		MeshObjectPtr& meshObject;

		std::vector<EntityPtr> instancedMeshEntities;

		uint64_t UID = meshObject->UID;

		bool operator==(const MeshInstancingRenderID& other) const {
			return other.UID == UID;
		}
	};
}

namespace std {
	template<> struct std::hash<Comphi::MeshInstancingRenderID> {
		size_t operator()(Comphi::MeshInstancingRenderID const& urid) const {
			return urid.UID;
		}
	};
}

namespace Comphi{
	struct BatchRenderID {
		MaterialPtr& material;
		MaterialInstancePtr& materialInstance;
		
		uint64_t UID = Comphi::Random::hash_combine(0, material->UID, materialInstance->UID);
		
		std::unordered_set<MeshInstancingRenderID> meshInstancingRenderIDs;

		bool operator==(const BatchRenderID& other) const {
			return other.UID == UID;
		}
	};
}

namespace std {
	template<> struct std::hash<Comphi::BatchRenderID> {
		size_t operator()(Comphi::BatchRenderID const& urid) const {
			return urid.UID;
		}
	};
}

namespace Comphi {

	class SceneGraph
	{
	public:
		void addEntity(EntityPtr& entity);
		//void addScene(SceneGraphPtr& entity);

		std::unordered_set<BatchRenderID> batchRenderIDs;
		std::vector<CameraPtr> cameras;

		//std::vector<BufferDataPtr> globalData;
		/*std::vector<SceneGraphPtr> subScenes;
		std::vector<MaterialPtr> materials;
		std::vector<MaterialInstancePtr> materialInstances;
		std::vector<MeshObjectPtr> meshObjects;
		std::vector<Entity> entityObjects;*/


	};

	typedef std::shared_ptr<SceneGraph> SceneGraphPtr;
}



