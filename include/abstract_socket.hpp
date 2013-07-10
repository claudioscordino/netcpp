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
#include "logger.hpp"


namespace net {

/** @brief Structure to avoid issues when giving buffers to AbstractSocket.
 *
 * This structure allows to avoid issues about templates when giving a buffer
 * to AbstractSocket. It is not meant to be directly used by the user.
 * The user must only use buffer(...).
 */
struct __buffer {
	/// Pointer to address holding data:
	void* ptr_;
	/// Size (in bytes) of data:
	std::size_t size_;
};

/**
 * @brief Function to pass a std::array to AbstractSocket
 */
template<std::size_t N>
inline __buffer buffer(std::array<char, N>& buf)
{
	return __buffer {reinterpret_cast<void*>(buf.data()), buf.size()};
}

/**
 * @brief Base abstract class for protocols.
 *
 * This is the base abstract class for protocols. Any supported protocol
 * must inherit from this class.
 */
class AbstractSocket {
public:
	/**
	 * @brief Read operation on the socket
	 *
	 * This function reads from the socket taking care of synchronization
	 * with other asynchronous operations.
	 * @param buf Pointer where read data must be put
	 * @param size Size of data to be read
	 * @return Number of bytes actually read
	 *
	 * Example of usage:
	 * <code>
	 * 		std::array<char, 5> buf;
	 * 		AbstractSocket::read(net::buffer(b), 3);
	 * </code>
	 */
	inline int read (__buffer buf, std::size_t size){
		if (buf.size_ == 0 || size > buf.size_){
			DEBUG(ERROR, "Wrong buffer size!");
			throw std::runtime_error ("Wrong buffer size");
		}
		m_.lock();
		int ret = socket_->read(buf.ptr_, size);
		m_.unlock();
		return ret;
	}

	/**
	 * @brief Write operation on the socket
	 *
	 * This function writes to the socket taking care of synchronization
	 * with other asynchronous operations.
	 * @param buf Pointer to data to be written
	 * @param size Size of data to be written
	 * @return Number of bytes actually written
	 *
	 * Example of usage:
	 * <code>
	 * 		std::array<char, 5> buf;
	 * 		AbstractSocket::write(net::buffer(b), 3);
	 * </code>
	 */
	inline int write (__buffer buf, std::size_t size){
		if (buf.size_ == 0 || size > buf.size_){
			DEBUG(ERROR, "Wrong buffer size!");
			throw std::runtime_error ("Wrong buffer size");
		}
		m_.lock();
		int ret = socket_->write(buf.ptr_, size);
		m_.unlock();
		return ret;

	}

	/**
	 * @brief Close the socket
	 * @return true in case of success; false otherwise
	 */
	inline bool close(){
		return socket_->close();
	}

	~AbstractSocket(){
		delete socket_;
		delete worker_;
	}

	/**
	 * @brief Pointer to the platform-dependent socket
	 */
	AbstractSystemSocket* socket_;

protected:
	/**
	 * @brief Constructor
	 *
	 * Only derived classes can use this constructor
	 */
	AbstractSocket(AbstractSystemSocket* sock):
		socket_{sock}, worker_{nullptr} {}



private:
	/**
	 * @brief Thread for asynchronous operations
	 */
	std::thread* worker_;

	/**
	 * @brief Mutex for synchronization with asynchronous operations.
	 */
	std::mutex m_;
};

}

#endif // ABSTRACT_SOCKET_HPP_
