#include <iostream>
#include "tcp_ip4.hpp"
#include "address.hpp"

int main()
{
	net::ip4::tcp::address addr (std::string("127.0.0.1"), 1234);
	net::ip4::tcp::server s(addr);
	std::array<char, 5> b;
	s.read(buffer(b), 3);
	return 0;
}
	
