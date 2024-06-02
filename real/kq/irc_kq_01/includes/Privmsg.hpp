#ifndef __PRIVMSG_HPP__
#define __PRIVMSG_HPP__

#include "Command.hpp"

class Privmsg : public Command
{
  public:
    Privmsg();
    void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
    bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
