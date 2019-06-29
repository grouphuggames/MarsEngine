#pragma once
#include "Core.h"
#include <emmintrin.h>	// SSE2 instructions


namespace Mars
{
	class vec4
	{
	public:
		MARS_INLINE vec4() : data(_mm_set_ps1(0.f)) {}
		MARS_INLINE explicit vec4(f32 val) : data(_mm_set_ps1(val)) {}
		MARS_INLINE explicit vec4(f32 _x, f32 _y, f32 _z, f32 _w) : data(_mm_set_ps(_w, _z, _y, _x)) {}

		MARS_INLINE friend vec4 operator+(vec4 a, vec4 b)
		{
			a.data = _mm_add_ps(a.data, b.data);

			return a;
		}

		MARS_INLINE friend vec4 operator+=(vec4& a, vec4 b)
		{
			a = a + b;

			return a;
		}

		MARS_INLINE friend vec4 operator-(vec4 a, vec4 b)
		{
			a.data = _mm_sub_ps(a.data, b.data);

			return a;
		}

		MARS_INLINE friend vec4 operator*(vec4 a, vec4 b)
		{
			a.data = _mm_mul_ps(a.data, b.data);

			return a;
		}

		MARS_INLINE friend vec4 operator*(vec4 a, f32 b)
		{
			__m128 tmp = _mm_set1_ps(b);
			a.data = _mm_mul_ps(a.data, tmp);

			return a;
		}

		MARS_INLINE friend vec4 operator*(f32 a, vec4 b)
		{
			__m128 tmp = _mm_set1_ps(a);
			b.data = _mm_mul_ps(b.data, tmp);

			return b;
		}

		MARS_INLINE friend vec4 operator/(vec4 a, f32 b)		// might be faster way to do this... _mm_set1_ps(1 / b); _mm_mul_ps(a.data, b); AND _mm_set1_ps(_mm_rcp_ps(b)); _mm_mul_ps(a.data, b); were both same speed
		{
			__m128 tmp = _mm_set1_ps(b);
			a.data = _mm_div_ps(a.data, tmp);

			return a;
		}

		MARS_INLINE friend vec4 operator/=(vec4& a, f32 b)
		{
			a = a / b;

			return a;
		}

		friend std::ostream& operator<<(std::ostream& os, vec4 a)
		{
			return os << "{ " << a.x() << ", " << a.y() << ", " << a.z() << ", " << a.w() << " }";
		}

		MARS_INLINE f32 x() const
		{
			return _mm_cvtss_f32(data);
		}

		MARS_INLINE f32 y() const
		{
			return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(1, 1, 1, 1)));
		}

		MARS_INLINE f32 z() const
		{
			return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(2, 2, 2, 2)));
		}

		MARS_INLINE f32 w() const
		{
			return _mm_cvtss_f32(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 3, 3, 3)));
		}

		MARS_INLINE static f32 Dot(vec4 a, vec4 b)
		{
			return (a * b).Sum();
		}

		MARS_INLINE f32 Length()
		{
			return sqrtf(Dot(*this, *this));
		}

		__m128 data;

	private:
		MARS_INLINE f32 Sum()
		{
			return (this->x() + this->y() + this->z() + this->w());
		}
	};
}
