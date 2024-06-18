#include "../includes/Command.hpp"
#include "../includes/Util.hpp"

void Command::part(int fd, std::vector<std::string> cmdVector)
{
	// PART <channel>{,<channel>}
	std::map<int, Client>& clientList = _server.getClientList();
	std::map<int, Client>::iterator operator_iter;

	operator_iter = clientList.find(fd);
	if (cmdVector.size() < 2)
	{
		ERROR_needmoreparams_461(operator_iter->second);
		return;
	}

	std::stringstream ss(cmdVector[1]);			// <channel>{,<channel>}
	std::string buffer;
	std::vector<std::string> channelVector;
	while (getline(ss, buffer, ','))			//	<channel> <channel> <channel> ... 담는다
		channelVector.push_back(buffer);
	
	std::vector<std::string>::iterator channelVector_iter;
	channelVector_iter = channelVector.begin();

	


}