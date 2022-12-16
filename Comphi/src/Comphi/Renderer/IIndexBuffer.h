#pragma once

namespace Comphi {

	typedef uint32_t Index;
	typedef std::vector<Index> IndexArray;

	class IIndexBuffer
	{
	public:
		uint32_t i_indexCount;
	};
}