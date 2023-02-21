#pragma once
#include "Comphi/API/Components/Component.h"
#include "Comphi/API/Rendering/MeshObject.h"
#include "Comphi/API/Rendering/MaterialInstance.h"

namespace Comphi{

	class Renderer : public Component
	{
	public:
		Renderer(MaterialInstance& material, MeshObject& meshObject);
		~Renderer() = default;
	};
	
	typedef std::shared_ptr<Renderer> RendererPtr;

}
