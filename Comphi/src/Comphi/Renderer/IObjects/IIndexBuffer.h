#pragma once

namespace Comphi {

	typedef uint16_t Index;
	typedef std::vector<Index> IndexArray;

	class IIndexBuffer
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};
}