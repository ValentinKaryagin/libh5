#pragma once

#include "commons.h"

namespace h5
{
	class byte_writer
	{
	protected:
		uint8_t *data;
		size_t w_carriage = 0;

	public:
		byte_writer() = delete;
		byte_writer(const byte_writer &other) = delete;
		byte_writer(byte_writer &&other) = delete;

	public:
		byte_writer(uint8_t *data);
		virtual ~byte_writer();

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
		void reset_carriage();
	};
}
