#pragma once

#include "..\..\libh5\h5.h"

namespace cosmo
{
	class scene
		: public h5::scene
	{
	protected:
		h5::static_mesh							*plane_mesh		= nullptr;
		std::vector<h5::physics_component *>	plane_physics;
		std::vector<h5::draw_component *>		plane_draws;

	protected:
		h5::static_mesh							*obj_mesh		= nullptr;
		std::vector<h5::physics_component *>	obj_physics;
		std::vector<h5::draw_component *>		obj_draws;

	public:
		scene();
		virtual ~scene();

	public:
		int32_t init() override;
	};
}
