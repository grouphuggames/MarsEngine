#pragma once
#include "Core.h"


namespace Mars
{
	class vec4
	{
	public:
		vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		vec4(f32 val) : x(val), y(val), z(val), w(val) {}
		vec4(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}

		vec4 operator+(const vec4& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;

			return *this;
		}

		struct
		{
			f32 x, y, z, w;
		};
	};
}
