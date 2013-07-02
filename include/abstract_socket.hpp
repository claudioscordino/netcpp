/*
 * abstract_socket.hpp
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

#ifndef ABSTRACT_SOCKET_HPP_
#define ABSTRACT_SOCKET_HPP_

#include <unistd.h>
#include <mutex>
#include <stdexcept>
#include <array>
#include <thread>

#include "abstract_system_socket.hpp"


namespace net {

#if 0
class AsyncOperation {
public:
	inline void Run(){
		if (read_operation_)
			ret_ = socket_->read(buf_->data(), size_);
		else
			ret_ = socket_->write(buf_->data(), size_);
		m_->unlock();
	}
	inline int getResult(){
		return ret_;
	}

private:
	std::array<>* buf_;
	size_t size_;
	bool read_operation_;
	int ret_;
	AbstractSystemSocket* socket_;
	std::mutex* m_;

	friend class AbstractSocket;
};
#endif

template<std::size_t N>
inline void* buffer(std::array<char, N>& buf)
{
	return reinterpret_cast<void*>(buf.data());
}

class AbstractSocket {
public:
	int read (void* buf, std::size_t size){
		if (buf->size() == 0 || size > buf->size())
			throw std::runtime_error ("Wrong buffer size");
		m_.lock();
		int ret = socket_->read(buf->data(), size);
		m_.unlock();
		return ret;
	}
	int write (void* buf, std::size_t size){
		if (buf->size() == 0 || size > buf->size())
			throw std::runtime_error ("Wrong buffer size");
		m_.lock();
		int ret = socket_->write(buf->data(), size);
		m_.unlock();
		return ret;

	}
	bool close(){
		return socket_->close();
	}



protected:
	AbstractSocket(AbstractSystemSocket* sock):
		socket_{sock}, worker_{nullptr}, m_{nullptr}{}

	AbstractSystemSocket* socket_;

private:
	std::thread* worker_;
	std::mutex m_;
};

}

#endif // ABSTRACT_SOCKET_HPP_
