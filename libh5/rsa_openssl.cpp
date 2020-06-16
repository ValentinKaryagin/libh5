#include "rsa_openssl.h"

#include "openssl_private.h"

namespace h5
{
	rsa_openssl::rsa_openssl() {}

	rsa_openssl::~rsa_openssl()
	{
		terminate();
	}

	int32_t rsa_openssl::init()
	{
		rsa = RSA_new();
		if (!rsa)
		{
			return ERRLVL_OPENSSL_MEMORY_ALLOCATE;
		}

		bn = BN_new();
		if (!bn)
		{
			return ERRLVL_OPENSSL_MEMORY_ALLOCATE;
		}

		if (!BN_set_word(bn, RSA_F4))
		{
			return ERRLVL_OPENSSL_SET_WORD;
		}

		if (!RSA_generate_key_ex(rsa, 1024, bn, nullptr))
		{
			return ERRLVL_OPENSSL_GENERATE_KEY;
		}

		return 0;
	}

	void rsa_openssl::terminate()
	{
		if (bn)
		{
			BN_free(bn);
			bn = nullptr;
		}

		if (rsa)
		{
			RSA_free(rsa);
			rsa = nullptr;
		}
	}

	int32_t rsa_openssl::get_private_key(std::string *out)
	{
		BIO *rsa_private_key = BIO_new(BIO_s_mem());
		if (!rsa_private_key)
		{
			return ERRLVL_OPENSSL_MEMORY_ALLOCATE;
		}

		if (!PEM_write_bio_RSAPrivateKey(rsa_private_key, rsa, nullptr, nullptr, 0, nullptr, nullptr))
		{
			return ERRLVL_OPENSSL_WRITE_KEY;
		}

		out->resize(BIO_pending(rsa_private_key));
		
		if (!BIO_read(rsa_private_key, &out[0], static_cast<int>(out->size())))
		{
			return ERRLVL_OPENSSL_READ;
		}

		BIO_free(rsa_private_key);
		return 0;
	}

	int32_t rsa_openssl::get_public_key(std::string *out)
	{
		BIO *rsa_public_key = BIO_new(BIO_s_mem());
		if (!rsa_public_key)
		{
			return ERRLVL_OPENSSL_MEMORY_ALLOCATE;
		}

		if (!PEM_write_bio_RSAPublicKey(rsa_public_key, rsa))
		{
			return ERRLVL_OPENSSL_WRITE_KEY;
		}

		out->resize(BIO_pending(rsa_public_key));

		if (!BIO_read(rsa_public_key, &out[0], static_cast<int>(out->size())))
		{
			return ERRLVL_OPENSSL_READ;
		}

		BIO_free(rsa_public_key);
		return 0;
	}
}
