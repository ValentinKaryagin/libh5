#pragma once

#include "commons.h"

namespace h5
{
	class byte_reader
	{
	protected:
		uint8_t *data;
		size_t r_carriage = 0;

	public:
		byte_reader() = delete;
		byte_reader(const byte_reader &other) = delete;
		byte_reader(byte_reader &&other) = delete;

	public:
		byte_reader(uint8_t *data);
		virtual ~byte_reader();

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
		void reset_carriage();
	};
}
