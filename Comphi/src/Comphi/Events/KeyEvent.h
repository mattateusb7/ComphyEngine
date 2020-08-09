#pragma once
#include "Event.h"

namespace Comphi {

	class COMPHI_API KeyboardEvent : public Event
	{
	public:

		inline Uint GetKeyCode() { return m_keycode; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyboardEvent(Uint keycode, Uint keyRepeatCount)
			: m_keycode(keycode), m_keyRepeatCount(keyRepeatCount) {}
		~KeyboardEvent() {}

		Uint m_keycode;
		Uint m_keyRepeatCount;
	};

	class COMPHI_API KeyPressedEvent : public KeyboardEvent
	{
	public:

		KeyPressedEvent(Uint keycode, Uint keyRepeatCount)
			: KeyboardEvent(keycode, keyRepeatCount) {}
		~KeyPressedEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			if (m_keyRepeatCount > 0) 
				ss << "KeyboardPressedEvent (repeat):" << m_keycode;
			else 
				ss << "KeyboardPressedEvent:" << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	};	
	
	
	class COMPHI_API KeyReleasedEvent : public KeyboardEvent
	{
	public:

		KeyReleasedEvent(Uint keycode)
			: KeyboardEvent(keycode, 0) {}
		~KeyReleasedEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardReleasedEvent:" << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);

	};

}