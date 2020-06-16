#pragma once

#include "physics_world_btphysics.h"
#include "collision_object_btphysics.h"
#include "rigid_body_btphysics.h"

#ifndef LIBH5
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\third_party\\bullet3-2.88\\build\\lib\\Debug\\BulletDynamics_Debug.lib")
#pragma comment(lib, "..\\..\\third_party\\bullet3-2.88\\build\\lib\\Debug\\BulletCollision_Debug.lib")
#pragma comment(lib, "..\\..\\third_party\\bullet3-2.88\\build\\lib\\Debug\\LinearMath_Debug.lib")
#else
#pragma comment(lib, "..\\..\\third_party\\bullet3-2.88\\build\\lib\\Release\\BulletDynamics.lib")
#pragma comment(lib, "..\\..\\third_party\\bullet3-2.88\\build\\lib\\Release\\BulletCollision.lib")
#pragma comment(lib, "..\\..\\third_party\\bullet3-2.88\\build\\lib\\Release\\LinearMath.lib")
#endif
#endif

namespace h5
{
	typedef physics_world_btphysics			physics_world;
	typedef collision_object_btphysics		collision_object;
	typedef rigid_body_btphysics			rigid_body;
}
