#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include "tcp_ip4.hpp"
#include "address.hpp"
#include "logger.hpp"


void do_server()
{
	DEBUG(DEBUG, "SERVER: Creating address...");
	net::ip4::tcp::address addr (std::string("127.0.0.1"), 1234);
	DEBUG(DEBUG, "SERVER: Creating server...");
	net::ip4::tcp::server srv;
	DEBUG(DEBUG, "SERVER: Opening server...");
	srv.open(&addr);

	// sleep
	std::this_thread::sleep_for(std::chrono::seconds(2));


	while (true){
		DEBUG(DEBUG, "SERVER: Creating buffer...");
		std::array<char, 5> b;
		DEBUG(DEBUG, "SERVER: Reading...");
		srv.read(net::buffer(b), 3);
	}
}


void do_client()
{
	DEBUG(DEBUG, "CLIENT: Creating address...");
	net::ip4::tcp::address addr (std::string("127.0.0.1"), 1234);
	DEBUG(DEBUG, "CLIENT: Creating client...");
	net::ip4::tcp::client clt;
	DEBUG(DEBUG, "CLIENT: Opening client...");
	clt.open(&addr);

	std::this_thread::sleep_for(std::chrono::seconds(2));



	while (true){
		DEBUG(DEBUG, "CLIENT: Creating buffer...");
		std::array<char, 5> b;
		DEBUG(DEBUG, "CLIENT: Writing...");
		clt.write(net::buffer(b), 3);
	}
}

int main()
{
	DEBUG_CONF("/tmp/netwing.txt", DEBUG, DEBUG);
	DEBUG(DEBUG, "Launching 1st thread...");
	std::thread t1(do_server);
	DEBUG(DEBUG, "Launching 2nd thread...");
	std::thread t2(do_client);
	t1.join();
	t2.join();
	return 0;
}
	
