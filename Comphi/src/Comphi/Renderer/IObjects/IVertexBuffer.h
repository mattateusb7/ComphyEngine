#pragma once

namespace Comphi {
	class IVertexBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}