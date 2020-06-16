#pragma once

#include "..\..\libh5\h5.h"

#include <list>

class server
	: public h5::iocp_server
{
protected:
	h5::mutex list_mtx;
	std::list<h5::overlapped_socket *> list;

public:
	server();
	virtual ~server();

protected:
	void add_to_list(h5::overlapped_socket *socket);
	void remove_from_list(h5::overlapped_socket *socket);

protected:
	void on_accepted(h5::overlapped_socket *socket) override;
	void on_recv(h5::overlapped_socket *socket, size_t bytes_received) override;
	void on_send(h5::overlapped_socket *socket, size_t bytes_sended) override;
	void on_closed(h5::overlapped_socket *socket) override;
};
