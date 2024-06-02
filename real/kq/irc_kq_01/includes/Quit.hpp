#ifndef __QUIT_HPP__
#define __QUIT_HPP__

#include "Command.hpp"

class Quit : public Command
{
    public:
	Quit();
	void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
	bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};


#endif
