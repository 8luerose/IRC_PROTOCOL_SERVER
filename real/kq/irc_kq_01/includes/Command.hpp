#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include "Client.hpp"
#include "Channel.hpp"
#include <exception>
#include <map>
#include <string>
#include <sys/event.h>

class Command
{
		int		_requiredArgsNumber;
		Command();
	public:
		Command(int argnum);
		virtual void	execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels) = 0;
		virtual bool	checkArgs(std::vector<std::string>& cmdlist, Client& client) = 0;
		void	makeWriteEvent(int ident, std::vector<struct kevent>& changelist, std::string msg);
		int	getRequiredArgsNumber() const;
};

#endif
