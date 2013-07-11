/*
 * posix_socket.cpp
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
 * This method is private because it is meant to be used through the other read()
 * methods.
 * Note: it can block the caller, because it continues reading until the given
 * number of bytes have been read.
 * @param buffer Pointer to the buffer where read bytes must be stored
 * @param size Number of bytes to be read
 * @exception runtime_error if the ::read() returns an error
 * @return The number of actually read bytes or -1 in case of error
 */
int PosixSocket::read (void* buffer, size_t size)
{
	size_t remaining = size;
	while (remaining > 0) {
		ssize_t ret = ::read (fd_, ((char*)buffer)+(size-remaining),
		    remaining);
		if (ret == 0){
			// End of file reached
			DEBUG(DEBUG, "End of file reached");
			break;
		} else if (ret < 0) {
			DEBUG(ERROR, "Read error");
			throw std::runtime_error ("Read error");
			return -1;
		}
		remaining -= ret;
	}
	return (size-remaining);
}



/**
 * \brief Low-level write
 *
 * This method is private because it is meant to be used through the other
 * write() methods.
 * Note: it can block the caller, because it continues writing until the
 * given number of bytes have been written.
 * @param buffer Pointer to the buffer containing bytes to be written
 * @param size Number of bytes to be written
 * @exception runtime_error if the ::write() returns 0 or an error
 * @return The number of actually written bytes or -1 in case of error
 */
int PosixSocket::write (const void* buffer, size_t size)
{
	size_t remaining = size;
	while (remaining > 0) {
		ssize_t ret = ::write (fd_,
		    ((char*)buffer)+(size-remaining), remaining);
		if (ret == 0){
			DEBUG(DEBUG, "Cannot write more");
			// Cannot write more
			break;
		} else if (ret < 0) {
			DEBUG(ERROR, "Write error");
			throw std::runtime_error ("Write error");
			return -1;
		}
		remaining -= ret;
	}
	return (size-remaining);
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
		struct sockaddr_un serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sun_family = AF_LOCAL;
		strncpy(serv_addr.sun_path, addr->getAddress().c_str(),
		    sizeof(serv_addr.sun_path) - 1);
		if (::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			return;
	} else if ((protocol_ == protocol::TCP_IPv4) || (protocol_ == protocol::UDP_IPv4)) {
		struct sockaddr_in serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons((dynamic_cast<ip4::tcp::address*>(addr))->getPort());
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		if (::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
			return;
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
