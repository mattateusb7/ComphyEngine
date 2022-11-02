#pragma once
#include "Comphi/Renderer/IGraphicsContext.h"

#include "GLError.h"
#include "Comphi/Renderer/GraphicsAPI.h" //DEBUG!

#include "Objects/IndexBuffer.h"
#include "Objects/VertexBuffer.h"
#include "Objects/ShaderPipeline.h"
#include "Objects/ShaderProgram.h"
#include "ShaderWizard.h"

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
		GLFWwindow* windowHandle;
		std::unique_ptr<VertexBuffer> vao;
		std::unique_ptr<ShaderProgram> vertexShader;
		std::unique_ptr<ShaderProgram> fragmentShader;
		std::unique_ptr<ShaderPipeline> shaderPipe;
	};
}