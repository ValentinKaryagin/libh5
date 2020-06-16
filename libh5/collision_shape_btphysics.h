#pragma once

#include "btphysics_public.h"

namespace h5
{
	class collision_shape_btphysics
	{
	protected:
		btTriangleMesh *mesh = nullptr;
		btCollisionShape *shape = nullptr;

	public:


	public:
		collision_shape_btphysics();
		virtual ~collision_shape_btphysics();

	public:

	};	
}
