#pragma once

#include "math.h"

namespace h5
{
	class frustum_culling
	{
	protected:
		enum planes
			: size_t
		{
			near,
			far,
			left,
			right,
			bottom,
			top,			
			max
		};

	protected:
		vector4 planes[planes::max]{};

	public:
		frustum_culling(const matrix &vp);
		virtual ~frustum_culling();

	public:
		bool check_sphere(const vector3 &location, float radius);
	};
}
