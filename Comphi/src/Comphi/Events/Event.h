#pragma once
#include "cphipch.h"
#include "Comphi/Core/Core.h"

namespace Comphi {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowOpen, WindowFocus, WindowLostFocus, WindowMoved, WindowResized,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory 
	{
		None = 0,
		EventCategoryApplication =	BIT(0),
		EventCategoryInput =		BIT(1),
		EventCategoryKeyboard =		BIT(2),
		EventCategoryMouse =		BIT(3),
		EventCategoryMouseButton =	BIT(4)
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
				//then dereference the pointer to get correct(cast) event reference type
				m_event.Handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

	using EventCallback = std::function<void(Event&)>;

//Add Log types ToString Variants
	//inline std::ostream& operator<<(std::ostream& os, const Event& e) {
	//	return os << e.ToString();
	//}
}