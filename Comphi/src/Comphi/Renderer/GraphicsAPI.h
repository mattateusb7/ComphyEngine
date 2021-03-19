#pragma once
#include "Comphi/Log.h"
#include "Objects/IBuffers.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/ShaderWizard.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/GraphicsContext.h"

#include "Vulkan/GraphicsContext.h"

#include "IGraphicsContext.h"
#include <GLFW/glfw3.h>

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
			COMPHILOG_CORE_WARN("OpenGL rendering API Selected.");
			activeAPI = RenderingAPI::OpenGL; 
		}
		static void selectVulkan() { 
			COMPHILOG_CORE_WARN("Vulkan rendering API Selected.");
			activeAPI = RenderingAPI::Vulkan;
		}
		
		static inline RenderingAPI getActiveAPI() { return activeAPI; }

		struct create {
			static IGraphicsContext* GraphicsContext(GLFWwindow* windowHandler);
			static IVertexBuffer* VertexBuffer(const float* vertices, const uint& count);
			static IIndexBuffer* IndexBuffer(const uint* indices);
			static IShaderPipeline* ShaderPipeline();
			static IShaderProgram* ShaderProgram(Comphi::ShaderType shaderType, IFileRef* shaderFile);
		};

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
