#pragma once

namespace Comphi {

	class ITexture
	{
	public:
		IFileRef* i_imageFileRef;
		ITexture(IFileRef& imageFileRef) : i_imageFileRef(&imageFileRef) {};
	};
}