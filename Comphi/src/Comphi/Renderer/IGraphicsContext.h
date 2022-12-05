#pragma once
#include "Comphi/API/Scene.h"

namespace Comphi {
	class IGraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void Draw() = 0;
		virtual void SetScenes(MultiScene& scenes) = 0; //TODO: Refactor ?
		virtual void ResizeWindow(uint x, uint y) = 0;
		virtual void ResizeFramebuffer(uint x, uint y) = 0;
		virtual void CleanUp() = 0;
	};
}