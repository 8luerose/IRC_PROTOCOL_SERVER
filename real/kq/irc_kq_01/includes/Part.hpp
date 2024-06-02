#ifndef __PART_HPP__
#define __PART_HPP__

#include "Command.hpp"

class Part : public Command
{
    public:
		Part();
		void  execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
	    bool  checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
