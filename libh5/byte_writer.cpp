#include "byte_writer.h"

namespace h5
{
	byte_writer::byte_writer(uint8_t *data)
		: data(data)
	{}

	byte_writer::~byte_writer() {}


	void byte_writer::write_int8(int8_t value)
	{
		data[w_carriage++] = (value & 0xff);
	}

	void byte_writer::write_uint8(uint8_t value)
	{
		data[w_carriage++] = (value & 0xff);
	}

	void byte_writer::write_int16(int16_t value)
	{
		data[w_carriage++] = (value & 0xff);
		data[w_carriage++] = (value >> 8 & 0xff);
	}

	void byte_writer::write_uint16(uint16_t value)
	{
		data[w_carriage++] = (value & 0xff);
		data[w_carriage++] = (value >> 8 & 0xff);
	}

	void byte_writer::write_int32(int32_t value)
	{
		data[w_carriage++] = (value & 0xff);
		data[w_carriage++] = (value >> 8 & 0xff);
		data[w_carriage++] = (value >> 16 & 0xff);
		data[w_carriage++] = (value >> 24 & 0xff);
	}

	void byte_writer::write_uint32(uint32_t value)
	{
		data[w_carriage++] = (value & 0xff);
		data[w_carriage++] = (value >> 8 & 0xff);
		data[w_carriage++] = (value >> 16 & 0xff);
		data[w_carriage++] = (value >> 24 & 0xff);
	}

	void byte_writer::write_float(float value)
	{
		int32_t converted = *(int32_t *)&value;
		data[w_carriage++] = (converted & 0xff);
		data[w_carriage++] = (converted >> 8 & 0xff);
		data[w_carriage++] = (converted >> 16 & 0xff);
		data[w_carriage++] = (converted >> 24 & 0xff);
	}

	void byte_writer::write_double(double value)
	{
		int64_t converted = *(int64_t *)&value;
		data[w_carriage++] = (converted & 0xff);
		data[w_carriage++] = (converted >> 8 & 0xff);
		data[w_carriage++] = (converted >> 16 & 0xff);
		data[w_carriage++] = (converted >> 24 & 0xff);
		data[w_carriage++] = (converted >> 32 & 0xff);
		data[w_carriage++] = (converted >> 40 & 0xff);
		data[w_carriage++] = (converted >> 48 & 0xff);
		data[w_carriage++] = (converted >> 56 & 0xff);
	}

	void byte_writer::write_int64(int64_t value)
	{
		data[w_carriage++] = (value & 0xff);
		data[w_carriage++] = (value >> 8 & 0xff);
		data[w_carriage++] = (value >> 16 & 0xff);
		data[w_carriage++] = (value >> 24 & 0xff);
		data[w_carriage++] = (value >> 32 & 0xff);
		data[w_carriage++] = (value >> 40 & 0xff);
		data[w_carriage++] = (value >> 48 & 0xff);
		data[w_carriage++] = (value >> 56 & 0xff);
	}

	void byte_writer::write_uint64(uint64_t value)
	{
		data[w_carriage++] = (value & 0xff);
		data[w_carriage++] = (value >> 8 & 0xff);
		data[w_carriage++] = (value >> 16 & 0xff);
		data[w_carriage++] = (value >> 24 & 0xff);
		data[w_carriage++] = (value >> 32 & 0xff);
		data[w_carriage++] = (value >> 40 & 0xff);
		data[w_carriage++] = (value >> 48 & 0xff);
		data[w_carriage++] = (value >> 56 & 0xff);
	}

	void byte_writer::reset_carriage()
	{
		w_carriage = 0;
	}
}
