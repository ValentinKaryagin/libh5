#pragma once

#include "windows_public.h"

namespace h5
{
	class socket_ws2
	{
	public:
		enum class ip_protocol
		{
			tcp,
			udp
		};

	public:
		static int64_t refs;

	public:
		static void inc_ref();
		static void dec_ref();

	protected:
		unsigned long long win32_socket = ~0;

	public:
		socket_ws2();
		virtual ~socket_ws2();

	public:
		int32_t init(ip_protocol protocol);
		int32_t init(const socket_ws2 &other);
		int32_t init(unsigned long long other_win32_socket);
		void terminate();

	public:
		int32_t bind(uint16_t port, const std::wstring &ip_address) const;
		int32_t listen() const;

	public:
		int32_t accept(socket_ws2 *out_socket, uint16_t *out_port, std::wstring *out_ip_address);
		int32_t recv(uint8_t *buffer, size_t buffer_size, size_t *out_bytes_received);
		int32_t send(const uint8_t *data, size_t data_size, size_t *out_bytes_sended);

	public:
		int32_t connect(uint16_t port, const std::wstring &ip_address) const;
		int32_t disconnect();
		int32_t close();

	public:
		bool is_valid() const;

	public:
		unsigned long long get_win32_socket() const;
	};
}
