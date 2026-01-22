// server.cpp

#include <cstring> // memset() & string operations
#include <iostream> // console output
#include <netinet/in.h> // internet address family
#include <sys/socket.h> // core socket functionality
#include <thread> // for multi-threading
#include <unistd.h> // POSIX OS - close(), read()
#include <vector> // to store client connections

# include "../include/server.h"

// container to track active client connections
std::vector<int> clients;

void handle_client(int client_fd) {}

int main () 
{
    // create socket file descriptor
    // AF_INET: IPv4 internet protocols
    // SOCK_STREAM: TCP connection-oriented socket
    // 0: default protocol (TCP for SOCK_STREAM)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // config socket address for network binding
    // sockaddr_in: internet socket address struct
    sockaddr_in address 
    {
        AF_INET, // address family (IPv4)
        htons(PORT), // host-to-network short - convert port to network byte order
        INADDR_ANY // accept connections on any network interface
    };

    // bind socket to network interface & port
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // start listening for incoming connections
    listen(server_fd, 10); // backlog 10 max pending connections

    // continuous loop to accept client connections
    while(true)
    {
        // accept incoming client connection
        int client_fd = accept(server_fd, nullptr, nullptr);
        
        // store client file descriptor
        {
            clients.push_back(client_fd);
        }
        
        // create new thread for each client connection
        std::thread(handle_client, client_fd).detach(); // detach allows thread to run independently
    }
    return 0;
}