#pragma once
#include "Comphi/Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Comphi {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		void Init() override;
		void SwapBuffers() override;
		void Draw() override;

	private:
		GLFWwindow* m_WindowHandle;
		Uint m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	};

}