#include "stream_vector.h"

namespace h5
{
	stream_vector::stream_vector() {}

	stream_vector::stream_vector(unsigned char *arr, size_t data_size)
	{
		data.resize(data_size);
		std::memcpy(data.data(), arr, data_size);
		w_carriage = data_size;
	}

	stream_vector::stream_vector(const std::vector<uint8_t> &vec)
	{
		data = vec;
		w_carriage = vec.size();
	}

	stream_vector::~stream_vector() {}

	void stream_vector::write_int8(int8_t value)
	{
		data.push_back(value & 0xff);
		w_carriage += sizeof(int8_t);
	}

	void stream_vector::write_uint8(uint8_t value)
	{
		data.push_back(value & 0xff);
		w_carriage += sizeof(uint8_t);
	}

	void stream_vector::write_int16(int16_t value)
	{
		data.push_back(value & 0xff);
		data.push_back(value >> 8 & 0xff);
		w_carriage += sizeof(int16_t);
	}

	void stream_vector::write_uint16(uint16_t value)
	{
		data.push_back(value & 0xff);
		data.push_back(value >> 8 & 0xff);
		w_carriage += sizeof(uint16_t);
	}

	void stream_vector::write_int32(int32_t value)
	{
		data.push_back(value & 0xff);
		data.push_back(value >> 8 & 0xff);
		data.push_back(value >> 16 & 0xff);
		data.push_back(value >> 24 & 0xff);
		w_carriage += sizeof(int32_t);
	}

	void stream_vector::write_uint32(uint32_t value)
	{
		data.push_back(value & 0xff);
		data.push_back(value >> 8 & 0xff);
		data.push_back(value >> 16 & 0xff);
		data.push_back(value >> 24 & 0xff);
		w_carriage += sizeof(uint32_t);
	}

	void stream_vector::write_float(float value)
	{
		int32_t converted = *(int32_t *)&value;
		data.push_back(converted & 0xff);
		data.push_back(converted >> 8 & 0xff);
		data.push_back(converted >> 16 & 0xff);
		data.push_back(converted >> 24 & 0xff);
		w_carriage += sizeof(float);
	}

	void stream_vector::write_double(double value)
	{
		int64_t converted = *(int64_t *)&value;
		data.push_back(converted & 0xff);
		data.push_back(converted >> 8 & 0xff);
		data.push_back(converted >> 16 & 0xff);
		data.push_back(converted >> 24 & 0xff);
		data.push_back(converted >> 32 & 0xff);
		data.push_back(converted >> 40 & 0xff);
		data.push_back(converted >> 48 & 0xff);
		data.push_back(converted >> 56 & 0xff);
		w_carriage += sizeof(double);
	}

	void stream_vector::write_int64(int64_t value)
	{
		data.push_back(value & 0xff);
		data.push_back(value >> 8 & 0xff);
		data.push_back(value >> 16 & 0xff);
		data.push_back(value >> 24 & 0xff);
		data.push_back(value >> 32 & 0xff);
		data.push_back(value >> 40 & 0xff);
		data.push_back(value >> 48 & 0xff);
		data.push_back(value >> 56 & 0xff);
		w_carriage += sizeof(int64_t);
	}

	void stream_vector::write_uint64(uint64_t value)
	{
		data.push_back(value & 0xff);
		data.push_back(value >> 8 & 0xff);
		data.push_back(value >> 16 & 0xff);
		data.push_back(value >> 24 & 0xff);
		data.push_back(value >> 32 & 0xff);
		data.push_back(value >> 40 & 0xff);
		data.push_back(value >> 48 & 0xff);
		data.push_back(value >> 56 & 0xff);
		w_carriage += sizeof(uint64_t);
	}

	void stream_vector::write_byte_array(const uint8_t *arr, size_t arr_size)
	{
		const size_t new_size = w_carriage + arr_size;
		data.resize(new_size);
		std::memcpy(data.data() + w_carriage, arr, arr_size);
		w_carriage = new_size;
	}

	void stream_vector::write_string(const char *str)
	{
		const size_t str_size = strlen(str) + sizeof(char);
		const size_t new_size = w_carriage + str_size;
		data.resize(new_size);
		std::memcpy(data.data() + w_carriage, str, str_size);
		w_carriage += str_size;
	}

	void stream_vector::write_wstring(const wchar_t *wstr)
	{
		const size_t wstr_size = wcslen(wstr) * sizeof(wchar_t) + sizeof(wchar_t);
		const size_t new_size = w_carriage + wstr_size;
		data.resize(new_size);
		std::memcpy(data.data() + w_carriage, wstr, wstr_size);
		w_carriage += wstr_size;
	}

	void stream_vector::write_byte_array(const std::vector<uint8_t> &vec)
	{
		const size_t vec_size = vec.size();
		const size_t new_size = w_carriage + vec_size;
		data.resize(new_size);
		std::memcpy(data.data() + w_carriage, vec.data(), vec_size);
		w_carriage = new_size;
	}

	void stream_vector::write_string(const std::string &str)
	{
		write_string(str.c_str());
	}

	void stream_vector::write_wstring(const std::wstring &wstr)
	{
		write_wstring(wstr.c_str());
	}

	int8_t stream_vector::read_int8()
	{
		return data[r_carriage++] & 0xff;
	}

	uint8_t stream_vector::read_uint8()
	{
		return data[r_carriage++] & 0xff;
	}

	int16_t stream_vector::read_int16()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00;
	}

	uint16_t stream_vector::read_uint16()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00;
	}

	int32_t stream_vector::read_int32()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000;
	}

	uint32_t stream_vector::read_uint32()
	{
		return
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000;
	}

	float stream_vector::read_float()
	{
		int32_t converted =
			data[r_carriage++] & 0xff |
			data[r_carriage++] << 8 & 0xff00 |
			data[r_carriage++] << 16 & 0xff0000 |
			data[r_carriage++] << 24 & 0xff000000;
		return *(float *)&converted;
	}

	double stream_vector::read_double()
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

	int64_t stream_vector::read_int64()
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

	uint64_t stream_vector::read_uint64()
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

	const uint8_t *stream_vector::read_byte_array_directly(size_t size)
	{
		const size_t result_offset = r_carriage;
		r_carriage += size;
		return data.data() + result_offset;
	}

	const char *stream_vector::read_string_directly()
	{
		const size_t result_offset = r_carriage;
		r_carriage += strlen((const char *)data.data() + result_offset);
		return (const char *)data.data() + result_offset;
	}

	const wchar_t *stream_vector::read_wstring_directly()
	{
		const size_t result_offset = r_carriage;
		r_carriage += wcslen((const wchar_t *)(data.data() + result_offset));
		return (const wchar_t *)data.data() + result_offset;
	}

	std::vector<uint8_t> stream_vector::read_byte_array(size_t size)
	{
		std::vector<uint8_t> result;
		result.resize(size);
		std::memcpy(result.data(), data.data() + r_carriage, size);
		r_carriage += size;
		return result;
	}

	std::string stream_vector::read_string()
	{
		std::string result((const char *)data.data() + r_carriage);
		r_carriage += result.length() + sizeof(char);
		return result;
	}

	std::wstring stream_vector::read_wstring()
	{
		std::wstring result((const wchar_t *)(data.data() + r_carriage));
		r_carriage += result.length() * sizeof(wchar_t) + sizeof(wchar_t);
		return result;
	}

	void stream_vector::reset_write_carriage()
	{
		w_carriage = 0;
	}

	void stream_vector::reset_read_carriage()
	{
		r_carriage = 0;
	}

	void stream_vector::clear()
	{
		data.clear();
	}
}
