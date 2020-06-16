#pragma once

#include "static_mesh.h"

namespace h5
{
	class cube_mesh
		: public static_mesh
	{
	public:
		cube_mesh();
		virtual ~cube_mesh();
	};
}
