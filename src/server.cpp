// server.cpp

# include "../include/server.h"

// container to track active client connections
std::vector<int> clients;

// broadcast message to all clients except the sender
void broadcast_msg(const std::string& msg, int sender);

// handle individual client connection
void handle_client(int client_fd);

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

    std::cout << "server listening on port " << PORT << std::endl;

    // continuous loop to accept client connections
    while(true)
    {
        // accept incoming client connection
        // return a new socket descriptor for this specific client 
        int client_fd = accept(server_fd, nullptr, nullptr);
        
        // store client file descriptor
        clients.push_back(client_fd);
        
        // create new thread for each client connection
        std::thread(handle_client, client_fd).detach(); // detach allows thread to run independently
    }
    return 0;
}

// broadcast message to all clients except the sender
void broadcast_msg(const std::string& msg, int sender) {
    for (int client : clients)
        if (client != sender)
            send(client, msg.c_str(), msg.size(), 0);
}

// handle individual client connection
void handle_client(int client_fd) 
{
    char buffer[1024]; // buffer to store received messages
    
    // main communication loop for this client
    while(true) 
    {
        // read data from the client
        int msg_length = read(client_fd, buffer, sizeof(buffer) -1);
        
        // check for connection closure (0) or read error (-1)
        if(msg_length <= 0) 
        {
            break;
        }
        
        // null-terminate the buffer to create valid string
        buffer[msg_length] = '\0';

        // create a message with client ID & their message
        std::string msg = "CLIENT " + std::to_string(client_fd) + ": " + buffer;

        // broadcast message to other clients
        broadcast_msg(msg, client_fd);
    }

    // close the client socket
    close(client_fd);
}