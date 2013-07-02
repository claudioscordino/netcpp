export CXX = g++
export CXXFLAGS = -O3 -Wextra -std=c++11 -pthread

.PHONY: clean install doc 

all:
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean

