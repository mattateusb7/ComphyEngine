#pragma once
namespace Comphi {

	typedef std::chrono::steady_clock::time_point TimePoint;

	class Time
	{
	public :
		static inline TimePoint currentTime();

		TimePoint Start();
		TimePoint Stop();
		inline float deltaTime();
		
		const static TimePoint programTime;
	protected:
		TimePoint startTime = programTime;
		TimePoint stopTime = programTime;
	};

	const TimePoint Time::programTime = std::chrono::high_resolution_clock::now();
}


