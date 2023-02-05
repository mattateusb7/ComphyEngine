#pragma once

//API
#include "Comphi/Allocation/IObjectPool.h"

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
#include "Comphi/API/GraphicsPipeline.h"

namespace Comphi {


	class GraphicsAPI
	{
	public:
		
		enum RenderingAPI {
			None,
			Vulkan
		};

		struct select {
			static void None() {
				COMPHILOG_CORE_ERROR("No rendering API Selected.");
				activeAPI = RenderingAPI::None;
			}
			static void Vulkan() {
				COMPHILOG_CORE_INFO("Vulkan rendering API Selected.");
				activeAPI = RenderingAPI::Vulkan;
			}
		};
		
		static inline RenderingAPI getActiveAPI() { return activeAPI; }

		//TODO : Design Comfy Interfaces
		struct create { //comply with vulkan object constructors
			static IGraphicsContext* GraphicsContext(void* windowHandler);
			static CameraInstance Camera(CameraProperties cameraProperties = {}, TransformData transformData = {}, IObjectPool* pool = &cameraPool);
			static GameObjectInstance GameObject(MeshData meshData = {}, TransformData transformData = {}, IObjectPool* pool = &gameObjectPool);
			static SceneInstance Scene();
			static ShaderInstance ShaderProgram(Comphi::ShaderType shaderType, IFileRef& shaderFile, IObjectPool* pool = &shaderPool);
			static TextureInstance Texture(IFileRef& fileref, IObjectPool* pool = &texturePool); //TODO: make abstraction of ImgSpecification as a parameter & add pixelArray initializer
			static MaterialInstance Material(MaterialResources materialProperties, IObjectPool* pool = &materialPool);
			static MeshInstance Mesh(IFileRef& objFile, MaterialInstance& material, IObjectPool* pool = &meshPool);
			static MeshInstance Mesh(VertexArray& vertices, IndexArray& indices, MaterialInstance& material, IObjectPool* pool = &meshPool);
		};
		
		static IObjectPool cameraPool;
		static IObjectPool gameObjectPool;
		static IObjectPool shaderPool;
		static IObjectPool texturePool;
		static IObjectPool materialPool;
		static IObjectPool meshPool;

		static void cleanUpInstances();

	private:
		static inline RenderingAPI activeAPI = RenderingAPI::None;
		static inline std::shared_ptr<IGraphicsContext> currentGraphicsContext = nullptr;

	};

}
