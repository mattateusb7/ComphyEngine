#pragma once
#include "Transform.h"
#include "MeshObject.h"

namespace Comphi {

	class GameObject;

#define GameObjectInstance std::shared_ptr<GameObject>
#define MakeGameObjectInstance std::make_shared<GameObject>

	struct SceneObjectData {
		GameObjectInstance parent;
		Transform transform;
	};

	class GameObject : public SceneObjectData
	{
	public:

		GameObject(MeshInstance mesh, SceneObjectData data = {});
		~GameObject() = default;

		//GameObject Data
		MeshInstance mesh;

		void updateMVP(UniformBufferObject mvpUBO, uint currentImage);
		std::vector<Vulkan::UniformBuffer>& getMVP_UBOs() { return MVP_UBOs; };

	protected:

		void initializeMeshData();
		void sendDataLayoutToDesciptorPool();

		//TODO : Move to TransformObj 
		//Figure out where to initialize & update 
		//Make UniformBuffer Interface (API)
		std::vector<Vulkan::UniformBuffer> MVP_UBOs; //TODO : MVP matrix uniform -> Move to TransformObj

	private:
		void initMVP();

	};

	
}


