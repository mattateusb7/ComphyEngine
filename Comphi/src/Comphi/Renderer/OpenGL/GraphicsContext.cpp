#include "cphipch.h"
#include "GraphicsContext.h"
#include "GLError.h"

#include "Comphi/Renderer/GraphicsAPI.h" //DEBUG!
#include "Comphi/Platform/Windows/FileRef.h"

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

		COMPHILOG_CORE_INFO("OpenGLContext Initialized.");

		/***DEBUG***/

		check_gl_error_on();

		Windows::FileRef* vert = new Windows::FileRef("shaders\\vert.glsl");
		Windows::FileRef* frag = new Windows::FileRef("shaders\\frag.glsl");

		vertexShader.reset(GraphicsAPI::create::ShaderProgram(Comphi::ShaderType::VertexShader,vert));
		fragmentShader.reset(GraphicsAPI::create::ShaderProgram(Comphi::ShaderType::FragmentShader,frag));
		
		float vertices[3 * 3]{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		
		vao.reset(GraphicsAPI::create::VertexBuffer(vertices,3));
		shaderPipe.reset(GraphicsAPI::create::ShaderPipeline());

		/***DEBUG***/
	}

	void GraphicsContext::Draw()
	{
		check_gl_error();

		glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderPipe->BindProgram(*vertexShader);
		shaderPipe->BindProgram(*fragmentShader);
		shaderPipe->BindPipeline();

		vao->bind();

		glDrawArrays(GL_TRIANGLES, 0, 9);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 9, 1);
		check_gl_error();
	}

	void GraphicsContext::ResizeWindow(uint x, uint y)
	{
		glViewport(0, 0, x, y);
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}