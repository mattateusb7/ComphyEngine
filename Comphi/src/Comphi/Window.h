#pragma once
#include "Comphi/Core.h"
#include "Comphi/Events/Event.h"
#include "Comphi/Events/ApplicationEvent.h"
#include "Comphi/Events/MouseEvent.h"
#include "Comphi/Events/KeyEvent.h"

namespace Comphi{
	
	struct WindowProperties {
		std::string Title;
		Uint Width;
		Uint Height;

		WindowProperties(const std::string& title = "[Comphi Engine]",
			Uint width = 1280, Uint height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class COMPHI_API Window
	{
	public:
		using EventCallback = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnWindowResized(Uint x, Uint y) = 0;
		virtual void OnBeginUpdate() = 0;
		virtual void OnUpdate() = 0;

		virtual Uint GetWidth() const = 0;
		virtual Uint GetHeight() const = 0;

		//Attributes
		virtual void SetEventCallback(const EventCallback& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//Implement in Platform Specific File
		static Window* Create(const WindowProperties& props = WindowProperties());
	};

}
