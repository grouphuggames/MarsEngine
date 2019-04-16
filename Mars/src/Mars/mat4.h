#pragma once
#include "Core.h"
#include "vec3.h"
#include <xmmintrin.h>


namespace Mars
{
	class mat4
	{
	public:
		__forceinline mat4() {}

		__forceinline explicit mat4(f32 val)
		{
			__m128 a = _mm_set_ps(0.f, 0.f, 0.f, val);
			__m128 b = _mm_set_ps(0.f, 0.f, val , 0.f);
			__m128 c = _mm_set_ps(0.f, val, 0.f, 0.f);
			__m128 d = _mm_set_ps(val, 0.f, 0.f, 0.f);

			data[0] = a;
			data[1] = b;
			data[2] = c;
			data[3] = d;
		}

		static mat4 LookAtLH(vec3 position, vec3 target, vec3 up)
		{
			vec3 z_axis = vec3::Normalize(target - position);
			vec3 x_axis = vec3::Normalize(vec3::Cross(up, z_axis));
			vec3 y_axis = vec3::Cross(z_axis, x_axis);

			__m128 a = _mm_set_ps(0.f, z_axis.x(), y_axis.x(), x_axis.x());
			__m128 b = _mm_set_ps(0.f, z_axis.y(), y_axis.y(), x_axis.y());
			__m128 c = _mm_set_ps(0.f, z_axis.z(), y_axis.z(), x_axis.z());
			__m128 d = _mm_set_ps(1.f, -vec3::Dot(z_axis, position), -vec3::Dot(y_axis, position), -vec3::Dot(x_axis, position));

			return mat4(a, b, c, d);
		}

		static mat4 PerspectiveFovLH(f32 angle, f32 aspect_ratio, f32 _near, f32 _far)
		{
			f32 h = 1.f / tanf(angle / 2);
			f32 w = h / aspect_ratio;

			__m128 a = _mm_set_ps(0.f, 0.f, 0.f, w);
			__m128 b = _mm_set_ps(0.f, 0.f, h, 0.f);
			__m128 c = _mm_set_ps(1.f, _far / (_far - _near), 0.f, 0.f);
			__m128 d = _mm_set_ps(0.f, (-_near * _far) / (_far - _near), 0.f, 0.f);

			return mat4(a, b, c, d);
		}

		static mat4 Transpose(mat4 matrix)		// this is slow, simdize this later to make this way faster...
		{
			mat4 result;

			result.data[0].m128_f32[0] = matrix.data[0].m128_f32[0];
			result.data[0].m128_f32[1] = matrix.data[1].m128_f32[0];
			result.data[0].m128_f32[2] = matrix.data[2].m128_f32[0];
			result.data[0].m128_f32[3] = matrix.data[3].m128_f32[0];

			result.data[1].m128_f32[0] = matrix.data[0].m128_f32[1];
			result.data[1].m128_f32[1] = matrix.data[1].m128_f32[1];
			result.data[1].m128_f32[2] = matrix.data[2].m128_f32[1];
			result.data[1].m128_f32[3] = matrix.data[3].m128_f32[1];

			result.data[2].m128_f32[0] = matrix.data[0].m128_f32[2];
			result.data[2].m128_f32[1] = matrix.data[1].m128_f32[2];
			result.data[2].m128_f32[2] = matrix.data[2].m128_f32[2];
			result.data[2].m128_f32[3] = matrix.data[3].m128_f32[2];

			result.data[3].m128_f32[0] = matrix.data[0].m128_f32[3];
			result.data[3].m128_f32[1] = matrix.data[1].m128_f32[3];
			result.data[3].m128_f32[2] = matrix.data[2].m128_f32[3];
			result.data[3].m128_f32[3] = matrix.data[3].m128_f32[3];

			return result;
		}

		static mat4 Scale(vec3 scale)
		{
			__m128 a = _mm_set_ps(scale.x(), 0.f, 0.f, 0.f);
			__m128 b = _mm_set_ps(0.f, scale.y(), 0.f, 0.f);
			__m128 c = _mm_set_ps(0.f, 0.f, scale.z(), 0.f);
			__m128 d = _mm_set_ps(0.f, 0.f, 0.f, 1.f);

			return mat4(a, b, c, d);
		}

		static mat4 Rotate(vec3 axis, f32 angle)		// we're gonna try this out using quaternions later...
		{
			mat4 result(1.f);

			f32 c = cosf(angle);
			f32 s = sinf(angle);
			f32 omc = 1.f - c;
			
			result.data[0].m128_f32[0] = axis.x() * omc + c;
			result.data[0].m128_f32[1] = axis.y() * axis.x() * omc + axis.z() * s;
			result.data[0].m128_f32[2] = axis.x() * axis.z() * omc - axis.y() * s;

			result.data[1].m128_f32[0] = axis.x() * axis.y() * omc - axis.z() * s;
			result.data[1].m128_f32[1] = axis.y() * omc + c;
			result.data[1].m128_f32[2] = axis.y() * axis.z() * omc + axis.x() * s;

			result.data[2].m128_f32[0] = axis.x() * axis.z() * omc + axis.y() * s;
			result.data[2].m128_f32[1] = axis.y() * axis.z() * omc - axis.x() * s;
			result.data[2].m128_f32[2] = axis.z() * omc + c;

			return result;
		}

		static mat4 Translate(vec3 translate)
		{
			mat4 result = mat4(1.f);
			result.data[3] = _mm_set_ps(1.f, translate.z(), translate.y(), translate.x());

			return result;
		}

		__forceinline friend mat4 operator*(mat4 left, const mat4 right)	// this has to be done painfully slow until i can figure out the proper way to simdize this...
		{
			mat4 result;

			result.data[0].m128_f32[0] = left.data[0].m128_f32[0] * right.data[0].m128_f32[0] + left.data[0].m128_f32[1] * right.data[1].m128_f32[0] + left.data[0].m128_f32[2] * right.data[2].m128_f32[0] + left.data[0].m128_f32[3] * right.data[3].m128_f32[0];
			result.data[0].m128_f32[1] = left.data[0].m128_f32[0] * right.data[0].m128_f32[1] + left.data[0].m128_f32[1] * right.data[1].m128_f32[1] + left.data[0].m128_f32[2] * right.data[2].m128_f32[1] + left.data[0].m128_f32[3] * right.data[3].m128_f32[1];
			result.data[0].m128_f32[2] = left.data[0].m128_f32[0] * right.data[0].m128_f32[2] + left.data[0].m128_f32[1] * right.data[1].m128_f32[2] + left.data[0].m128_f32[2] * right.data[2].m128_f32[2] + left.data[0].m128_f32[3] * right.data[3].m128_f32[2];
			result.data[0].m128_f32[3] = left.data[0].m128_f32[0] * right.data[0].m128_f32[3] + left.data[0].m128_f32[1] * right.data[1].m128_f32[3] + left.data[0].m128_f32[2] * right.data[2].m128_f32[3] + left.data[0].m128_f32[3] * right.data[3].m128_f32[3];

			result.data[1].m128_f32[0] = left.data[1].m128_f32[0] * right.data[0].m128_f32[0] + left.data[1].m128_f32[1] * right.data[1].m128_f32[0] + left.data[1].m128_f32[2] * right.data[2].m128_f32[0] + left.data[1].m128_f32[3] * right.data[3].m128_f32[0];
			result.data[1].m128_f32[1] = left.data[1].m128_f32[0] * right.data[0].m128_f32[1] + left.data[1].m128_f32[1] * right.data[1].m128_f32[1] + left.data[1].m128_f32[2] * right.data[2].m128_f32[1] + left.data[1].m128_f32[3] * right.data[3].m128_f32[1];
			result.data[1].m128_f32[2] = left.data[1].m128_f32[0] * right.data[0].m128_f32[2] + left.data[1].m128_f32[1] * right.data[1].m128_f32[2] + left.data[1].m128_f32[2] * right.data[2].m128_f32[2] + left.data[1].m128_f32[3] * right.data[3].m128_f32[2];
			result.data[1].m128_f32[3] = left.data[1].m128_f32[0] * right.data[0].m128_f32[3] + left.data[1].m128_f32[1] * right.data[1].m128_f32[3] + left.data[1].m128_f32[2] * right.data[2].m128_f32[3] + left.data[1].m128_f32[3] * right.data[3].m128_f32[3];

			result.data[2].m128_f32[0] = left.data[2].m128_f32[0] * right.data[0].m128_f32[0] + left.data[2].m128_f32[1] * right.data[1].m128_f32[0] + left.data[2].m128_f32[2] * right.data[2].m128_f32[0] + left.data[0].m128_f32[3] * right.data[3].m128_f32[0];
			result.data[2].m128_f32[1] = left.data[2].m128_f32[0] * right.data[0].m128_f32[1] + left.data[2].m128_f32[1] * right.data[1].m128_f32[1] + left.data[2].m128_f32[2] * right.data[2].m128_f32[1] + left.data[0].m128_f32[3] * right.data[3].m128_f32[1];
			result.data[2].m128_f32[2] = left.data[2].m128_f32[0] * right.data[0].m128_f32[2] + left.data[2].m128_f32[1] * right.data[1].m128_f32[2] + left.data[2].m128_f32[2] * right.data[2].m128_f32[2] + left.data[0].m128_f32[3] * right.data[3].m128_f32[2];
			result.data[2].m128_f32[3] = left.data[2].m128_f32[0] * right.data[0].m128_f32[3] + left.data[2].m128_f32[1] * right.data[1].m128_f32[3] + left.data[2].m128_f32[2] * right.data[2].m128_f32[3] + left.data[0].m128_f32[3] * right.data[3].m128_f32[3];

			result.data[3].m128_f32[0] = left.data[3].m128_f32[0] * right.data[0].m128_f32[0] + left.data[3].m128_f32[1] * right.data[1].m128_f32[0] + left.data[3].m128_f32[2] * right.data[2].m128_f32[0] + left.data[3].m128_f32[3] * right.data[3].m128_f32[0];
			result.data[3].m128_f32[1] = left.data[3].m128_f32[0] * right.data[0].m128_f32[1] + left.data[3].m128_f32[1] * right.data[1].m128_f32[1] + left.data[3].m128_f32[2] * right.data[2].m128_f32[1] + left.data[3].m128_f32[3] * right.data[3].m128_f32[1];
			result.data[3].m128_f32[2] = left.data[3].m128_f32[0] * right.data[0].m128_f32[2] + left.data[3].m128_f32[1] * right.data[1].m128_f32[2] + left.data[3].m128_f32[2] * right.data[2].m128_f32[2] + left.data[3].m128_f32[3] * right.data[3].m128_f32[2];
			result.data[3].m128_f32[3] = left.data[3].m128_f32[0] * right.data[0].m128_f32[3] + left.data[3].m128_f32[1] * right.data[1].m128_f32[3] + left.data[3].m128_f32[2] * right.data[2].m128_f32[3] + left.data[3].m128_f32[3] * right.data[3].m128_f32[3];

			return result;
		}

	private:
		__m128 data[4];

		__forceinline explicit mat4(__m128 a, __m128 b, __m128 c, __m128 d)
		{
			data[0] = a;
			data[1] = b;
			data[2] = c;
			data[3] = d;
		}
	};
}
