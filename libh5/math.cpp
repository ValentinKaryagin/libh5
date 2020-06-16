#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "quat.h"
#include "matrix.h"

namespace h5
{
	const vector2 vector2::zero		= vector2();
	const vector2 vector2::right	= vector2(1.0f, 0.0);
	const vector2 vector2::down		= vector2(0.0f, 1.0);

	const vector3 vector3::zero		= vector3();
	const vector3 vector3::forward	= vector3(1.0f, 0.0f, 0.0f);
	const vector3 vector3::right	= vector3(0.0f, 1.0f, 0.0f);
	const vector3 vector3::up		= vector3(0.0f, 0.0f, 1.0f);
	const vector3 vector3::identity = vector3(1.0f, 1.0f, 1.0f);

	const vector4 vector4::zero		= vector4();

	const quat quat::identity		= quat();

	const matrix matrix::identity	= matrix();
}
