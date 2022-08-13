#pragma once
#include "Event.h"

namespace Comphi {

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent(){}

		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
	
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(){}

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent(){}

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(){}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};	
	
	class WindowOpenEvent : public Event
	{
	public:
		WindowOpenEvent(){}

		EVENT_CLASS_TYPE(WindowOpen);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};	
	
	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent(){}

		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent(){}

		EVENT_CLASS_TYPE(WindowLostFocus);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(uint posX, uint posY)
			: m_posX(posX), m_posY(posY) {}

		inline uint GetposX() { return m_posX; };
		inline uint GetposY() { return m_posY; };

		EVENT_CLASS_TYPE(WindowMoved);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		uint m_posX, m_posY;
	};
		
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint offsetX, uint offsetY) 
			: m_offsetX(offsetX), m_offsetY(offsetY) {}

		inline uint GetOffsetX() { return m_offsetX; };
		inline uint GetOffsetY() { return m_offsetY; };

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint m_offsetX, m_offsetY;
	};

	class FramebufferResizedEvent : public Event
	{
	public:
		FramebufferResizedEvent(uint offsetX, uint offsetY)
			: m_offsetX(offsetX), m_offsetY(offsetY) {}

		inline uint GetOffsetX() { return m_offsetX; };
		inline uint GetOffsetY() { return m_offsetY; };

		std::string ToString() const override {
			std::stringstream ss;
			ss << "FramebufferResizedEvent: " << m_offsetX << ", " << m_offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(FramebufferResized);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint m_offsetX, m_offsetY;
	};

}