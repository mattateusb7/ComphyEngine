#pragma once
#include "Comphi/Allocation/IObject.h"
#include "Comphi/Platform/IFileRef.h"

namespace Comphi {

	class ITexture : public IObject
	{
	public:
		ITexture() = default;
		ITexture(IFileRef& imageFileRef) : imageFileRef(&imageFileRef) {};
		//ITexture(IFileRef& imageFileRef) : imageFileRef(&imageFileRef) {}; //TODO: Pixeldata array
		IFileRef* imageFileRef;
		virtual void cleanUp() override {};
		//virtual void* getDataPtr() {};
	};
}