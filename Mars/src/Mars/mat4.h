#pragma once
#include "Core.h"
#include <xmmintrin.h>


namespace Mars
{
	class mat4
	{
	public:
		__forceinline mat4() {}
		__forceinline explicit mat4(f32 val) {}
	private:
		__m128 data[4];
	};
}
