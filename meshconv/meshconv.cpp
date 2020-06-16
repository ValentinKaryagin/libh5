#include "..\libh5\static_mesh.h"

int wmain(int argc, wchar_t **argv)
{
	h5::static_mesh mesh(argv[1]);
	mesh.read_wavefront();

	mesh.set_path(argv[2]);

	mesh.write_native();

	//h5::static_mesh mesh(u("D:\\Development\\Master\\highfivesoft\\libh5\\build\\win64\\Debug\\projects\\cosmo\\plane.stm"));
	//mesh.read_native();

	return 0;
}
