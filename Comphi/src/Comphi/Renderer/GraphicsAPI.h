#pragma once
#include "Comphi/Log.h"
#include "Objects/IBuffers.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/ShaderWizard.h"
#include "OpenGL/ShaderProgram.h"

namespace Comphi {
	class GraphicsAPI
	{
	public:
		
		static inline enum RenderingAPI {
			None,
			OpenGL,
			DirectX
		}activeAPI = RenderingAPI::None;

		static void selectNone() {
			COMPHILOG_CORE_ERROR("No rendering API Selected.");
			activeAPI = RenderingAPI::None; 
		}
		static void selectOpenGL() { 
			COMPHILOG_CORE_INFO("OpenGL rendering API Selected.");
			activeAPI = RenderingAPI::OpenGL; 
		}
		static void selectDirectX() { 
			COMPHILOG_CORE_INFO("DirectX rendering API Selected.");
			activeAPI = RenderingAPI::DirectX; 
		}
		
		inline RenderingAPI getActiveAPI() { return activeAPI; }

		struct create {
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
