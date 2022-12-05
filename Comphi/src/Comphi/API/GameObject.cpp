#include "cphipch.h"
#include "GameObject.h"

namespace Comphi {
	
	GameObject::GameObject(std::shared_ptr<IMeshObject>& mesh, GameObjectData sceneData)
	{
		transform = sceneData.transform;
		parent = sceneData.parent;

		this->mesh = mesh;
		initializeMeshData();
	}


	void GameObject::initializeMeshData() {
		if (mesh.get() && mesh->i_material.get()) {
			//send Data Layout To DesciptorPool
			mesh->initMVP();
			//TODO: Add DescriptoSetLayoutProperties Struct in the future to allow diferent layouts compatible with Descriptor Pool
			mesh->i_material->sendDescriptorSet(mesh->i_MVP_UBOs);
		}
	}


}