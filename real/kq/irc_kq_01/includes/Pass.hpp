#ifndef __PASS_HPP__
# define __PASS_HPP__

#include "Command.hpp"
#include <vector>

class Pass : public Command
{
	public:
		Pass();
		void	execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels);
		bool	checkArgs(std::vector<std::string>& cmdlist, Client& client);
};

// static void print_all_about_channel(Channel &c);

#endif
