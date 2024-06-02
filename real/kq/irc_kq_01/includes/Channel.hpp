#ifndef _CHANNEL_HPP__
#define _CHANNEL_HPP__

#include <vector>

class Client;
#include "Client.hpp"

class Channel
{
        std::string _name;
        std::vector<Client*> _clients;
        int _opfd;
    public:
        Channel(std::string name, int opfd);

        std::vector<Client*>& getClients();
        void addClient(Client*);
        bool findJoinClient(std::string);

        std::string getChannelName() const;
        int getOpFd() const;
        void setOpFd(int);
};


#endif
