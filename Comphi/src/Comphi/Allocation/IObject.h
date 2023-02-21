#pragma once

namespace Comphi {

	class IObject
	{
	protected:
		virtual void cleanUp() = 0;
	};
}
