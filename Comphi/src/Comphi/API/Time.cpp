#include "cphipch.h"
#include "Time.h"

namespace Comphi {
	
	const TimePoint Time::programTime = std::chrono::high_resolution_clock::now();

	TimePoint Time::currentTime()
	{
		return std::chrono::high_resolution_clock::now();
	}

	TimePoint Time::Start()
	{
		return startTime = currentTime();
	}

	TimePoint Time::Stop()
	{
		return stopTime = currentTime();
	}

	float Time::deltaTime()
	{
		return std::chrono::duration<float, std::chrono::seconds::period>(currentTime() - startTime).count();
	}


}