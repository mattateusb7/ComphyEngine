#include "cphipch.h"
#include "GameObject.h"

namespace Comphi {

	GameObject::GameObject(MeshData meshData, TransformData transformData)
	{
		mesh = meshData.mesh;
		parent = transformData.parent;
		transform = transformData.transform;
	}


}