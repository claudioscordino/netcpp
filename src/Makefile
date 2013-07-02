.PHONY: clean 
CXXFLAGS += -I../include

all:  tcp_ip4.o posix_socket.o logger.o main.o
	$(CXX) $(CXXFLAGS) -o netwing tcp_ip4.o posix_socket.o logger.o main.o -pthread

posix_socket.o: posix_socket.cpp ../include/*.hpp

tcp_ip4.o: tcp_ip4.cpp ../include/*.hpp

logger.o: logger.cpp ../include/*.hpp

main.o: main.cpp ../include/*.hpp


clean:
	rm -fr *.o
