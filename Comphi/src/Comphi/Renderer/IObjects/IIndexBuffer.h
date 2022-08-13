#pragma once

namespace Comphi {
	class IIndexBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}