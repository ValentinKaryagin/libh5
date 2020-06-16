#include "plane200x200_mesh.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(plane200x200_mesh);

	plane200x200_mesh::plane200x200_mesh()
		: static_mesh(u(".\\plane200x200.stm"))
	{}

	plane200x200_mesh::~plane200x200_mesh() {}
}
