#include "object.h"

#include "game_engine.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(object);

	object::object()
		: id(guid(e_init::force))
	{}

	object::~object() {}
	
	const guid *object::get_guid() const
	{
		return &id;
	}
}
