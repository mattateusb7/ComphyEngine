#pragma once
#include "Material.h"
#include "Comphi/Renderer/IMeshObject.h"

namespace Comphi {
	
	typedef std::shared_ptr<IMeshObject> MeshInstance;

	struct MeshData {
		MeshInstance mesh;
	};

	//ABSTRACTION LAYER
	//class Mesh : public IMeshObject, MeshData
	//{ };

}


