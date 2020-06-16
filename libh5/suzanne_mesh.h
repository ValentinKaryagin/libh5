#pragma once

#include "static_mesh.h"

namespace h5
{
	class suzanne_mesh
		: public static_mesh
	{
	public:
		suzanne_mesh();
		virtual ~suzanne_mesh();
	};
}
