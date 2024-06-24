#ifndef SERVER_HPP
#define SERVER_HPP


#include <iostream>
#include <map>
#include <string>
#include "Client.hpp"
#include "Channel.hpp"

// Forward declaration of Client and Channel classes
class Client;
class Channel;

class Server {
private:
    std::map<int, Client> clientList;
    std::map<std::string, Channel*> channelList;

public:
    void addClient(int fd, Client client);
    void addChannel(std::string channelName, Channel* channel);
    std::map<int, Client>& getClientList();
    std::map<std::string, Channel*>& getChannelList();
    Channel* findChannel(const std::string& channelName);
    std::map<int, Client>::iterator findClient(const std::string& nickname);
};

#endif
