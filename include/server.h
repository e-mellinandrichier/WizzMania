// server.h

# pragma once

#include <cstring> // memset() & string operations
#include <iostream> // console output
#include <netinet/in.h> // internet address family
#include <sys/socket.h> // core socket functionality
#include <thread> // for multi-threading
#include <unistd.h> // POSIX OS - close(), read()
#include <vector> // to store client connections

#define PORT 8080 // standard port for local dev