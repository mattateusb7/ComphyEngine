#pragma once
#include "Comphi/Core.h"
#include "Comphi/Window.h"

#include <GLFW/glfw3.h>

namespace Comphi {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		void OnBeginUpdate() override;
		void OnWindowResized(Uint x, Uint y) override;

		inline Uint GetWidth() const override { return m_Data.Width; };
		inline Uint GetHeight() const override { return m_Data.Height; };
		inline bool IsVSync() const override { return m_Data.VSync; };

		void SetVSync(bool enabled) override;
		void SetEventCallback(const EventCallback& callback) override;
		inline void* GetNativeWindow() const override { return m_Window; };

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
	
		struct WindowData {
			std::string Title = "";
			Uint Width = 0;
			Uint Height = 0;
			bool VSync = false;

			EventCallback EventCallback;

			WindowData() :Title(""), Width(0), Height(0) {}

		}m_Data;
	};
}