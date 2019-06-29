#pragma once
#include "Core.h"
#include <chrono>
#include <random>


namespace Mars
{
#define PI 3.14159265
#define HALF_PI PI / 2

	MARS_INLINE f32 ToRadians(f32 val) { return val * ((f32)PI / 180.f); }
	MARS_INLINE f32 ToDegrees(f32 val) { return val * (180.f / (f32)PI); }
	MARS_INLINE f32 Min(f32 a, f32 b) { return (a < b) ? a : b; }
	MARS_INLINE f32 Max(f32 a, f32 b) { return (a < b) ? b : a; }

	MARS_INLINE f32 Floor(f32 val)
	{
		s32 a = (s32)val;
		return (f32)a;
	}

	MARS_INLINE f32 Clamp(f32 bottom, f32 top, f32 val)
	{
		if (val > top)	val = top;
		if (val < bottom)	val = bottom;

		return val;
	}

	MARS_INLINE f32 RandFloat(f32 min, f32 max)
	{
		std::mt19937 generator((u32)std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<f32> distribution(min, max);
		return distribution(generator);
	}

	MARS_INLINE s32 Min(s32 a, s32 b) { return b ^ ((a ^ b) & -(a < b)); }
	MARS_INLINE s32 Max(s32 a, s32 b) { return a ^ ((a ^ b) & -(a < b)); }

	MARS_INLINE u32 Abs(s32 a)
	{
		const s32 mask = a >> ((sizeof(s32) * 8) - 1);
		return (a + mask) ^ mask;
	}

	MARS_INLINE s32 Clamp(s32 bottom, s32 top, s32 val)
	{
		if (val > top)	val = top;
		if (val < bottom)	val = bottom;

		return val;
	}

	MARS_INLINE s32 RandInt(s32 min, s32 max)
	{
		std::mt19937 generator((u32)std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<s32> distribution(min, max);
		return distribution(generator);
	}
}
