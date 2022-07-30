#pragma once
#include "cphipch.h"
#include "Comphi/Core/Core.h"

namespace Comphi {

	enum class EventType
	{
		None = 0,
		GenericError,
		WindowClose, WindowOpen, WindowFocus, WindowLostFocus, WindowMoved, WindowResized,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory 
	{
		None = 0,
		EventCategoryError =		BIT(0),
		EventCategoryApplication =	BIT(1),
		EventCategoryInput =		BIT(2),
		EventCategoryKeyboard =		BIT(3),
		EventCategoryMouse =		BIT(4),
		EventCategoryMouseButton =	BIT(5),
		EventCategoryVulkan =		BIT(6),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type; }\
								virtual EventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override {return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category; }

	class Event {
		friend class EventDispatcher;
		friend class Application;
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual const char* GetName() const = 0; //Probably debug only
		
		virtual inline std::string ToString() const { return GetName(); }
		inline bool isInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	protected:
		bool Handled = false;
	};
	
	class EventDispatcher {
		template<typename T> 
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) 
			: m_event(event){}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_event.GetEventType() == T::GetStaticType()) {
				//point to event reference with correct type pointer 
				//then dereference the pointer to (cast) get correct event reference type
				m_event.Handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	class EventHandler {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		template<typename T>
		static bool Throw(Event& event, EventFn<T> func ) {
			std::unique_ptr<EventDispatcher> newEvent = std::make_unique<EventDispatcher>(event);
			return newEvent->Dispatch(func);
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

	using EventCallback = std::function<void(Event&)>;

}