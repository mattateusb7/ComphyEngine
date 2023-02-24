#pragma once
#include "Comphi/API/SceneGraph/Scene.h"

namespace Comphi {
	class IGraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void Draw() = 0;
		virtual void SetScenes(SceneGraph& scenes) = 0;
		virtual void ResizeWindow(uint x, uint y) = 0;
		virtual void ResizeFramebuffer(uint x, uint y) = 0;
		virtual void CleanUp() = 0;
	};
}