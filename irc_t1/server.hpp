#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include "main.hpp"
#include "Command.hpp"
#include "Channel.hpp"

class Client;
class Channel;
class Command;

class Server {
public:
    Server(char *, char *);
    ~Server();

    std::map<int, Client>& getClients();
    std::string getPassword();
    std::string getMessage(int);
    std::map<std::string, Channel *>& getChannelList();
    void run();
    Channel* findChannel(std::string);
    std::map<int, Client>::iterator findClient(std::string);
    void removeChannel(std::string);
    void appendNewChannel(std::string&, int);

private:
    Server();
    Server(const Server &);
    Server &operator=(const Server &);

    std::string _password;
    unsigned short int _portNum;
    int _serverSock;
    struct sockaddr_in _serverAddr;
    int _kq;
    struct kevent _changeList[256];
    int _changeListSize;
    struct kevent _eventList[256];
    std::map<int, Client> _clients;
    std::map<std::string, Channel *> _channelList;
    std::string _message[BUF_SIZE];
    Command *_command;

    unsigned short int setPortNum(char *);
    std::string setPassword(char *);
    void setServerSock();
    void setServerAddr();
    void setServerBind();
    void setServerListen();
    void setBot();
    void execute();
    int recvMessage(int);
    void addClient(int);
    bool checkMessageEnds(int);
    void doCommand(int);
    void addEvent(int fd, int16_t filter, uint16_t flags);
    void removeEvent(int fd, int16_t filter);
};

#endif
