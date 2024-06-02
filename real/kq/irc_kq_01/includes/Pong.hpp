#ifndef __PONG_HPP__
#define __PONG_HPP__

#include "Command.hpp"
#include <utility>

class Pong : public Command
{
  public:
    Pong();
    void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
    bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
