#pragma once

#include "vector2.h"
#include "vector3.h"

namespace h5
{
	struct vertex
	{
		vector3	position;		
		vector3 normal;
		vector2	tex_coord;

		__forceinline vertex() {}

		__forceinline vertex(const vector3 &position, const vector3 &normal = vector3(), const vector2 &tex_coord = vector2())
			: position(position)			
			, normal(normal)
			, tex_coord(tex_coord)
		{}

		__forceinline ~vertex() {}
	};
}
