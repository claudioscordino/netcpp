/*
 * udp_ipv4.hpp
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

#ifndef UDP_IPV4_HPP_
#define UDP_IPV4_HPP_

#include <stdexcept>
#include "abstract_socket.hpp"
#include "posix_socket.hpp"
#include "address.hpp"
#include "tcp_ip4.hpp"



namespace net {
namespace ip4 {
namespace udp {

typedef net::ip4::tcp::address address;


class server: public AbstractSocket {
public:
	inline void bind (Address* addr){
		AbstractSocket::socket_->bind(addr);
	}
	server():
	    AbstractSocket(new PosixSocket(protocol(protocol::UDP_IPv4))){}

	inline void open (Address* addr) {
		bind(addr);
	}
};



class client: public AbstractSocket {
public:
	void connect (Address* addr){
		AbstractSocket::socket_->connect(addr);
	}
	client():
	    AbstractSocket(new PosixSocket(protocol(protocol::UDP_IPv4))){}

	inline void open (Address* addr) {
		connect (addr);
	}

};



}}}

#endif // UDP_IPV4_HPP_
