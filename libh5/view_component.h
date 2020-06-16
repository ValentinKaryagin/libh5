#pragma once

#include "transform_component.h"

namespace h5
{
	class view_component
		: public transform_component
	{
	public:
		static const float FOV_DEFAULT;

	protected:
		float fov = FOV_DEFAULT;

	public:
		view_component();
		virtual ~view_component();

	public:
		matrix get_view_matrix();

	public:
		float get_fov();
	};
}
