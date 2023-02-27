#pragma once
#include "Comphi/API/Components/Component.h"
#include "Comphi/API/Rendering/MeshObject.h"
#include "Comphi/API/Rendering/MaterialInstance.h"

namespace Comphi{

	class Renderer : public Component
	{
	public:
		Renderer(MeshObjectPtr& meshObject, MaterialInstancePtr& material);
		
		MeshObjectPtr& meshObject;
		MaterialInstancePtr& material;

		~Renderer() = default;
	};
	
	typedef std::shared_ptr<Renderer> RendererPtr;

}
