#pragma once

#include "static_mesh.h"

namespace h5
{
	class plane_mesh
		: public static_mesh
	{
	public:
		plane_mesh();
		virtual ~plane_mesh();
	};
}
