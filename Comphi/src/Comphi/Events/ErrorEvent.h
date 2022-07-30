#pragma once
#include "Event.h"

namespace Comphi {

	class GenericErrorEvent : public Event
	{
	public:
		GenericErrorEvent(){}

		EVENT_CLASS_TYPE(GenericError);
		EVENT_CLASS_CATEGORY(EventCategoryError);
	};
}