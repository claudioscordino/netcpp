/*
 * posix_socket.hpp
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

#ifndef POSIX_SOCKET_HPP_
#define POSIX_SOCKET_HPP_

#include <unistd.h>

#include "abstract_system_socket.hpp"
#include "address.hpp"
#include "protocol.hpp"


namespace net {

class PosixSocket: public AbstractSystemSocket {

public:
	virtual void connect (Address* addr);
	virtual void bind (Address* addr);

	virtual int read (void* buffer, size_t size);
	virtual int write (const void* buffer, size_t size);

	virtual bool close();

	virtual void accept (AbstractSystemSocket* sock);

	virtual void listen (int maxPendingConnections);

	PosixSocket(const enum protocol& prot);
	virtual ~PosixSocket();


private:

	/**
	 * \brief Number of the file descriptor.
	 * This is the return value of open(), socket() or accept().
	 */
	int fd_;

};

}


#endif // POSIX_SOCKET_HPP_

