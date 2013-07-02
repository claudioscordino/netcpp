/*
 * tcp_ipv4.hpp
 *
 * Copyright (C) 2013 Evidence Srl - www.evidence.eu.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef TCP_IPV4_HPP_
#define TCP_IPV4_HPP_

#include <stdexcept>
#include "abstract_socket.hpp"
#include "posix_socket.hpp"

namespace net {
namespace ip4 {
namespace tcp {

class server: public AbstractSocket {
public:
	void accept (AbstractSystemSocket* sock){
		AbstractSocket<N>::socket_->accept(sock);
	}
	void bind (Address* addr){
		AbstractSocket::socket_->bind(addr);
	}
	void listen (int max_pending_connections){
		AbstractSocket::socket_->listen(max_pending_connections);
	}
	server():
	    AbstractSocket(new PosixSocket(protocol(protocol::TCP_IPv4))){}
};



class client: public AbstractSocket {
public:
	void connect (Address* addr){
		AbstractSocket::socket_->connect(addr);
	}
	client():
	    AbstractSocket(new PosixSocket(protocol(protocol::TCP_IPv4))){}
};



}}}

#endif // TCP_IPV4_HPP_
