NAME=WIZZmania

# compiler & flags
CXX=g++
CXXFLAGS=-Wall -g -std=c++17 -pthread

# executables
WM_SERVER=server
WM_CLIENT=client

# source files
SERVER_SRC=src/server.cpp
CLIENT_SRC=src/client.cpp

# default
all: server client

# build server
server:
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(WM_SERVER)

# build client
client:
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(WM_CLIENT)

# clean up
.PHONY: clean
clean:
	rm -f $(WM_SERVER) $(WM_CLIENT)