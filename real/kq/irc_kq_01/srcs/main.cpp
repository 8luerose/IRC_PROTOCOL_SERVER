#include <iostream>
#include <string>
#include <sys/fcntl.h>
#include <vector>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "../includes/Server.hpp"

Server Server::_server;

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        Server& server = Server::getInstance();
        server.servSetup(argv[1]);
        server.setPswd(argv[2]);
        server.run();
    }
    else
        std::cerr << "Usage) ./ircserv <port> <password>";
    return (0);
}
