#include "cube_mesh.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(cube_mesh);

	cube_mesh::cube_mesh()
		: static_mesh(u(".\\cube.stm"))
	{}

	cube_mesh::~cube_mesh() {}
}
