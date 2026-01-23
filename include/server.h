// server.h

# pragma once

#include <arpa/inet.h> // inet_pton() - internet presentation to network
#include <cstring> // memset() & string operations
#include <iostream> // console output
#include <netinet/in.h> // internet address family
#include <sys/socket.h> // core socket functionality
#include <thread> // for multi-threading
#include <unistd.h> // POSIX OS - close(), read()
#include <vector> // to store client connections

constexpr const char* SERVER_ADDRESS = "127.0.0.1"; // localhost IP
constexpr int PORT = 8080; // standard port for local dev
