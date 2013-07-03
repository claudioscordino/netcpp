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



struct __buffer {
	void* ptr_;
	std::size_t size_;
};



template<std::size_t N>
inline __buffer buffer(std::array<char, N>& buf)
{
	__buffer b;
	b.ptr_ = reinterpret_cast<void*>(buf.data());
	b.size_ = buf.size();
	return b;
}

class AbstractSocket {
public:
	int read (__buffer buf, std::size_t size){
		if (buf.size_ == 0 || size > buf.size_)
			throw std::runtime_error ("Wrong buffer size");
		m_.lock();
		int ret = socket_->read(buf.ptr_, size);
		m_.unlock();
		return ret;
	}
	int write (__buffer buf, std::size_t size){
		if (buf.size_ == 0 || size > buf.size_)
			throw std::runtime_error ("Wrong buffer size");
		m_.lock();
		int ret = socket_->write(buf.ptr_, size);
		m_.unlock();
		return ret;

	}
	bool close(){
		return socket_->close();
	}



protected:
	AbstractSocket(AbstractSystemSocket* sock):
		socket_{sock}, worker_{nullptr} {}

	AbstractSystemSocket* socket_;

private:
	std::thread* worker_;
	std::mutex m_;
};

}

#endif // ABSTRACT_SOCKET_HPP_
