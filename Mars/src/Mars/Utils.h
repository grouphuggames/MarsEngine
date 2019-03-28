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

	using MARS_NANOSECOND = std::chrono::nanoseconds;
	using MARS_MICROSECOND = std::chrono::microseconds;
	using MARS_MILLISECOND = std::chrono::milliseconds;
	using MARS_SECOND = std::chrono::seconds;

	struct TimerInfo
	{
		MARS_TIME time_scale;
		std::chrono::time_point<std::chrono::steady_clock> timer_start;
		std::chrono::time_point<std::chrono::steady_clock> timer_stop;
		f32 time_delta;
	};

	void StartTimer(TimerInfo& info)
	{
		info.timer_start = std::chrono::high_resolution_clock::now();
	}

	void StopTimer(TimerInfo& info)
	{
		info.timer_stop = std::chrono::high_resolution_clock::now();
		if (info.time_scale == MARS_TIME::MARS_NANOSECOND)
			info.time_delta = std::chrono::duration_cast<std::chrono::nanoseconds>(info.timer_stop - info.timer_start).count();
		else if (info.time_scale == MARS_TIME::MARS_MICROSECOND)
			info.time_delta = std::chrono::duration_cast<std::chrono::microseconds>(info.timer_stop - info.timer_start).count();
		else if (info.time_scale == MARS_TIME::MARS_MILLISECOND)
			info.time_delta = std::chrono::duration_cast<std::chrono::milliseconds>(info.timer_stop - info.timer_start).count();
		else if (info.time_scale == MARS_TIME::MARS_SECOND)
			info.time_delta = std::chrono::duration_cast<std::chrono::seconds>(info.timer_stop - info.timer_start).count();
	}
}