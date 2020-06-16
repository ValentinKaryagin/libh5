#pragma once

#include "commons.h"

namespace h5
{
	class stream_vector
	{
	protected:
		size_t w_carriage = 0, r_carriage = 0;
		std::vector<uint8_t> data;

	public:
		stream_vector();
		stream_vector(unsigned char *arr, size_t arr_size);
		stream_vector(const std::vector<uint8_t> &vec);
		virtual ~stream_vector();

	public:
		void write_int8(int8_t value);
		void write_uint8(uint8_t value);
		void write_int16(int16_t value);
		void write_uint16(uint16_t value);
		void write_int32(int32_t value);
		void write_uint32(uint32_t value);
		void write_float(float value);
		void write_double(double value);
		void write_int64(int64_t value);
		void write_uint64(uint64_t value);

	public:
		void write_byte_array(const uint8_t *arr, size_t arr_size);
		void write_string(const char *str);
		void write_wstring(const wchar_t *wstr);

	public:
		void write_byte_array(const std::vector<uint8_t> &vec);
		void write_string(const std::string &str);
		void write_wstring(const std::wstring &wstr);

	public:
		int8_t read_int8();
		uint8_t read_uint8();
		int16_t read_int16();
		uint16_t read_uint16();
		int32_t read_int32();
		uint32_t read_uint32();
		float read_float();
		double read_double();
		int64_t read_int64();
		uint64_t read_uint64();

	public:
		const uint8_t * read_byte_array_directly(size_t size);
		const char * read_string_directly();
		const wchar_t * read_wstring_directly();

	public:
		std::vector<uint8_t> read_byte_array(size_t size);
		std::string read_string();
		std::wstring read_wstring();

	public:
		void reset_write_carriage();
		void reset_read_carriage();

	public:
		void clear();
	};
}
