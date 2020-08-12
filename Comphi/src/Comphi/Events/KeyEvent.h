#pragma once
#include "Event.h"

namespace Comphi {

	class KeyboardEvent : public Event
	{
	public:

		inline Uint GetKeyCode() { return m_keycode; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyboardEvent(Uint keycode)
			: m_keycode(keycode) {}

		Uint m_keycode;
	};

	class KeyPressedEvent : public KeyboardEvent
	{
	public:

		KeyPressedEvent(Uint keycode, Uint keyRepeatCount)
			: KeyboardEvent(keycode), m_keyRepeatCount(keyRepeatCount) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardPressedEvent: " << m_keycode << " (" << m_keyRepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	protected:
		Uint m_keyRepeatCount;
	};	
	
	
	class KeyReleasedEvent : public KeyboardEvent
	{
	public:

		KeyReleasedEvent(Uint keycode)
			: KeyboardEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardReleasedEvent:" << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);

	};	
	
	class KeyTypedEvent : public KeyboardEvent
	{
	public:

		KeyTypedEvent (Uint keycode)
			: KeyboardEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardTypedEvent: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};

}