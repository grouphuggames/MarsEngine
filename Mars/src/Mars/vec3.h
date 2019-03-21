#pragma once
#include "Core.h"
#include <xmmintrin.h>	// SSE2 instructions


// still needs to be profiled...
namespace Mars
{
	class slow_vec3
	{
	public:
		__forceinline slow_vec3() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		__forceinline explicit slow_vec3(f32 val) : x(val), y(val), z(val), w(0.f) {}
		__forceinline explicit slow_vec3(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(0.f) {}

		struct
		{
			f32 x, y, z;
		};

	private:
		f32 w;
	};

	__forceinline slow_vec3 operator+(slow_vec3 a, slow_vec3 b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;

		return a;
	}

	__forceinline slow_vec3 operator-(slow_vec3 a, slow_vec3 b)
	{
		a.x -= b.x;
		a.y -= b.y;
		a.z -= b.z;

		return a;
	}

	__forceinline slow_vec3 operator*(slow_vec3 a, f32 b)
	{
		a.x *= b;
		a.y *= b;
		a.z *= b;

		return a;
	}

	__forceinline slow_vec3 operator*(f32 a, slow_vec3 b)
	{
		b.x *= a;
		b.y *= a;
		b.z *= a;

		return b;
	}

	__forceinline slow_vec3 operator/(slow_vec3 a, f32 b)
	{
		a.x /= b;
		a.y /= b;
		a.z /= b;

		return a;
	}

	class vec3
	{
	public:
		__forceinline vec3() : data(_mm_set_ps1(0.f)) {}
		__forceinline explicit vec3(f32 val) : data(_mm_set_ps(val, val, val, 0.f)) {}
		__forceinline explicit vec3(f32 _x, f32 _y, f32 _z, f32 _w) : data(_mm_set_ps(_x, _y, _z, 0.f)) {}

		__m128 data;
	};

	__forceinline vec3 operator+(vec3 a, vec3 b)
	{
		a.data = _mm_add_ps(a.data, b.data);

		return a;
	}

	__forceinline vec3 operator-(vec3 a, vec3 b)
	{
		a.data = _mm_sub_ps(a.data, b.data);

		return a;
	}

	__forceinline vec3 operator*(vec3 a, f32 b)
	{
		__m128 tmp = _mm_set1_ps(b);
		a.data = _mm_mul_ps(a.data, tmp);

		return a;
	}

	__forceinline vec3 operator/(vec3 a, f32 b)		// might be faster way to do this... _mm_set1_ps(1 / b); _mm_mul_ps(a.data, b); AND _mm_set1_ps(_mm_rcp_ps(b)); _mm_mul_ps(a.data, b); were both same speed
	{
		__m128 tmp = _mm_set1_ps(b);
		a.data = _mm_div_ps(a.data, tmp);

		return a;
	}
}
