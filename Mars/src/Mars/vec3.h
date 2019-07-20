#pragma once
#include "Core.h"
#include "MMath.h"
#include <emmintrin.h>	// SSE2 instructions

#define SHUFFLE3(V, X,Y,Z) vec3(_mm_shuffle_ps((V).data, (V).data, _MM_SHUFFLE(Z,Z,Y,X)))


namespace Mars
{
	class vec3
	{
	public:
		MARS_INLINE vec3() : data(_mm_set_ps1(0.f)) {}
		MARS_INLINE explicit vec3(f32 val) : data(_mm_set_ps(0.f, val, val, val)) {}
		MARS_INLINE explicit vec3(f32 _x, f32 _y, f32 _z) : data(_mm_set_ps(0.f, _z, _y, _x)) {}

		MARS_INLINE friend vec3 operator+(vec3 a, vec3 b)
		{
			a.data = _mm_add_ps(a.data, b.data);

			return a;
		}

		MARS_INLINE friend vec3 operator+=(vec3& a, vec3 b)
		{
			a = a + b;

			return a;
		}

		MARS_INLINE friend vec3 operator-(vec3 a, vec3 b)
		{
			a.data = _mm_sub_ps(a.data, b.data);

			return a;
		}

		MARS_INLINE friend vec3 operator-=(vec3& a, vec3 b)
		{
			a = a - b;

			return a;
		}

		MARS_INLINE friend bool operator==(const vec3& a, const vec3 b)
		{
			if (a.x() == b.x() && a.y() == b.y() && a.z() == b.z())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		MARS_INLINE friend vec3 operator*(vec3 a, vec3 b)
		{
			a.data = _mm_mul_ps(a.data, b.data);

			return a;
		}

		MARS_INLINE friend vec3 operator*(vec3 a, f32 b)
		{
			__m128 tmp = _mm_set1_ps(b);
			a.data = _mm_mul_ps(a.data, tmp);

			return a;
		}

		MARS_INLINE friend vec3 operator*(f32 a, vec3 b)
		{
			__m128 tmp = _mm_set1_ps(a);
			b.data = _mm_mul_ps(b.data, tmp);

			return b;
		}

		MARS_INLINE friend vec3 operator/(vec3 a, f32 b)		// might be faster way to do this... _mm_set1_ps(1 / b); _mm_mul_ps(a.data, b); AND _mm_set1_ps(_mm_rcp_ps(b)); _mm_mul_ps(a.data, b); were both same speed
		{
			__m128 tmp = _mm_set1_ps(b);
			a.data = _mm_div_ps(a.data, tmp);

			return a;
		}

		friend std::ostream& operator<<(std::ostream& os, vec3 a)
		{
			return os << "{ " << a.x() << ", " << a.y() << ", " << a.z() << " }";
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

		MARS_INLINE static f32 Dot(vec3 a, vec3 b)
		{
			return (a * b).Sum();
		}

		MARS_INLINE f32 Length()
		{
			return sqrtf(Dot(*this, *this));
		}

		MARS_INLINE static vec3 Normalize(vec3 v)
		{
			return (v * (1.f / v.Length()));
		}

		MARS_INLINE static vec3 Cross(vec3 a, vec3 b)
		{
			return (a.zxy() * b - a * b.zxy()).zxy();
		}

		__m128 data;

	private:
		MARS_INLINE explicit vec3(__m128 v) : data(v) {}

		MARS_INLINE vec3 yzx() const
		{
			return SHUFFLE3(*this, 1, 2, 0);
		}

		MARS_INLINE vec3 zxy() const
		{
			return SHUFFLE3(*this, 2, 0, 1);
		}

		MARS_INLINE f32 Sum()
		{
			return (this->x() + this->y() + this->z());
		}
	};
}
