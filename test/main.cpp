#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include "tcp_ip4.hpp"
#include "stream_local.hpp"
#include "address.hpp"
#include "logger.hpp"


void do_server()
{
	DEBUG(DEBUG, "SERVER: Creating address...");
	net::local::stream::address addr ("/tmp/libnetpp");
	DEBUG(DEBUG, "SERVER: Creating server...");
	net::local::stream::server srv;
	DEBUG(DEBUG, "SERVER: Opening server...");
	srv.open(&addr);

	DEBUG(DEBUG, "SERVER: Creating server2...");
	net::local::stream::server srv2(&srv);

	// sleep
// 	std::this_thread::sleep_for(std::chrono::seconds(2));

	while (true){
		DEBUG(DEBUG, "SERVER: Creating buffer...");
		std::array<char, 5> b;
		DEBUG(DEBUG, "SERVER: Reading...");
		srv2.read(net::buffer(b), 3);
		DEBUG(DEBUG, "SERVER: Received: " << b[0] << " "
			<< b[1] << " " 
			<< b[2] << " "
			<< b[3] << " "
			<< b[4]);
	}
}


void do_client()
{
	std::this_thread::sleep_for(std::chrono::seconds(2));

	DEBUG(DEBUG, "CLIENT: Creating address...");
	net::local::stream::address addr ("/tmp/libnetpp");
	DEBUG(DEBUG, "CLIENT: Creating client...");
	net::local::stream::client clt;
	DEBUG(DEBUG, "CLIENT: Opening client...");
	clt.open(&addr);


	while (true){
		DEBUG(DEBUG, "CLIENT: Creating buffer...");
		std::array<char, 5> b {'c', 'i', 'a', 'o'};
		DEBUG(DEBUG, "CLIENT: Writing...");
		clt.write(net::buffer(b), 3);
	}
}

int main()
{
	DEBUG_CONF("libnetpp.log", DEBUG, DEBUG);
	DEBUG(DEBUG, "Launching 1st thread...");
	std::thread t1(do_server);
	DEBUG(DEBUG, "Launching 2nd thread...");
	std::thread t2(do_client);
	t1.join();
	t2.join();
	return 0;
}
	
