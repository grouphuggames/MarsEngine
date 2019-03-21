#pragma once
#include "Core.h"
#include <xmmintrin.h>	// SSE2 instructions


namespace Mars
{
	class slow_vec4
	{
	public:
		__forceinline slow_vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		__forceinline explicit slow_vec4(f32 val) : x(val), y(val), z(val), w(val) {}
		__forceinline explicit slow_vec4(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}

		struct
		{
			f32 x, y, z, w;
		};
	};

	__forceinline slow_vec4 operator+(slow_vec4 a, slow_vec4 b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;

		return a;
	}

	__forceinline slow_vec4 operator-(slow_vec4 a, slow_vec4 b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;
		a.w -= b.w;

		return a;
	}

	__forceinline slow_vec4 operator*(slow_vec4 a, f32 b)
	{
		a.x *= b;
		a.y *= b;
		a.z *= b;
		a.w *= b;

		return a;
	}

	__forceinline slow_vec4 operator*(f32 a, slow_vec4 b)
	{
		b.x *= a;
		b.y *= a;
		b.z *= a;
		b.w *= a;

		return b;
	}

	__forceinline slow_vec4 operator/(slow_vec4 a, f32 b)
	{
		a.x /= b;
		a.y /= b;
		a.z /= b;
		a.w /= b;

		return a;
	}

	class vec4
	{
	public:
		__forceinline vec4() : data(_mm_set_ps1(0.f)) {}
		__forceinline explicit vec4(f32 val) : data(_mm_set_ps1(val)) {}
		__forceinline explicit vec4(f32 _x, f32 _y, f32 _z, f32 _w) : data(_mm_set_ps(_x, _y, _z, _w)) {}

		__m128 data;
	};

	__forceinline vec4 operator+(vec4 a, vec4 b)
	{
		a.data = _mm_add_ps(a.data, b.data);

		return a;
	}

	__forceinline vec4 operator-(vec4 a, vec4 b)
	{
		a.data = _mm_sub_ps(a.data, b.data);

		return a;
	}

	__forceinline vec4 operator*(vec4 a, f32 b)
	{
		__m128 tmp = _mm_set1_ps(b);
		a.data = _mm_mul_ps(a.data, tmp);

		return a;
	}

	__forceinline vec4 operator/(vec4 a, f32 b)		// might be faster way to do this... _mm_set1_ps(1 / b); _mm_mul_ps(a.data, b); AND _mm_set1_ps(_mm_rcp_ps(b)); _mm_mul_ps(a.data, b); were both same speed
	{
		__m128 tmp = _mm_set1_ps(b);
		a.data = _mm_div_ps(a.data, tmp);

		return a;
	}
}
