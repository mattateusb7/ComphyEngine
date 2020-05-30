#pragma once

#include "Event.h"

namespace Comphi {

	class CPHI_API MouseMovedEvent : public Event
	{
	public:

		MouseMovedEvent(Uint posX, Uint posY) 
			: _posX(posX), _posY(posY){}

		inline Uint GetX() { return _posX; };
		inline Uint GetY() { return _posY; };

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << _posX << ", " << _posY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		Uint _posX, _posY;
	};
	
	class CPHI_API MouseScrolledEvent : public Event
	{
	public:

		MouseScrolledEvent(int offsetX, int offsetY)
			: _offsetX(offsetX), _offsetY(offsetY){}

		inline Uint GetOffsetX() { return _offsetX; };
		inline Uint GetOffsetY() { return _offsetY; };

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << _offsetX << ", " << _offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		Uint _offsetX, _offsetY;
	};


	class CPHI_API MouseButtonEvent : public Event
	{
	public:

		inline Uint GetMouseButton() { return _button; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouseButton);
	
	protected :
		MouseButtonEvent(int button)
			: _button(button) {}

		Uint _button;
	};

	class CPHI_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:

		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button){}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);

	};
	
	class CPHI_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:

		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button){}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << _button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);

	};

}