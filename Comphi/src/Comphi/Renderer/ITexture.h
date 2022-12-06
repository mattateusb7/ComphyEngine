#pragma once
#include "Comphi/Platform/IFileRef.h"
namespace Comphi {

	class ITexture
	{
	public:
		IFileRef* i_imageFileRef;
		ITexture() = default;
		ITexture(IFileRef& imageFileRef) : i_imageFileRef(&imageFileRef) {};
	};
}