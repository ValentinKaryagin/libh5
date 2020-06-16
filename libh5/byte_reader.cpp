#include "byte_reader.h"

namespace h5
{
	byte_reader::byte_reader(uint8_t *data)
		: data(data)
	{}

	byte_reader::~byte_reader() {}

	int8_t byte_reader::read_int8()
	{
		return data[r_carriage++] & 0xff;
	}

	uint8_t byte_reader::read_uint8()
	{
		return data[r_carriage++] & 0xff;
	}

	int16_t byte_reader::read_int16()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00;
	}

	uint16_t byte_reader::read_uint16()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00;
	}

	int32_t byte_reader::read_int32()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000;
	}

	uint32_t byte_reader::read_uint32()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000;
	}

	float byte_reader::read_float()
	{
		int32_t converted =
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000;
		return *(float *)&converted;
	}

	double byte_reader::read_double()
	{
		int64_t converted =
			(data[r_carriage++] & 0xff |
				data[r_carriage++] << 8 & 0xff00 |
				data[r_carriage++] << 16 & 0xff0000 |
				data[r_carriage++] << 24 & 0xff000000 |
				(int64_t)data[r_carriage++] << 32 & 0xff00000000 |
				(int64_t)data[r_carriage++] << 40 & 0xff0000000000 |
				(int64_t)data[r_carriage++] << 48 & 0xff000000000000 |
				(int64_t)data[r_carriage++] << 56 & 0xff00000000000000);
		return *(double *)&converted;
	}

	int64_t byte_reader::read_int64()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000 |
			(int64_t)data[r_carriage++] << 32 & 0xff00000000 |
			(int64_t)data[r_carriage++] << 40 & 0xff0000000000 |
			(int64_t)data[r_carriage++] << 48 & 0xff000000000000 |
			(int64_t)data[r_carriage++] << 56 & 0xff00000000000000;
	}

	uint64_t byte_reader::read_uint64()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000 |
			(uint64_t)data[r_carriage++] << 32 & 0xff00000000 |
			(uint64_t)data[r_carriage++] << 40 & 0xff0000000000 |
			(uint64_t)data[r_carriage++] << 48 & 0xff000000000000 |
			(uint64_t)data[r_carriage++] << 56 & 0xff00000000000000;
	}

	void byte_reader::reset_carriage()
	{
		r_carriage = 0;
	}
}
