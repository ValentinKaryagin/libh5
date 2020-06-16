#pragma once

#include "rsa_openssl.h"

#ifndef LIBH5
#pragma comment(lib, "..\\..\\third_party\\openssl-OpenSSL_1_1_1d\\libcrypto_static.lib")
#pragma comment(lib, "..\\..\\third_party\\openssl-OpenSSL_1_1_1d\\libssl_static.lib")
#endif

namespace h5
{
	typedef rsa_openssl rsa;
}
