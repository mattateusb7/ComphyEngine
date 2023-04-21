#pragma once
#include "Comphi/Renderer/ITexture.h"
namespace Comphi {
	
	/*class Texture : public ITexture
	{
	private:
		Texture()  = default;
		~Texture() = default;
	};*/

	typedef std::shared_ptr<ITexture> TexturePtr;
}
