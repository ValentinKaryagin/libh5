#include "plane_mesh.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(plane_mesh);

	plane_mesh::plane_mesh()
		: static_mesh(u(".\\plane.stm"))
	{}

	plane_mesh::~plane_mesh() {}
}
