#include "server.h"

server::server()
	: iocp_server()
{}

server::~server() {}

void server::add_to_list(h5::overlapped_socket *socket)
{
	h5::lock_guard guard(&list_mtx);
	list.push_back(socket);
}

void server::remove_from_list(h5::overlapped_socket *socket)
{
	h5::lock_guard guard(&list_mtx);
	list.remove(socket);
}

void server::on_accepted(h5::overlapped_socket *socket)
{
	DEBUG_PRINT_FUNCFILELINE();
}

void server::on_recv(h5::overlapped_socket *socket, size_t bytes_received)
{
	DEBUG_PRINT_FUNCFILELINE();

	send_pending(socket, bytes_received);
}

void server::on_send(h5::overlapped_socket *socket, size_t bytes_sended)
{
	DEBUG_PRINT_FUNCFILELINE();

	recv_pending(socket);
}

void server::on_closed(h5::overlapped_socket *socket)
{
	DEBUG_PRINT_FUNCFILELINE();
}
