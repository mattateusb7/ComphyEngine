#include "cphipch.h"
#include "GraphicsHandler.h"

namespace Comphi::Vulkan {

	static std::shared_ptr<GraphicsHandler> graphicsHandler = std::make_shared<GraphicsHandler>();
	//static GraphicsHandler graphicsHandler = GraphicsHandler();

	GraphicsHandler* GraphicsHandler::get()
	{
		return graphicsHandler.get();
	}

	void GraphicsHandler::DeleteStatic()
	{
		this->isInUse = false;
		this->~GraphicsHandler();
	}

	GraphicsHandler::~GraphicsHandler()
	{
		//KEEP Static Reference after program closed
		if(isInUse) 
			graphicsHandler = std::make_shared<GraphicsHandler>(*graphicsHandler.get());
	}
}
