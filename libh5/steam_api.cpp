#include "steam_api.h"

#include "..\third_party\steamworks\public\steam\steam_api.h"

namespace h5
{
	steam_api::steam_api() {}

	steam_api::~steam_api()
	{
		terminate();
	}

	int32_t steam_api::init()
	{
		if (!SteamAPI_Init())
		{
			return ERRLVL_STEAM_API_INIT;
		}	

		return 0;
	}

	void steam_api::terminate()
	{
		SteamAPI_Shutdown();
	}

	const std::string steam_api::get_username() const
	{
		return SteamFriends()->GetPersonaName();
	}

	steam_api *steam_api::get_instance()
	{
		static steam_api steam_api_instance;
		return &steam_api_instance;
	}
}
