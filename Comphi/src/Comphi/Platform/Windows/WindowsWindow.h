#pragma once
#include "../IWindow.h"
#include "Comphi/Renderer/IGraphicsContext.h"

#include <GLFW/glfw3.h> //TEMP - future platform independent

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

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		IGraphicsContext* m_GraphicsContext;

		struct WindowData {
			std::string Title = "";
			uint Width = 0;
			uint Height = 0;
			bool VSync = false;

			EventCallback EventCallback;

			WindowData() :Title(""), Width(0), Height(0) {}

		}m_Data;
	};
}