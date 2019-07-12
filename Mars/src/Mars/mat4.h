#pragma once
#include "Core.h"
#include "vec3.h"
#include <emmintrin.h>


namespace Mars
{
	class mat4
	{
	public:
		MARS_INLINE mat4() {}

		MARS_INLINE explicit mat4(f32 val)
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
#ifdef _WIN32
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
#else
            result.data[0][0] = matrix.data[0][0];
			result.data[0][1] = matrix.data[1][0];
			result.data[0][2] = matrix.data[2][0];
			result.data[0][3] = matrix.data[3][0];

			result.data[1][0] = matrix.data[0][1];
			result.data[1][1] = matrix.data[1][1];
			result.data[1][2] = matrix.data[2][1];
			result.data[1][3] = matrix.data[3][1];

			result.data[2][0] = matrix.data[0][2];
			result.data[2][1] = matrix.data[1][2];
			result.data[2][2] = matrix.data[2][2];
			result.data[2][3] = matrix.data[3][2];

			result.data[3][0] = matrix.data[0][3];
			result.data[3][1] = matrix.data[1][3];
			result.data[3][2] = matrix.data[2][3];
			result.data[3][3] = matrix.data[3][3];
#endif

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

			axis = vec3::Normalize(axis);
            
#ifdef _WIN32
			result.data[0].m128_f32[0] = axis.x() * axis.x() * omc + c;
			result.data[0].m128_f32[1] = axis.y() * axis.x() * omc - axis.z() * s;
			result.data[0].m128_f32[2] = axis.x() * axis.z() * omc + axis.y() * s;

			result.data[1].m128_f32[0] = axis.x() * axis.y() * omc + axis.z() * s;
			result.data[1].m128_f32[1] = axis.y() * axis.y() * omc + c;
			result.data[1].m128_f32[2] = axis.y() * axis.z() * omc - axis.x() * s;

			result.data[2].m128_f32[0] = axis.x() * axis.z() * omc - axis.y() * s;
			result.data[2].m128_f32[1] = axis.y() * axis.z() * omc + axis.x() * s;
			result.data[2].m128_f32[2] = axis.z() * axis.z() * omc + c;
            
#else
            result.data[0][0] = axis.x() * axis.x() * omc + c;
			result.data[0][1] = axis.y() * axis.x() * omc - axis.z() * s;
			result.data[0][2] = axis.x() * axis.z() * omc + axis.y() * s;

			result.data[1][0] = axis.x() * axis.y() * omc + axis.z() * s;
			result.data[1][1] = axis.y() * axis.y() * omc + c;
			result.data[1][2] = axis.y() * axis.z() * omc - axis.x() * s;

			result.data[2][0] = axis.x() * axis.z() * omc - axis.y() * s;
			result.data[2][1] = axis.y() * axis.z() * omc + axis.x() * s;
			result.data[2][2] = axis.z() * axis.z() * omc + c;
#endif

			return result;
		}

		static mat4 Translate(vec3 translate)
		{
			mat4 result = mat4(1.f);
			result.data[3] = _mm_set_ps(1.f, translate.z(), translate.y(), translate.x());

			return result;
		}
        
#ifdef _WIN32
		friend std::ostream& operator<<(std::ostream& os, mat4 a)
		{
			return os << "\n"
				<< a.data[0].m128_f32[0] << " " << a.data[0].m128_f32[1] << " " << a.data[0].m128_f32[2] << " " << a.data[0].m128_f32[3] << "\n"
				<< a.data[1].m128_f32[0] << " " << a.data[1].m128_f32[1] << " " << a.data[1].m128_f32[2] << " " << a.data[1].m128_f32[3] << "\n"
				<< a.data[2].m128_f32[0] << " " << a.data[2].m128_f32[1] << " " << a.data[2].m128_f32[2] << " " << a.data[2].m128_f32[3] << "\n"
				<< a.data[3].m128_f32[0] << " " << a.data[3].m128_f32[1] << " " << a.data[3].m128_f32[2] << " " << a.data[3].m128_f32[3] << "\n";
		}
#else
        friend std::ostream& operator<<(std::ostream& os, mat4 a)
		{
			return os << "\n"
                << a.data[0][0] << " " << a.data[0][1] << " " << a.data[0][2] << " " << a.data[0][3] << "\n"
                << a.data[1][0] << " " << a.data[1][1] << " " << a.data[1][2] << " " << a.data[1][3] << "\n"
                << a.data[2][0] << " " << a.data[2][1] << " " << a.data[2][2] << " " << a.data[2][3] << "\n"
                << a.data[3][0] << " " << a.data[3][1] << " " << a.data[3][2] << " " << a.data[3][3] << "\n";
		}
#endif

		MARS_INLINE friend mat4 operator*(mat4 left, const mat4 right)	// this has to be done painfully slow until i can figure out the proper way to simdize this...
		{
			mat4 result;
#ifdef _WIN32
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
#else
            result.data[0][0] = left.data[0][0] * right.data[0][0] + left.data[0][1] * right.data[1][0] + left.data[0][2] * right.data[2][0] + left.data[0][3] * right.data[3][0];
			result.data[0][1] = left.data[0][0] * right.data[0][1] + left.data[0][1] * right.data[1][1] + left.data[0][2] * right.data[2][1] + left.data[0][3] * right.data[3][1];
			result.data[0][2] = left.data[0][0] * right.data[0][2] + left.data[0][1] * right.data[1][2] + left.data[0][2] * right.data[2][2] + left.data[0][3] * right.data[3][2];
			result.data[0][3] = left.data[0][0] * right.data[0][3] + left.data[0][1] * right.data[1][3] + left.data[0][2] * right.data[2][3] + left.data[0][3] * right.data[3][3];

			result.data[1][0] = left.data[1][0] * right.data[0][0] + left.data[1][1] * right.data[1][0] + left.data[1][2] * right.data[2][0] + left.data[1][3] * right.data[3][0];
			result.data[1][1] = left.data[1][0] * right.data[0][1] + left.data[1][1] * right.data[1][1] + left.data[1][2] * right.data[2][1] + left.data[1][3] * right.data[3][1];
			result.data[1][2] = left.data[1][0] * right.data[0][2] + left.data[1][1] * right.data[1][2] + left.data[1][2] * right.data[2][2] + left.data[1][3] * right.data[3][2];
			result.data[1][3] = left.data[1][0] * right.data[0][3] + left.data[1][1] * right.data[1][3] + left.data[1][2] * right.data[2][3] + left.data[1][3] * right.data[3][3];

			result.data[2][0] = left.data[2][0] * right.data[0][0] + left.data[2][1] * right.data[1][0] + left.data[2][2] * right.data[2][0] + left.data[0][3] * right.data[3][0];
			result.data[2][1] = left.data[2][0] * right.data[0][1] + left.data[2][1] * right.data[1][1] + left.data[2][2] * right.data[2][1] + left.data[0][3] * right.data[3][1];
			result.data[2][2] = left.data[2][0] * right.data[0][2] + left.data[2][1] * right.data[1][2] + left.data[2][2] * right.data[2][2] + left.data[0][3] * right.data[3][2];
			result.data[2][3] = left.data[2][0] * right.data[0][3] + left.data[2][1] * right.data[1][3] + left.data[2][2] * right.data[2][3] + left.data[0][3] * right.data[3][3];

			result.data[3][0] = left.data[3][0] * right.data[0][0] + left.data[3][1] * right.data[1][0] + left.data[3][2] * right.data[2][0] + left.data[3][3] * right.data[3][0];
			result.data[3][1] = left.data[3][0] * right.data[0][1] + left.data[3][1] * right.data[1][1] + left.data[3][2] * right.data[2][1] + left.data[3][3] * right.data[3][1];
			result.data[3][2] = left.data[3][0] * right.data[0][2] + left.data[3][1] * right.data[1][2] + left.data[3][2] * right.data[2][2] + left.data[3][3] * right.data[3][2];
			result.data[3][3] = left.data[3][0] * right.data[0][3] + left.data[3][1] * right.data[1][3] + left.data[3][2] * right.data[2][3] + left.data[3][3] * right.data[3][3];
#endif

			return result;
		}

		MARS_INLINE friend mat4 operator*=(mat4& a, const mat4 b)
		{
			a = a * b;
			return a;
		}

		MARS_INLINE const f32* GetData()
		{
			f32 res[16];

			res[0] = this->data[0].m128_f32[0];
			res[1] = this->data[0].m128_f32[1];
			res[2] = this->data[0].m128_f32[2];
			res[3] = this->data[0].m128_f32[3];
			res[4] = this->data[1].m128_f32[0];
			res[5] = this->data[1].m128_f32[1];
			res[6] = this->data[1].m128_f32[2];
			res[7] = this->data[1].m128_f32[3];
			res[8] = this->data[2].m128_f32[0];
			res[9] = this->data[2].m128_f32[1];
			res[10] = this->data[2].m128_f32[2];
			res[11] = this->data[2].m128_f32[3];
			res[12] = this->data[3].m128_f32[0];
			res[13] = this->data[3].m128_f32[1];
			res[14] = this->data[3].m128_f32[2];
			res[15] = this->data[3].m128_f32[3];

			return res;
		}

	private:
		__m128 data[4];

		MARS_INLINE explicit mat4(__m128 a, __m128 b, __m128 c, __m128 d)
		{
			data[0] = a;
			data[1] = b;
			data[2] = c;
			data[3] = d;
		}
	};
}
