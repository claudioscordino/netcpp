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

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdexcept>
#include <strings.h>
#include <cstring>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "posix_socket.hpp"
#include "protocol.hpp"
#include "logger.hpp"
#include "tcp_ip4.hpp"


namespace net {


PosixSocket::PosixSocket(const enum protocol& prot):
		AbstractSystemSocket(prot)
{
	if (prot == protocol::STREAM_LOCAL)
		fd_ = socket(AF_LOCAL, SOCK_STREAM, 0);
	else if (prot == protocol::DGRAM_LOCAL)	
		fd_ = socket(AF_LOCAL, SOCK_DGRAM, 0);
	else if (prot == protocol::TCP_IPv4)	
		fd_ = socket(AF_INET, SOCK_STREAM, 0);
	else if (prot == protocol::UDP_IPv4)	
		fd_ = socket(AF_INET, SOCK_DGRAM, 0);
	else {
 		DEBUG(ERROR, "Error: protocol unkown");
		throw std::runtime_error ("Protocol unknown");
	}
	if (fd_ < 0) {
 		DEBUG(ERROR, "Error when creating socket");
		throw std::runtime_error ("Socket error");
	}
}

PosixSocket::~PosixSocket()
{
	close();
}


/**
 * \brief Low-level read
 *
 * @param buffer Pointer to the buffer where read bytes must be stored
 * @param size Number of bytes to be read
 * @return The number of actually read bytes or -1 in case of error
 */
int PosixSocket::read (void* buffer, size_t size)
{
	return ::read(fd_, buffer , size);
}



/**
 * \brief Low-level write
 *
 * @param buffer Pointer to the buffer containing bytes to be written
 * @param size Number of bytes to be written
 * @return The number of actually written bytes or -1 in case of error
 */
int PosixSocket::write (const void* buffer, size_t size)
{
	return ::write(fd_, buffer, size);
}


bool PosixSocket::close(){
	return !(::close(fd_));
}

/**
 * \brief Constructor to accept() connections on a socket.
 *
 * This constructor calls accept().
 * @param socket on which a new connection must be accepted.
 * @exception runtime_error in case of error in accept()
 */
void PosixSocket::accept (AbstractSystemSocket* sock)
{
	if ((sock->getProtocol() != protocol::STREAM_LOCAL && sock->getProtocol() != protocol::TCP_IPv4) ||
	    (protocol_ != protocol::STREAM_LOCAL && protocol_ != protocol::TCP_IPv4)){
 		DEBUG(ERROR, "Accept not available!");
		throw std::runtime_error("Accept not available");
	}

	fd_ = ::accept((dynamic_cast<PosixSocket*> (sock))->fd_, NULL, 0);
	if (fd_ < 0) {
 		DEBUG(ERROR, "Error in accept()!");
		throw std::runtime_error("Accept error");
	}
}


void PosixSocket::listen (int maxPendingConnections)
{
	if (protocol_ != protocol::STREAM_LOCAL && protocol_ != protocol::TCP_IPv4){
 		DEBUG(ERROR, "Listen not available!");
		throw std::runtime_error("Listen not available");
	}
	if (::listen(fd_, maxPendingConnections) < 0) {
		::close(fd_);
 		DEBUG(ERROR, "Error when listening");
		throw std::runtime_error ("Listen error");
	}
}

void PosixSocket::bind (Address* addr)
{
	if ((protocol_ == protocol::DGRAM_LOCAL) || (protocol_ == protocol::STREAM_LOCAL)) { 
		DEBUG(DEBUG, "Local protocol found");
		struct sockaddr_un serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sun_family = AF_LOCAL;
		strncpy(serv_addr.sun_path, addr->getAddress().c_str(),
		    sizeof(serv_addr.sun_path) - 1);
		if (::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			return;
	} else if ((protocol_ == protocol::TCP_IPv4) || (protocol_ == protocol::UDP_IPv4)) {
		DEBUG(DEBUG, "Network protocol found");
		struct sockaddr_in serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons((dynamic_cast<ip4::tcp::address*>(addr))->getPort());
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		if (::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			return;
	} else {
		DEBUG(ERROR, "Unknown protocol found");
	}

error:
	::close(fd_);
 	DEBUG(ERROR, "Error when binding socket");
	throw std::runtime_error ("Bind error");
}


void PosixSocket::connect (Address* addr)
{
	if ((protocol_ == protocol::DGRAM_LOCAL) || (protocol_ == protocol::STREAM_LOCAL)) {
		struct sockaddr_un serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sun_family = AF_LOCAL;
		strncpy(serv_addr.sun_path, addr->getAddress().c_str(), sizeof(serv_addr.sun_path) - 1);
		if (::connect(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			return;
	} else if ((protocol_ == protocol::UDP_IPv4) || (protocol_ == protocol::TCP_IPv4)) {
		struct sockaddr_in serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons((dynamic_cast<ip4::tcp::address*>(addr))->getPort());


		struct in_addr add;
		inet_aton(addr->getAddress().c_str(), &add);
		bcopy(&add, &serv_addr.sin_addr.s_addr, sizeof(add));

		if (::connect(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			return;
	}

error:
	::close(fd_);
 	DEBUG(ERROR, "Error when creating client socket");
	throw std::runtime_error ("Client socket error");
}






}
