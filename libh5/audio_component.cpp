#include "audio_component.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(audio_component);

	audio_component::audio_component()
		: transform_component()
	{}

	audio_component::~audio_component() {}
}
