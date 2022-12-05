#pragma once

#include "Comphi/Renderer/ITexture.h"

namespace Comphi {

	class Texture : public ITexture
	{
		Texture(IFileRef& imageFileRef) : ITexture(imageFileRef) {};
	};


};

