#pragma once

#include "iocp.h"

#include "socket_ws2.h"

#define WIN32_OVERLAPPED_SIZE	32
#define WIN32_WSABUF_SIZE		16

#define IOCP_BUFFER_SIZE		8196

namespace h5
{
	class overlapped_socket
	{
	public:
		friend class iocp_server;

	private:
		unsigned long long		socket;
		char					recv_overlapped[WIN32_OVERLAPPED_SIZE]{}, send_overlapped[WIN32_OVERLAPPED_SIZE]{};
		char					raw_recv_buff[IOCP_BUFFER_SIZE]{}, raw_send_buff[IOCP_BUFFER_SIZE]{};
		char					recv_buff[WIN32_WSABUF_SIZE]{}, send_buff[WIN32_WSABUF_SIZE]{};
		volatile unsigned int	recv_pending_flag = 0, send_pending_flag = 0;

	public:
		overlapped_socket(unsigned long long socket = ~0);
		virtual ~overlapped_socket();

	public:
		uint8_t *get_recv_buffer();
		uint8_t *get_send_buffer();
	};

	class iocp_server
		: public iocp
	{
	protected:
		unsigned long long listener = ~0;
		unsigned long long acceptor = ~0;
		uint8_t acceptor_buffer[IOCP_BUFFER_SIZE]{};
		char accept[WIN32_OVERLAPPED_SIZE]{};
		unsigned long bytes_received_on_accept = 0;

	public:
		iocp_server();
		virtual ~iocp_server();

	public:
		iocp_server(const iocp_server &other) = delete;
		iocp_server(iocp_server &&other) = delete;

	public:
		int32_t init(uint16_t port, const std::string &ip_address, size_t threads_count = 0);
		void terminate();

	protected:
		int32_t recv_pending(overlapped_socket *socket, size_t buffer_size = IOCP_BUFFER_SIZE);
		int32_t send_pending(overlapped_socket *socket, size_t buffer_size = IOCP_BUFFER_SIZE);

	protected:
		virtual void on_accepted(overlapped_socket *socket) = 0;
		virtual void on_recv(overlapped_socket *socket, size_t bytes_received) = 0;
		virtual void on_send(overlapped_socket *socket, size_t bytes_sended) = 0;
		virtual void on_closed(overlapped_socket *socket) = 0;

	private:
		void on_io(bool status, void *key, _OVERLAPPED *overlapped, size_t bytes_transferred) override;
	};
}
