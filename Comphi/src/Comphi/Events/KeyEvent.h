#pragma once

#include "Event.h"

namespace Comphi {

	class CPHI_API KeyboardEvent : public Event
	{
	public:

		inline Uint GetKeyCode() { return _keycode; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyboardEvent(Uint keycode)
			: _keycode(keycode) {}
		~KeyboardEvent() {}

		Uint _keycode;
	};

	class CPHI_API KeyPressedEvent : public KeyboardEvent
	{
	public:

		KeyPressedEvent(Uint  keycode)
			: KeyboardEvent(keycode) {}
		~KeyPressedEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardPressedEvent:" << _keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);

	};	
	
	
	class CPHI_API KeyReleasedEvent : public KeyboardEvent
	{
	public:

		KeyReleasedEvent(Uint  keycode)
			: KeyboardEvent(keycode) {}
		~KeyReleasedEvent() {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyboardReleasedEvent:" << _keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);

	};

}