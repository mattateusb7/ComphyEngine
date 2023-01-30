#pragma once
#include "IObject.h"
#include "Comphi/Platform/IFileRef.h"

namespace Comphi {

	class ITexture : public IObject
	{
	public:
		ITexture() = default;
		ITexture(IFileRef& imageFileRef) : imageFileRef(&imageFileRef) {};
		IFileRef* imageFileRef;
	};
}