#include "cphipch.h"
#include "Renderer.h"

namespace Comphi {
	
	Renderer::Renderer(MaterialInstance& material, MeshObject& meshObject)
	{

		renderData.vertexBuffer->updateBufferData(meshData.vertexData.data());
		renderData.indexBuffer->updateBufferData(meshData.indexData.data());
	}
}
