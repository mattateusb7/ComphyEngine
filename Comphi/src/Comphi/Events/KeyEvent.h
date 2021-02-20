#pragma once
#include "Event.h"

namespace Comphi {

	class KeyboardEvent : public Event
	{
	public:

		inline uint GetKeyCode() { return m_keycode; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyboardEvent(uint keycode)
			: m_keycode(keycode) {}

		uint m_keycode;
	};

	class KeyPressedEvent : public KeyboardEvent
	{
	public:

		KeyPressedEvent(uint keycode, uint keyRepeatCount)
			: KeyboardEvent(keycode), m_keyRepeatCount(keyRepeatCount) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardPressedEvent: " << m_keycode << " (" << m_keyRepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	protected:
		uint m_keyRepeatCount;
	};	
	
	
	class KeyReleasedEvent : public KeyboardEvent
	{
	public:

		KeyReleasedEvent(uint keycode)
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

		KeyTypedEvent (uint keycode)
			: KeyboardEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardTypedEvent: " << m_keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};

}