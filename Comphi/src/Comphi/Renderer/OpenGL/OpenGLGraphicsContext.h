#pragma once
#include "Comphi/Renderer/IGraphicsContext.h"

#include "Comphi/Renderer/OpenGL/OpenGLShaderPipeline.h"
#include "Comphi/Renderer/OpenGL/OpenGLShaderWizard.h"
#include "Comphi/Renderer/OpenGL/OpenGLShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Comphi::OpenGL {
	class GraphicsContext : public IGraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* windowHandle);
		void Init() override;
		void SwapBuffers() override;
		void Draw() override;

	private:
		GLFWwindow* m_WindowHandle;

		//DEBUG
		uint m_VertexArray, m_VertexBuffer, m_IndexBuffer;

		ShaderPipeline pipe;
		ShaderProgram* vertexShader;
		ShaderProgram* fragmentShader;
	};
}