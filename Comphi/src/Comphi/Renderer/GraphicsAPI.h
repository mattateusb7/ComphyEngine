#pragma once

///OpenGL
#include "OpenGL/GraphicsContext.h"

///Vulkan
#include "Vulkan/GraphicsContext.h"

namespace Comphi {
	class GraphicsAPI
	{
	public:
		enum RenderingAPI {
			None,
			OpenGL,
			Vulkan
		};
	private:
		static inline RenderingAPI activeAPI = RenderingAPI::None;
	public:

		static void selectNone() {
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			activeAPI = RenderingAPI::None; 
		}
		static void selectOpenGL() { 
			COMPHILOG_CORE_INFO("OpenGL rendering API Selected.");
			activeAPI = RenderingAPI::OpenGL; 
		}
		static void selectVulkan() { 
			COMPHILOG_CORE_INFO("Vulkan rendering API Selected.");
			activeAPI = RenderingAPI::Vulkan;
		}
		
		static inline RenderingAPI getActiveAPI() { return activeAPI; }

		struct create {
			static IGraphicsContext* GraphicsContext(GLFWwindow& windowHandler);
			static IVertexBuffer* VertexBuffer(IGraphicsContext* currentGraphicsContext, const VertexArray& vertices);
			static IIndexBuffer* IndexBuffer(IGraphicsContext* currentGraphicsContext, const IndexArray& indices);
			static IGraphicsPipeline* GraphicsPipeline();
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
