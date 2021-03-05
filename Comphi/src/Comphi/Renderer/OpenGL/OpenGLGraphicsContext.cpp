#include "cphipch.h"
#include "OpenGLGraphicsContext.h"

#include "Comphi/Platform/Windows/WindowsFileRef.h" //TEMP Debug, ShaderPrograms are not initialized here

namespace Comphi::OpenGL {

	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		COMPHILOG_CORE_ASSERT(m_WindowHandle, "Window Handle is NULL!");
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		COMPHILOG_CORE_ASSERT(status, "Could not initialize Glad!");

		COMPHILOG_CORE_INFO("OpenGL Info:");
		COMPHILOG_CORE_INFO("	OpenGL Vendor: {0}", glGetString(GL_VENDOR));
		COMPHILOG_CORE_INFO("	OpenGL Renderer: {0}", glGetString(GL_RENDERER));
		COMPHILOG_CORE_INFO("	OpenGL Version: {0}", glGetString(GL_VERSION));

		COMPHILOG_CORE_INFO("OpenGLContext Initialized...");

		/***DEBUG***/
		vertexShader = new ShaderProgram(Comphi::ShaderType::VertexShader, new Windows::FileRef("C:\\ComphiEngine\\Sandbox\\..\\bin\\Debug-windows-x86_64\\Sandbox\\vert.glsl"));
		fragmentShader = new ShaderProgram(Comphi::ShaderType::FragmentShader, new Windows::FileRef("C:\\ComphiEngine\\Sandbox\\..\\bin\\Debug-windows-x86_64\\Sandbox\\frag.glsl"));
		ShaderWizard::CompileShader(*vertexShader);
		ShaderWizard::CompileShader(*fragmentShader);

		//VERTEX BUFFER
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3]{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		//INDEX BUFFER
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		uint indexes[3] = { 0,1,2 };

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
		/***DEBUG***/
		
	}

	void OpenGL::GraphicsContext::Draw()
	{
		glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		pipe.InitPipeline();

		glBindVertexArray(m_VertexArray);

		pipe.BindProgram(*vertexShader);
		pipe.BindProgram(*fragmentShader);

		pipe.BindPipeline();

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

	}

	void OpenGL::GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}