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

/// Address for local dgram communications.
typedef net::local::stream::address address;

/**
 * @brief Server for local dgram communications.
 */
class server: public AbstractSocket {
public:
	/**
	 * @brief Constructor
	 *
	 * This constructor allocates a concrete class
	 * derived from net::AbstractSystemSocket.
	 */
	server():
	    AbstractSocket{protocol(DGRAM, LOCAL)}{}
	
	/**
	 * @brief Method to bind the server to an address
	 *
	 * This method invokes the platform-specific bind() operation.
	 * @param addr Address (i.e., net::local::dgram::address)
	 * which the server must be bound to
	 */
	inline void bind (const Address& addr){
		AbstractSocket::socket_->bind(addr);
	}

	/**
	 * @brief Method to open the server towards a certain address
	 *
	 * This method calls net::local::dgram::server::bind(),
	 * which in turn invokes the platform-specific bind() operation.
	 * @param addr Address (i.e., net::local::dgram::address) 
	 * that must be open by the server
	 */
	inline void open (const Address& addr) {
		bind(addr);
	}
};



/**
 * @brief Client for local dgram communications.
 */
class client: public AbstractSocket {
public:
	/**
	 * @brief Constructor
	 *
	 * This constructor allocates a concrete class
	 * derived from net::AbstractSystemSocket.
	 */
	client():
	    AbstractSocket{protocol(DGRAM, LOCAL)}{}

	/**
	 * @brief Method to connect the client to an address
	 *
	 * This method invokes the platform-specific connect() operation.
	 * @param addr Address (i.e., net::local::dgram::address)
	 * which the client must be connected to
	 */
	inline void connect (const Address& addr){
		AbstractSocket::socket_->connect(addr);
	}

	/**
	 * @brief Method to open the client towards a certain address
	 *
	 * This method calls net::local::dgram::client::connect(),
	 * which in turn invokes the platform-specific bind() operation.
	 * @param addr Address (i.e., net::local::dgram::address) 
	 * that must be open by the client
	 */
	inline void open (const Address& addr) {
		connect (addr);
	}

};


}}} // net::local::dgram

#endif // DGRAM_LOCAL_HPP_
