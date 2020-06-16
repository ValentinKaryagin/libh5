#include "suzanne_mesh.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(suzanne_mesh);

	suzanne_mesh::suzanne_mesh()
		: static_mesh(u(".\\suzanne.stm"))
	{}

	suzanne_mesh::~suzanne_mesh() {}
}
