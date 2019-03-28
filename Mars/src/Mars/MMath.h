#pragma once
#include "Core.h"
#include <chrono>
#include <random>


namespace Mars
{
#define PI 3.14159265
#define HALF_PI PI / 2

	__forceinline f32 ToRadians(f32 val) { return val * (PI / 180.f); }
	__forceinline f32 ToDegrees(f32 val) { return val * (180.f / PI); }
	__forceinline f32 Min(f32 a, f32 b) { return (a < b) ? a : b; }
	__forceinline f32 Max(f32 a, f32 b) { return (a < b) ? b : a; }
	__forceinline f32 Abs(f32 a) { return *(reinterpret_cast<u32*>(&a)) &= 0xffffffff >> 1; }

	__forceinline f32 Floor(f32 val)
	{
		s32 a = (s32)val;
		return (f32)a;
	}

	__forceinline f32 Clamp(f32 bottom, f32 top, f32 val)
	{
		if (val > top)	val = top;
		if (val < bottom)	val = bottom;

		return val;
	}

	__forceinline f32 RandFloat(f32 min, f32 max)
	{
		std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<f32> distribution(min, max);
		return distribution(generator);
	}

	__forceinline s32 Min(s32 a, s32 b) { return b ^ ((a ^ b) & -(a < b)); }
	__forceinline s32 Max(s32 a, s32 b) { return a ^ ((a ^ b) & -(a < b)); }

	__forceinline u32 Abs(s32 a)
	{
		const s32 mask = a >> sizeof(s32) * CHAR_BIT - 1;
		return (a + mask) ^ mask;
	}

	__forceinline s32 Clamp(s32 bottom, s32 top, s32 val)
	{
		if (val > top)	val = top;
		if (val < bottom)	val = bottom;

		return val;
	}

	__forceinline s32 RandInt(s32 min, s32 max)
	{
		std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<s32> distribution(min, max);
		return distribution(generator);
	}
}
