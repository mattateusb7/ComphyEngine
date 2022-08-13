#pragma once
#include "Comphi/Core/Core.h"
#include "Comphi/Renderer/IGraphicsContext.h"
#include "Comphi/Renderer/GraphicsAPI.h"

#include <GLFW/glfw3.h>

namespace Comphi::OpenGL {
	class GraphicsContext : public IGraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow& windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Draw() override;
		virtual void ResizeWindow(uint x, uint y) override;
		virtual void ResizeFramebuffer(uint x, uint y) override;
		virtual void CleanUp() override;

	private:
		GLFWwindow* m_WindowHandle;
		
		std::unique_ptr<VertexBuffer> vao;
		std::unique_ptr<ShaderProgram> vertexShader;
		std::unique_ptr<ShaderProgram> fragmentShader;
		std::unique_ptr<ShaderPipeline> shaderPipe;
	};
}