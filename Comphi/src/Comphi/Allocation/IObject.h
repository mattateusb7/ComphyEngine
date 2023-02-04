#pragma once

namespace Comphi {

	enum ObjectType {
		Texture,
		Buffer
	};

	class IObject
	{
	public:
		virtual void cleanUp() = 0;
	protected:
		ObjectType type;
	};
}
