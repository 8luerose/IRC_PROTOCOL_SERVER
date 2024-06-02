#ifndef __JOIN_HPP__
#define __JOIN_HPP__

#include "Command.hpp"

class Join : public Command
{
  public:
    Join();
    void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
    bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
