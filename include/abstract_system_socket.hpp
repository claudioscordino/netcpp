/*
 * abstract_system_socket.hpp
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

#ifndef ABSTRACT_SYSTEM_SOCKET_HPP_
#define ABSTRACT_SYSTEM_SOCKET_HPP_

#include <unistd.h>

#include "protocol.hpp"
#include "address.hpp"

namespace net {

class AbstractSystemSocket {

public:
	virtual int read (void* buf, size_t size)=0;
	virtual int write (const void* buf, size_t size)=0;
	virtual bool close()=0;
	virtual void accept (AbstractSystemSocket* sock)=0;
	virtual void connect (Address* addr)=0;
	virtual void bind (Address* addr)=0;
	virtual void listen (int max_pending_connections)=0;
	protocol getProtocol() const {
		return protocol_;
	}

protected:
	AbstractSystemSocket(const protocol& prot):
		protocol_{prot}{};

	protocol protocol_;
};

}


#endif // ABSTRACT_SYSTEM_SOCKET_HPP_

