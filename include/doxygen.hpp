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

/** 
 *
 * \htmlonly
 * <br>
 * <center>
 * <img src="../libnetpp.png" width="250"></img><br>
 * A C++ network library
 * </center>
 * <br>
 * \endhtmlonly
 *
 * @mainpage LibNetpp
 * 
 * @author Evidence Srl - <a href="http://www.evidence.eu.com" target="_blank">www.evidence.eu.com</a>
 *
 * <br>
 * <br>
 * <h1>Introduction</h1>
 *
 * LibNetpp is a tiny C++ network library.
 *
 *
 * <br>
 * <br>
 * <h1>Rationale</h1>
 *
 * The C++ standard library is known to be less comprehensive than the libraries
 * traditionally available for other programming languages (e.g., Java, Python,
 * etc.). The C++11 standard began to fill this gap by adding several features
 * like support for multithreading, smart pointers, etc. Incredibly, however, a
 * basic support for networking is still missing.
 *
 * During the last years, a number of C++ libraries have been proposed to have
 * portable networking in C++, the most important being
 * <a href="http://www.boost.org/libs/asio/" target="_blank">Boost::asio</a>
 * and <a href="http://cpp-netlib.org" target="_blank">cpp-netlib</a>.
 * However, we believe that the syntax of both these libraries is still more
 * complicated than necessary.
 * In particular, we believe that an ideal C++ networking library should be:
 * <ul>
 * <li> easy to use
 * <li> easy to extend (either in terms of protocols and of supported architectures)
 * </ul>
 *
 * LibNetpp is a proof of concept to show that such a networking library can be
 * made easily designed in C++. This tiny library has been built after the
 * experience with the <a href="http://onposix.sourceforge.net" target="_blank">OnPosix library</a>.
 * The syntax of the library is straightforward. The modular design given by the
 * adoption of design patterns allows to easily add new protocols and supported
 * platforms.
 *
 *
 * <br>
 * <br>
 * <h1>Requirements</h1>
 *
 * To build LibNetpp you need both:
 * <ul>
 * <li> <a href="http://www.cmake.org" target="_blank">cmake</a>
 * <li> a C++11 compiler (e.g., gcc 4.7+ or LLVM 3.3+)
 * </ul>
 * 
 * Currently, only Posix systems (e.g., Linux). However, porting the library to
 * different platforms is very easy thanks to its modular design.
 *
 *
 * <br>
 * <br>
 * <h1>How to build the library</h1>
 *
 * Compile through the following commands:
 *
 * \code
 * git clone https://github.com/claudioscordino/libnetpp.git
 * cd libnetpp/build
 * cmake ..
 * make
 * make install
 * \endcode
 *
 * The library is put in the <i>libnetpp/bin/</i> directory.
 *
 * Documentation is generated through
 * <a href="htttp://www.doxygen.org" target="_blank">Doxygen</a>.
 * To generate the documentation, type:
 *
 * \code
 * git clone https://github.com/claudioscordino/libnetpp.git
 * cd libnetpp/build
 * cmake ..
 * make doc
 * \endcode
 *
 * Start reading documentation by opening <i>doc/html/index.html</i>.
 *  
 *
 *
 * <br>
 * <br>
 * <h1>Usage</h1>
 *
 * Usage of the library is straightforward:
 *
 * Example of TCP server:
 * \code
 * net::ip4::tcp::address addr (std::string("127.0.0.1"), 1234);
 * net::ip4::tcp::server main_srv;
 * net::ip4::tcp::server srv(&main_srv);
 * srv.open(&addr);
 * std::array<char, 5> buf;
 * srv.read(net::buffer(buf), 5);
 * \endcode
 *
 * Example of TCP client:
 * \code
 * net::ip4::tcp::address addr (std::string("127.0.0.1"), 1234);
 * net::ip4::tcp::client clt;
 * clt.open(&addr);
 * std::array<char, 5> b {'h', 'e', 'l', 'l', 'o'};
 * clt.write(net::buffer(b), 3);
 * \endcode
 *
 *
 * <br>
 * <br>
 * <h1>Supported platforms</h1>
 *
 * The library has a modular internal design which, thanks to the design patterns 
 * used, allows to decouple specific platform-dependent code (which is on the
 * AbstractSystemSocket hierarchy) from any protocol design (which is on the
 * AbstractSocket hierarchy).
 *
 * Thus, to add a new supported platform, you only need to inherit your
 * platform-specific class from class AbstractSystemSocket.
 * The build system, based on cmake, can be easily ported among all supported
 * platforms.
 *
 * <br>
 * <br>
 * <h1>Adding further network protocols</h1>
 *
 * To add a new protocol:
 * <ul>
 * <li> Create a proper namespace (like ip4::tcp)
 * <li> Inside the new namespace:
 * <ul>
 * <li> Create an address class inheriting from class Address
 * <li> Create server and client classes inheriting from class AbstractSocket
 * </ul>
 * </ul>
 *
 * <br>
 * <br>
 * <h1>Todo</h1>
 * <ul>
 * <li> Add other protocols besides TCP/IP and local Unix sockets.
 * <li> Add asynchronous operations
 * </ul>
 */
