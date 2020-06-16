#pragma once

#include "commons.h"

#ifndef LIBH5
#pragma comment(lib, "..\\..\\third_party\\steamworks\\redistributable_bin\\win64\\steam_api64.lib")
#endif

namespace h5
{
	/**
		steam api wrapp class
		docs: https://partner.steamgames.com/doc/sdk/api
	*/
	class steam_api
	{
	protected:
		steam_api();

	public:
		virtual ~steam_api();

	public:
		int32_t init();
		void terminate();
		
	public:
		const std::string get_username() const;

	public:
		static steam_api *get_instance();
	};
}
