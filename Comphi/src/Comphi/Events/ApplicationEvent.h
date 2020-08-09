#pragma once
#include "Event.h"

namespace Comphi {

	class COMPHI_API AppTickEvent : public Event
	{
	public:
		AppTickEvent(){}

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
	
	class COMPHI_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(){}

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class COMPHI_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent(){}

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class COMPHI_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};	
	
	class COMPHI_API WindowOpenEvent : public Event
	{
	public:
		WindowOpenEvent(){}

		EVENT_CLASS_TYPE(WindowOpen);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};	
	
	class COMPHI_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(){}

		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class COMPHI_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent(){}

		EVENT_CLASS_TYPE(WindowLostFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class COMPHI_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(Uint posX, Uint posY)
			: m_posX(posX), m_posY(posY) {}

		inline Uint GetposX() { return m_posX; };
		inline Uint GetposY() { return m_posY; };

		EVENT_CLASS_TYPE(WindowMoved);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		Uint m_posX, m_posY;
	};
		
	class COMPHI_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(Uint offsetX, Uint offsetY) 
			: m_offsetX(offsetX), m_offsetY(offsetY) {}

		inline Uint GetOffsetX() { return m_offsetX; };
		inline Uint GetOffsetY() { return m_offsetY; };

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		Uint m_offsetX, m_offsetY;
	};

}