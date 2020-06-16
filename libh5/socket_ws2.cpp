#include "socket_ws2.h"

#include <WS2tcpip.h>
#include <Windows.h>

namespace h5
{
	int64_t socket_ws2::refs = 0;

	void socket_ws2::inc_ref()
	{
		if (InterlockedIncrement64(&socket_ws2::refs) == 1)
		{
			WSADATA wsa_data{};
			if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
			{
				DEBUG_PRINT_FILELINERES(WSAGetLastError());
			}
		}
	}

	void socket_ws2::dec_ref()
	{
		if (!InterlockedDecrement64(&socket_ws2::refs))
		{
			if (WSACleanup() != 0)
			{
				DEBUG_PRINT_FILELINERES(WSAGetLastError());
			}
		}
	}

	socket_ws2::socket_ws2()
	{
		socket_ws2::inc_ref();
	}

	socket_ws2::~socket_ws2()
	{
		terminate();
		socket_ws2::dec_ref();
	}

	int32_t socket_ws2::init(ip_protocol protocol)
	{
		win32_socket = ::socket(
			AF_INET,
			protocol == ip_protocol::tcp ? SOCK_STREAM : SOCK_DGRAM,
			protocol == ip_protocol::tcp ? static_cast<int>(IPPROTO_TCP) : static_cast<int>(IPPROTO_UDP)
		);

		if (win32_socket == INVALID_SOCKET)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CREATE_SOCKET;
		}
		
		return 0;
	}

	int32_t socket_ws2::init(const socket_ws2 &other)
	{
		win32_socket  = other.win32_socket;
		if (win32_socket == INVALID_SOCKET)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CREATE_SOCKET;
		}

		return 0;
	}

	int32_t socket_ws2::init(unsigned long long other_win32_socket)
	{
		win32_socket = other_win32_socket;
		if (win32_socket == INVALID_SOCKET)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CREATE_SOCKET;
		}

		return 0;
	}

	void socket_ws2::terminate()
	{
		if (win32_socket != INVALID_SOCKET)
		{
			if (closesocket(win32_socket) == SOCKET_ERROR)
			{
				DEBUG_PRINT_FILELINERES(WSAGetLastError());
			}

			win32_socket = INVALID_SOCKET;			
		}		
	}

	int32_t socket_ws2::bind(uint16_t port, const std::wstring &ip_address) const
	{
		SOCKADDR_IN bind_addr{};
		bind_addr.sin_family = AF_INET;
		bind_addr.sin_port = htons(port);
		if (!InetPtonW(AF_INET, ip_address.c_str(), &bind_addr.sin_addr))
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CONVERT_IPADDR_TO_NET;
		}

		if (::bind(win32_socket, (const SOCKADDR *)&bind_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_BIND_ADDR;
		}

		return 0;
	}

	int32_t socket_ws2::listen() const
	{
		if (::listen(win32_socket, SOMAXCONN) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_LISTEN;
		}

		return 0;
	}

	int32_t socket_ws2::accept(socket_ws2 *out_socket, uint16_t *out_port, std::wstring *out_ip_address)
	{
		SOCKADDR_IN accept_addr{};
		int accept_addr_size = sizeof(SOCKADDR_IN);
		out_socket->win32_socket = ::accept(win32_socket, (SOCKADDR *)&accept_addr, &accept_addr_size);
		if (out_socket->win32_socket == INVALID_SOCKET) return ERRLVL_WS2_ACCEPT;
		*out_port = ntohs(accept_addr.sin_port);
		out_ip_address->resize(INET_ADDRSTRLEN);
		if (!InetNtopW(AF_INET, &accept_addr.sin_addr, (wchar_t *)out_ip_address->data(), INET_ADDRSTRLEN))
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CONVERT_IPADDR_TO_TXT;
		}
				
		return 0;
	}

	int32_t socket_ws2::recv(uint8_t *buffer, size_t buffer_size, size_t *out_bytes_received)
	{
		int res = ::recv(win32_socket, reinterpret_cast<char *>(buffer), static_cast<int>(buffer_size), 0);
		if (res == SOCKET_ERROR || res == 0)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_RECV;
		}
		*out_bytes_received = (size_t)res;

		return 0;
	}

	int32_t socket_ws2::send(const uint8_t *data, size_t data_size, size_t *out_bytes_sended)
	{
		int res = ::send(win32_socket, reinterpret_cast<const char *>(data), static_cast<int>(data_size), 0);
		if (res == SOCKET_ERROR || res == 0)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_SEND;
		}
		*out_bytes_sended = (size_t)res;

		return 0;
	}

	int32_t socket_ws2::connect(uint16_t port, const std::wstring &ip_address) const
	{
		SOCKADDR_IN connect_addr{};
		connect_addr.sin_family = AF_INET;
		connect_addr.sin_port = htons(port);
		if (!InetPtonW(AF_INET, ip_address.c_str(), &connect_addr.sin_addr))
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CONVERT_IPADDR_TO_NET;
		}

		if (::connect(win32_socket, (const SOCKADDR *)&connect_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CONNECT;
		}

		return 0;
	}

	int32_t socket_ws2::disconnect()
	{
		if (shutdown(win32_socket, SD_BOTH) == SOCKET_ERROR)
		{
			int res = WSAGetLastError();
			if (res != WSAENOTCONN)
			{
				DEBUG_PRINT_FILELINERES(WSAGetLastError());
				return ERRLVL_WS2_SHUTDOWN_SOCKET;
			}
		}

		if (closesocket(win32_socket) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CLOSE_SOCKET;
		}

		win32_socket = INVALID_SOCKET;

		return 0;
	}

	int32_t socket_ws2::close()
	{
		if (closesocket(win32_socket) == SOCKET_ERROR)
		{
			DEBUG_PRINT_FILELINERES(WSAGetLastError());
			return ERRLVL_WS2_CLOSE_SOCKET;
		}
		
		return 0;
	}

	bool socket_ws2::is_valid() const
	{
		return win32_socket != INVALID_SOCKET;
	}

	SOCKET socket_ws2::get_win32_socket() const
	{
		return win32_socket;
	}
}
