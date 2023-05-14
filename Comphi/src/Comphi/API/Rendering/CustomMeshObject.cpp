#include "cphipch.h"
#include "CustomMeshObject.h"

namespace Comphi {
	
	template<typename vx, typename ix>
	CustomMeshObject<vx,ix>::CustomMeshObject(CustomMeshDataBuffers<vx, ix> customMeshDataBuffers)
	{
		meshDataBuffers = customMeshDataBuffers;
		meshDataBuffers.vertexBuffer->updateBufferData(customMeshDataBuffers.vertexDataArray.data());
		meshDataBuffers.indexBuffer->updateBufferData(customMeshDataBuffers.indexDataArray.data());
	}
}