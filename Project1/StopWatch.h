#pragma once
#include <chrono>
#include <iostream>
#include <map>

template <typename Clock = std::chrono::high_resolution_clock>
class StopWatch {
	public: 
		StopWatch() 
		{
		};

		void reset()
		{
			*this = StopWatch();
		}

		typename Clock::time_point Start() 
		{
			startTime = Clock::now();
			return startTime;
		}

		typename Clock::duration elapsed() const
		{
			return Clock::now() - startTime;
		}

		std::chrono::high_resolution_clock::duration Stop()
		{
			auto currentTime = Clock::now();
			auto elapsed = currentTime - startTime;
			return elapsed;
		}

	private:
		std::chrono::high_resolution_clock::time_point startTime;
		std::chrono::high_resolution_clock::time_point currentTime;
};

template <typename T, typename Rep, typename Period>
T cast_duration(const std::chrono::duration<Rep, Period>& duration)
{
	return duration.count() * static_cast<T>(Period::num) / static_cast<T>(Period::den);
}
