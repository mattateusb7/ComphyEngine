#include "cphipch.h"
#include "GraphicsHandler.h"

namespace Comphi::Vulkan {

	static Comphi::Vulkan::GraphicsHandler graphicsHandler = GraphicsHandler();

	GraphicsHandler* GraphicsHandler::get()
	{
		return &graphicsHandler;
	}
}
