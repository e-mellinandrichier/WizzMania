NAME=WIZZmania

# compiler & flags
CXX=g++
CXXFLAGS=-Wall -g -std=c++17

# executables
WM_SERVER=server

# source files
SERVER_SRC=src/server.cpp

# default
all: server

# build server
server:
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(WM_SERVER)

# clean up
.PHONY: clean
clean:
	rm -f $(SERVER)