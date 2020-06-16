#pragma once

#include "openssl_public.h"

namespace h5
{
	class rsa_openssl
	{
	protected:
		rsa_st		*rsa	= nullptr;
		bignum_st	*bn		= nullptr;

	public:
		rsa_openssl();
		virtual ~rsa_openssl();

	public:
		int32_t init();
		void terminate();

	public:
		int32_t get_private_key(std::string *out);
		int32_t get_public_key(std::string *out);
	};
}
