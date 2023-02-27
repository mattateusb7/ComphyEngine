#pragma once
#include "Comphi/API/Components/Camera.h"
#include "Comphi/API/Components/Renderer.h"
#include "Entity.h"
#include <set>
#include "Comphi/Utils/Random.h"

namespace Comphi {

	/*enum renderingFlow {
		BatchRendering,
		MeshInstancing
	};*/
	//Mesh + Material + MaterialInstancing = Mesh Instancing
	//Material + MaterialInstancing - Mesh = Batch Rendering

	struct UniqueRenderID {
		MaterialPtr& material;
		MaterialInstancePtr& materialInstance;
		MeshObjectPtr& meshObject;

		std::vector<EntityPtr> entityList;
		uint64_t UID = Comphi::Random::hash_combine(0, material->UID, materialInstance->UID);

		bool operator==(const UniqueRenderID& other) const {
			return other.UID == UID;
		}
	};

	//struct comparator{
	//	bool operator() (const UniqueRenderID& lhs, const UniqueRenderIDs& rhs) const {
	//		return lhs.UID < rhs.value;
	//	}
	//};

}

namespace std {
	template<> struct std::hash<Comphi::UniqueRenderID> {
		size_t operator()(Comphi::UniqueRenderID const& urid) const {
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

		std::unordered_set<UniqueRenderID> sortedEntities;
		std::vector<CameraPtr> cameras;

		//std::vector<ShaderBufferDataPtr> globalData;
		/*std::vector<SceneGraphPtr> subScenes;
		std::vector<MaterialPtr> materials;
		std::vector<MaterialInstancePtr> materialInstances;
		std::vector<MeshObjectPtr> meshObjects;
		std::vector<Entity> entityObjects;*/


	};

	typedef std::shared_ptr<SceneGraph> SceneGraphPtr;
}



