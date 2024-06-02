#ifndef __NOTICE_HPP__
#define __NOTICE_HPP__

#include "Command.hpp"

class Notice : public Command
{
  public:
    Notice();
    void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
    bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
