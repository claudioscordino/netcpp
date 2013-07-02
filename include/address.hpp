/*
 * address.hpp
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

#ifndef ADDRESS_HPP_
#define ADDRESS_HPP_

#include <string>

namespace net {

	class Address {
	public:
		virtual std::string getAddress() const =0;
	};

	namespace ip4 {
		class address: public Address {
		public:
			address(const std::string& addr):
				address_{addr}{};
			std::string toString(){
				return address_;
			}
		private:
			std::string address_;
		};

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
				std::string address_;
				int port_;
			};

		}

		namespace udp {
			typedef net::ip4::tcp::address address;
		}

	}

	namespace local {
		typedef net::ip4::address address;
	}
}



#endif // ADDRESS_HPP_

