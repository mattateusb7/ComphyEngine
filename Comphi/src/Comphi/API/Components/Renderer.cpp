#include "cphipch.h"
#include "Renderer.h"

namespace Comphi {
	
	Renderer::Renderer(MeshObjectPtr& meshObject, MaterialInstancePtr& material)
		: meshObject(meshObject), material(material)
	{
	
	}
}
