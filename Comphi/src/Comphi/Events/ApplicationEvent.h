#pragma once

#include "Event.h"

namespace Comphi {

	class CPHI_API AppTickEvent : public Event
	{
	public:
		AppTickEvent(){}

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
	
	class CPHI_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(){}

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class CPHI_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent(){}

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class CPHI_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};	
	
	class CPHI_API WindowOpenEvent : public Event
	{
	public:
		WindowOpenEvent(){}

		EVENT_CLASS_TYPE(WindowOpen);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};	
	
	class CPHI_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(){}

		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class CPHI_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent(){}

		EVENT_CLASS_TYPE(WindowLostFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class CPHI_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(Uint posX, Uint posY)
			: _posX(posX), _posY(posY) {}

		inline Uint GetposX() { return _posX; };
		inline Uint GetposY() { return _posY; };

		EVENT_CLASS_TYPE(WindowMoved);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		Uint _posX, _posY;
	};
		
	class CPHI_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(Uint offsetX, Uint offsetY) 
			: _offsetX(offsetX), _offsetY(offsetY) {}

		inline Uint GetOffsetX() { return _offsetX; };
		inline Uint GetOffsetY() { return _offsetY; };

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizedEvent: " << _offsetX << ", " << _offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		Uint _offsetX, _offsetY;
	};

}