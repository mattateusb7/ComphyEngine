#pragma once

//API
#include "Comphi/Renderer/IGraphicsContext.h"

#include "Comphi/API/Time.h"
#include "Comphi/API/Transform.h"

#include "Comphi/API/Scene.h"
#include "Comphi/API/GameObject.h"
#include "Comphi/API/Mesh.h"
#include "Comphi/API/Camera.h"

#include "Comphi/API/Shader.h"
#include "Comphi/API/Texture.h"
#include "Comphi/API/Material.h"

namespace Comphi {


	class GraphicsAPI
	{
	public:
		
		enum RenderingAPI {
			None,
			Vulkan
		};
		static void selectNone() {
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			activeAPI = RenderingAPI::None; 
		}
		static void selectVulkan() { 
			COMPHILOG_CORE_INFO("Vulkan rendering API Selected.");
			activeAPI = RenderingAPI::Vulkan;
		}
		
		static inline RenderingAPI getActiveAPI() { return activeAPI; }

		//TODO : Design Comfy Interfaces
		struct create { //comply with vulkan object constructors
			static IGraphicsContext* GraphicsContext(void* windowHandler);
			static CameraInstance Camera(CameraProperties cameraProperties = {}, TransformData transformData = {});
			static GameObjectInstance GameObject(MeshData meshData = {}, TransformData transformData = {});
			static SceneInstance Scene();
			static ShaderInstance ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile);
			static TextureInstance Texture(IFileRef& fileref); //TODO: make abstraction of ImgSpecification as a parameter
			static MaterialInstance Material(MaterialProperties materialProperties);
			static MeshInstance Mesh(IFileRef& objFile, MaterialInstance& material);
			static MeshInstance Mesh(VertexArray& vertices, IndexArray& indices, MaterialInstance& material);
		};

	protected:
		static inline RenderingAPI activeAPI = RenderingAPI::None;
		static inline std::shared_ptr<IGraphicsContext> currentGraphicsContext = nullptr;

	};
}
