#pragma once
#include "Core.h"
#include <chrono>


namespace Mars
{
	enum class MARS_TIME
	{
		MARS_NANOSECOND,
		MARS_MICROSECOND,
		MARS_MILLISECOND,
		MARS_SECOND
	};

#ifdef _WIN32
	struct TimerInfo
	{
		MARS_TIME time_scale;
		std::chrono::time_point<std::chrono::steady_clock> timer_start;
		std::chrono::time_point<std::chrono::steady_clock> timer_stop;
		f32 time_delta;
	};
#else
    struct TimerInfo
	{
		MARS_TIME time_scale;
		std::chrono::time_point<std::chrono::system_clock> timer_start;
		std::chrono::time_point<std::chrono::system_clock> timer_stop;
		f32 time_delta;
	};
#endif

	void StartTimer(TimerInfo& info)
	{
		info.timer_start = std::chrono::high_resolution_clock::now();
	}

	void StopTimer(TimerInfo& info)
	{
		info.timer_stop = std::chrono::high_resolution_clock::now();
		if (info.time_scale == MARS_TIME::MARS_NANOSECOND)
			info.time_delta = (f32)std::chrono::duration_cast<std::chrono::nanoseconds>(info.timer_stop - info.timer_start).count();
		else if (info.time_scale == MARS_TIME::MARS_MICROSECOND)
			info.time_delta = (f32)std::chrono::duration_cast<std::chrono::microseconds>(info.timer_stop - info.timer_start).count();
		else if (info.time_scale == MARS_TIME::MARS_MILLISECOND)
			info.time_delta = (f32)std::chrono::duration_cast<std::chrono::milliseconds>(info.timer_stop - info.timer_start).count();
		else if (info.time_scale == MARS_TIME::MARS_SECOND)
			info.time_delta = (f32)std::chrono::duration_cast<std::chrono::seconds>(info.timer_stop - info.timer_start).count();
	}
}
