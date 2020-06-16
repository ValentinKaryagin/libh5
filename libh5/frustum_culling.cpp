#include "frustum_culling.h"

namespace h5
{
	frustum_culling::frustum_culling(const matrix &vp)
	{
		planes[near]	= vector4(-1.0f, 0.0f, 0.0f, 1.0f);
		planes[far]		= vector4(1.0f, 0.0f, 0.0f, 1.0f);
		planes[left]	= vector4(0.0f, -1.0f, 0.0f, 1.0f);
		planes[right]	= vector4(1.0f, 1.0f, 0.0f, 1.0f);
		planes[bottom]	= vector4(0.0f, 0.0f, -1.0f, 1.0f);
		planes[top]		= vector4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	frustum_culling::~frustum_culling() {}

	bool check_sphere(const vector3 &location, float radius)
	{
		return true;
	}
}
