// client.cpp

#include "../include/server.h"

// function to listen to the server & receive messages
void receive_messages(int server_socket);

int main()
{
    return 0;
}

// function to listen to the server & receive messages
void receive_messages(int server_fd)
{
    char buffer[1024]; // buffer to store received messages
    
    while(true) 
    {
        // read data from the server
        int msg_length = read(server_fd, buffer, sizeof(buffer) -1);
        
        // check for connection closure (0) or read error (-1)
        if(msg_length <= 0) 
        {
            break;
        }
        
        // null-terminate the buffer to create valid string
        buffer[msg_length] = '\0';

        std::cout << "New message received: " << buffer << std::endl; 
    }
}

