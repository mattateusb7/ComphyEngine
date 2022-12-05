#pragma once

//API
#include "Comphi/API/Time.h"
#include "Comphi/API/Transform.h"

#include "Comphi/API/Scene.h"
#include "Comphi/API/GameObject.h"
#include "Comphi/API/Camera.h"

#include "Comphi/API/Shader.h"
#include "Comphi/API/Texture.h"
#include "Comphi/API/Material.h"

///Vulkan
#include "IGraphicsContext.h"
#include "../../../vendor/GLFW/include/GLFW/glfw3.h"

namespace Comphi {
	class GraphicsAPI
	{
	public:
		enum RenderingAPI {
			None,
			Vulkan
		};
	private:
		static inline RenderingAPI activeAPI = RenderingAPI::None;
	public:

		static void selectNone() {
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			activeAPI = RenderingAPI::None; 
		}
		static void selectVulkan() { 
			COMPHILOG_CORE_INFO("Vulkan rendering API Selected.");
			activeAPI = RenderingAPI::Vulkan;
		}
		
		static inline RenderingAPI getActiveAPI() { return activeAPI; }

		struct create {
			static IGraphicsContext* GraphicsContext(GLFWwindow& windowHandler);
			//TODO : Design comfy Interfaces
			static IVertexBuffer* VertexBuffer(IGraphicsContext* currentGraphicsContext, const VertexArray& vertices);
			static IIndexBuffer* IndexBuffer(IGraphicsContext* currentGraphicsContext, const IndexArray& indices);
			static IShaderProgram* ShaderProgram(IGraphicsContext* currentGraphicsContext, Comphi::ShaderType shaderType, IFileRef& shaderFile);
			
		};

		//std::shared_ptr<IGraphicsContext> currentGraphicsContext;

		//struct start {
		//	virtual void test() = 0;
		//};

		//struct end{
		//	virtual void test() = 0;
		//};

		//struct draw {
		//	virtual void Triangle();
		//}
	};
}
