/*
 * dgram_local.hpp
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

#ifndef DGRAM_LOCAL_HPP_
#define DGRAM_LOCAL_HPP_

#include <stdexcept>
#include "abstract_socket.hpp"
#include "posix_socket.hpp"
#include "address.hpp"
#include "stream_local.hpp"



namespace net {
namespace local {
namespace dgram {

typedef net::local::stream::address address;


class server: public AbstractSocket {
public:
	inline void bind (Address* addr){
		AbstractSocket::socket_->bind(addr);
	}
	server():
	    AbstractSocket(new PosixSocket(protocol(protocol::DGRAM_LOCAL))){}

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
	    AbstractSocket(new PosixSocket(protocol(protocol::DGRAM_LOCAL))){}

	inline void open (Address* addr) {
		connect (addr);
	}

};



}}}

#endif // DGRAM_LOCAL_HPP_