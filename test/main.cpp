/*
 * Copyright (C) 2013 Evidence Srl - www.evidence.eu.com
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

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
	net::local::stream::address addr ("/tmp/netcpp");
	DEBUG(DEBUG, "SERVER: Creating server...");
	net::local::stream::server srv;
	DEBUG(DEBUG, "SERVER: Opening server...");
	srv.open(addr);

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
	net::local::stream::address addr ("/tmp/netcpp");
	DEBUG(DEBUG, "CLIENT: Creating client...");
	net::local::stream::client clt;
	DEBUG(DEBUG, "CLIENT: Opening client...");
	clt.open(addr);


	while (true){
		DEBUG(DEBUG, "CLIENT: Creating buffer...");
		std::array<char, 5> b {'c', 'i', 'a', 'o'};
		DEBUG(DEBUG, "CLIENT: Writing...");
		clt.write(net::buffer(b), 3);
	}
}

int main()
{
	DEBUG_CONF("netcpp.log", DEBUG, DEBUG);
	DEBUG(DEBUG, "Launching 1st thread...");
	std::thread t1(do_server);
	DEBUG(DEBUG, "Launching 2nd thread...");
	std::thread t2(do_client);
	t1.join();
	t2.join();
	return 0;
}
	
