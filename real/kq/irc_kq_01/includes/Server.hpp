#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <iostream>
#include <string>
#include <sys/event.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <err.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <vector>

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandInvoker.hpp"
#include "Define.hpp"

#define BACKLOG 5
#define MAX_EVENTS 10
#define BUFFER_SIZE 512
 
class Server
{
        static Server _server;

        int _kq;
        int _servSock;
        CommandInvoker _invoker;

        std::string _password;
        std::map<int, Client> _clients;
        std::map<std::string, Channel> _channels;
        std::vector<struct kevent> _changelist;

        Server(){};
    
        void handleEof(struct kevent &k);
        void handleError(struct kevent &k);
        void acceptUser();
        void handleRead(struct kevent &k);
        void handleWrite(struct kevent &k);

    public:

        static Server& getInstance();

        void servSetup(char *port);
        void cmdsSetup();
        void run();

        void setPswd(std::string);

        std::string                       getPswd() const;
        std::map<int, Client>&            getClients();
        std::map<std::string, Channel>&   getChannels();
        std::vector<struct kevent>&       getChangeList();

        const Client* findUserByNick(std::string target);
        void close_sequance(void);
};

std::string mergeVec(const std::vector<std::string> &vec);
std::string mergeMsg (const std::vector<std::string> &vec, int idx);

#endif
