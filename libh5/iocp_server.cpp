#include "iocp_server.h"

#include <WS2tcpip.h>
#include <MSWSock.h>

#include <ctime>

namespace h5
{
	overlapped_socket::overlapped_socket(unsigned long long socket)
		: socket(socket)
	{
		WSABUF *wsa_recv_buff = (WSABUF *)recv_buff;
		wsa_recv_buff->len = IOCP_BUFFER_SIZE;
		wsa_recv_buff->buf = raw_recv_buff;
		
		WSABUF *wsa_send_buff = (WSABUF *)send_buff;
		wsa_send_buff->len = IOCP_BUFFER_SIZE;
		wsa_send_buff->buf = raw_send_buff;
	}

	overlapped_socket::~overlapped_socket() {}

	uint8_t *overlapped_socket::get_recv_buffer()
	{
		return (uint8_t *)raw_recv_buff;
	}

	uint8_t *overlapped_socket::get_send_buffer()
	{
		return (uint8_t *)raw_send_buff;
	}

	iocp_server::iocp_server()
		: iocp()
	{
		socket_ws2::inc_ref();
	}

	iocp_server::~iocp_server()
	{
		terminate();
		socket_ws2::dec_ref();
	}	

	int32_t iocp_server::init(uint16_t port, const std::string &ip_address, size_t threads_count)
	{
		int32_t error_level = 0;

		if (!error_level)
		{
			error_level = iocp::init(threads_count);
		}
		
		if (error_level)
		{
			DEBUG_PRINT_FILELINERES(error_level);
			return error_level;
		}		

		listener = ::WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if (listener == INVALID_SOCKET)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CREATE_SOCKET;
		}

		SOCKADDR_IN bind_addr{};
		bind_addr.sin_family = AF_INET;
		bind_addr.sin_port = htons(port);
		if (!inet_pton(AF_INET, ip_address.c_str(), &bind_addr.sin_addr))
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CONVERT_IPADDR_TO_NET;
		}

		if (::bind(listener, (const SOCKADDR *)&bind_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_BIND_ADDR;
		}

		if (::listen(listener, SOMAXCONN) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_LISTEN;
		}

		acceptor = ::WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if (acceptor == INVALID_SOCKET)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CREATE_SOCKET;
		}

		if (!AcceptEx(
			listener,
			acceptor,
			acceptor_buffer,
			0,
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&bytes_received_on_accept,
			(OVERLAPPED *)accept))
		{
			int res = WSAGetLastError();
			if (res != ERROR_IO_PENDING)
			{
				DEBUG_PRINT_FILELINERES(res);
				return ERRLVL_WS2_ACCEPT;
			}
		}
		
		error_level = associate((void *)listener, this);
		if (error_level)
		{
			DEBUG_PRINT_FILELINERES(error_level);
			return error_level;
		}	

		return 0;
	}

	void iocp_server::terminate()
	{
		if (closesocket(acceptor) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
		}

		if (closesocket(listener) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return;
		}
	}

	int32_t iocp_server::recv_pending(overlapped_socket *socket, size_t buffer_size)
	{
		DWORD flags = 0;
		DWORD received = 0;
		((WSABUF *)socket->recv_buff)->len = (ULONG)buffer_size;
		if (WSARecv(socket->socket, (WSABUF *)socket->recv_buff, 1, &received, &flags, (OVERLAPPED *)socket->recv_overlapped, nullptr) == SOCKET_ERROR)
		{
			int res = WSAGetLastError();
			if (res != ERROR_IO_PENDING && res != WSAECONNRESET)
			{
				DEBUG_PRINT_FILELINERES(res);
				return ERRLVL_WS2_RECV;
			}
		}

		_InterlockedIncrement(&socket->recv_pending_flag);

		return 0;
	}

	int32_t iocp_server::send_pending(overlapped_socket *socket, size_t buffer_size)
	{
		DWORD flags = 0;
		DWORD sended = 0;
		((WSABUF *)socket->recv_buff)->len = (ULONG)buffer_size;
		if (WSASend(socket->socket, (WSABUF *)socket->send_buff, 1, &sended, flags, (OVERLAPPED *)socket->send_overlapped, nullptr) == SOCKET_ERROR)
		{
			int res = WSAGetLastError();
			if (res != ERROR_IO_PENDING && res != WSAECONNRESET)
			{
				DEBUG_PRINT_FILELINERES(res);
				return ERRLVL_WS2_SEND;
			}
		}

		_InterlockedIncrement(&socket->send_pending_flag);

		return 0;
	}

	void iocp_server::on_io(bool status, void *key, _OVERLAPPED *overlapped, size_t bytes_transferred)
	{
		if (status)
		{
			if (key)
			{
				overlapped_socket *socket = (overlapped_socket *)key;

				if (bytes_transferred)
				{
					if (overlapped == (OVERLAPPED *)socket->recv_overlapped)
					{
						on_recv((overlapped_socket *)key, bytes_transferred);						

						_InterlockedDecrement(&socket->recv_pending_flag);
						return;
					}
					else if (overlapped == (OVERLAPPED *)socket->send_overlapped)
					{
						on_send((overlapped_socket *)key, bytes_transferred);

						_InterlockedDecrement(&socket->send_pending_flag);
						return;
					}
					else
					{
						DEBUG_PRINTF("Unexpected\n");
					}
				}
				else
				{
					if (overlapped == (OVERLAPPED *)socket->recv_overlapped ||
						overlapped == (OVERLAPPED *)socket->send_overlapped)
					{
						if (!_InterlockedExchange(&socket->recv_pending_flag, socket->recv_pending_flag) &&
							!_InterlockedExchange(&socket->send_pending_flag, socket->send_pending_flag))
						{
							SOCKET old_handle = _InterlockedExchange(&socket->socket, INVALID_SOCKET);
							if (old_handle != INVALID_SOCKET)
							{
								if (shutdown(old_handle, SD_BOTH) == SOCKET_ERROR)
								{
									int res = WSAGetLastError();
									if (res != WSAENOTCONN)
									{
										DEBUG_PRINT_FILELINERES(res);
										return;
									}
								}

								if (closesocket(old_handle) == SOCKET_ERROR)
								{
									int res = WSAGetLastError();
									if (res != WSAENOTCONN)
									{
										DEBUG_PRINT_FILELINERES(res);
										return;
									}
								}

								on_closed(socket);

								delete socket;

								return;
							}
						}						
					}
					else if (overlapped == (OVERLAPPED *)accept)
					{
						SOCKET accepted = acceptor;

						acceptor = ::WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
						if (acceptor == INVALID_SOCKET)
						{
							DEBUG_PRINT_FILELINERES(WSAGetLastError());
							return;
						}

						if (!AcceptEx(
							listener,
							acceptor,
							acceptor_buffer,
							0,
							sizeof(SOCKADDR_IN) + 16,
							sizeof(SOCKADDR_IN) + 16,
							&bytes_received_on_accept,
							(OVERLAPPED *)accept))
						{
							int res = WSAGetLastError();
							if (res != ERROR_IO_PENDING)
							{
								DEBUG_PRINT_FILELINERES(res);
								return;
							}
						}

						overlapped_socket *ov_socket = new overlapped_socket(accepted);

						on_accepted(ov_socket);

						int32_t error_level = 0;

						if (!error_level)
						{
							error_level = associate((void *)accepted, ov_socket);
						}

						if (error_level)
						{
							DEBUG_PRINT_FILELINERES(error_level);
							return;
						}

						error_level = recv_pending(ov_socket);
						if (error_level)
						{
							DEBUG_PRINT_FILELINERES(error_level);
							return;
						}

						return;
					}
					else
					{
						DEBUG_PRINTF("Unexpected\n");
					}
				}				
			}			
		}
		else
		{
			if (key)
			{
				overlapped_socket *socket = (overlapped_socket *)key;				

				if (overlapped == (OVERLAPPED *)socket->recv_overlapped ||
					overlapped == (OVERLAPPED *)socket->send_overlapped)
				{
					if (socket->recv_pending_flag)
					{
						_InterlockedDecrement(&socket->recv_pending_flag);
					}
					else if (socket->send_pending_flag)
					{
						_InterlockedDecrement(&socket->send_pending_flag);
					}

					if (!_InterlockedExchange(&socket->recv_pending_flag, socket->recv_pending_flag) &&
						!_InterlockedExchange(&socket->send_pending_flag, socket->send_pending_flag))
					{
						SOCKET old_handle = _InterlockedExchange(&socket->socket, INVALID_SOCKET);
						if (old_handle != INVALID_SOCKET)
						{
							if (shutdown(old_handle, SD_BOTH) == SOCKET_ERROR)
							{
								int res = WSAGetLastError();
								if (res != WSAENOTCONN)
								{
									DEBUG_PRINT_FILELINERES(res);
									return;
								}
							}

							if (closesocket(old_handle) == SOCKET_ERROR)
							{
								int res = WSAGetLastError();
								if (res != WSAENOTCONN)
								{
									DEBUG_PRINT_FILELINERES(res);
									return;
								}
							}

							on_closed(socket);

							delete socket;

							return;
						}
					}					
				}
			}
		}
	}
}
