#pragma once
#include "Comphi/Platform/IWindow.h"
#include "Comphi/Renderer/IGraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Comphi::Windows {
	class Window : public IWindow
	{
	public:
		Window(const WindowProperties& props);
		virtual ~Window();

		void OnUpdate() override;
		void OnBeginUpdate() override;
		void OnWindowResized(uint x, uint y) override;

		inline uint GetWidth() const override { return m_Data.Width; };
		inline uint GetHeight() const override { return m_Data.Height; };
		inline bool IsVSync() const override { return m_Data.VSync; };

		void SetVSync(bool enabled) override;
		void SetEventCallback(const EventCallback& callback) override;
		inline void* GetNativeWindow() const override { return m_Window; };
		inline void* GetGraphicsContext() const override { return m_GraphicsContext.get(); }
	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<IGraphicsContext> m_GraphicsContext;
		WindowProperties m_Data;
	};
}