#include "../includes/Channel.hpp"

Channel::Channel(std::string name, int opfd) : _name(name), _opfd(opfd) {}

std::vector<Client*>& Channel::getClients() { return (_clients); }

void Channel::addClient(Client *newClient)
{
    _clients.push_back(newClient);
}

bool Channel::findJoinClient(std::string target){
  for (std::vector<Client *>::iterator found = _clients.begin(); found != _clients.end(); ++found){
    if ((*found)->getNickname() == target)
      return true;
  }
  return false;
}

std::string Channel::getChannelName() const { return (_name); }

int Channel::getOpFd() const { return (_opfd); }

void Channel::setOpFd(int fd) { _opfd = fd; }
