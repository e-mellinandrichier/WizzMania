// server.cpp

#include <cstring> // memset() & string operations
#include <iostream> // console output
#include <netinet/in.h> // internet address family
#include <sys/socket.h> // core socket functionality
#include <thread> //
#include <unistd.h> // POSIX OS - close(), read()
#include <vector> //

# include "../include/server.h"

std::vector<int> clients;

void handle_client(int client_fd) {}

int main () 
{
    // create socket file desprciptor
    // AF_INET: IPv4 internet protocols
    // SOCK_STREAM: TCP connection-oriented socket
    // 0: default protocol (TCP for SOCK_STREAM)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // config socket address
    // sockaddr_in: internet socket address structure
    sockaddr_in address 
    {
        AF_INET, // adress family (IPv4)
        htons(PORT), // host-to-network short
        INADDR_ANY // accept connections on any network interface
    };

    // bind socket to a specific network interface & port
    // connect to address & port specified
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 10);

    while(true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        {
            clients.push_back(client_fd);
        }
        std::thread(handle_client, client_fd).detach();
    }
    return 0;
}