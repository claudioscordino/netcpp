/*
 * Copyright (C) 2013 Evidence Srl - www.evidence.eu.com
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef TCP_IPV4_HPP_
#define TCP_IPV4_HPP_

#include <stdexcept>
#include "abstract_socket.hpp"
#include "posix_socket.hpp"
#include "address.hpp"

namespace net {
namespace ip4 {
namespace tcp {

class address: public Address {
public:
	address(const std::string& addr, int port):
		address_{addr}, port_{port}{};
	std::string getAddress() const {
		return address_;
	}
	int getPort(){
		return port_;
	}
private:
	address();
	std::string address_;
	int port_;
};


class server: public AbstractSocket {
public:
	inline void bind (Address* addr){
		AbstractSocket::socket_->bind(addr);
	}
	inline void listen (int max_pending_connections){
		AbstractSocket::socket_->listen(max_pending_connections);
	}
	server():
	    AbstractSocket(new PosixSocket(protocol(protocol::TCP_IPv4))){}

	server(AbstractSocket* srv):
	    AbstractSocket(new PosixSocket(protocol(protocol::TCP_IPv4))){
		socket_->accept((srv->getSocket()));
	}

	inline void open (Address* addr, int max_pending_connections = 100) {
		bind (addr);
		listen(max_pending_connections);
	}
};



class client: public AbstractSocket {
public:
	void connect (Address* addr){
		AbstractSocket::socket_->connect(addr);
	}
	client():
	    AbstractSocket(new PosixSocket(protocol(protocol::TCP_IPv4))){}

	inline void open (Address* addr) {
		connect (addr);
	}

};



}}}

#endif // TCP_IPV4_HPP_
