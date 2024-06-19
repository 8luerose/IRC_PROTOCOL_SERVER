#include "Server.hpp"

void Server::addClient(int fd, Client client) {
    clientList[fd] = client;
}

void Server::addChannel(std::string channelName, Channel* channel) { // Channel*로 수정
    channelList[channelName] = channel;
}

std::map<int, Client>& Server::getClientList() {
    return clientList;
}

std::map<std::string, Channel*>& Server::getChannelList() { // Channel*로 수정
    return channelList;
}

Channel* Server::findChannel(const std::string& channelName) {
    if (channelList.find(channelName) != channelList.end()) {
        return channelList[channelName];
    }
    return nullptr;
}

std::map<int, Client>::iterator Server::findClient(const std::string& nickname) {
    for (std::map<int, Client>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            return it;
        }
    }
    return clientList.end();
}
