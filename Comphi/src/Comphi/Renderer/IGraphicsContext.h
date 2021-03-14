#pragma once

namespace Comphi {
	class IGraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Draw() = 0;
		virtual void ResizeWindow(uint x, uint y) = 0;
	};
}