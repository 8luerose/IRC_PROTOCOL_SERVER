#ifndef __USER_HPP__
# define __USER_HPP__

#include "Command.hpp"

#include <utility>

class User : public Command
{
  public:
    User();
    void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
    bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};


#endif
