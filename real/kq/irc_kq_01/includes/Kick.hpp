#ifndef __KICK_HPP__
#define __KICK_HPP__

#include "Command.hpp"

class Kick : public Command
{
    public:
		Kick();
		void	execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
		bool	checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

#endif
