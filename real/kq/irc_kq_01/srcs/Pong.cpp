#include "../includes/Pong.hpp"
#include "../includes/Define.hpp"
#include "../includes/Server.hpp"

Pong::Pong() : Command(1) {}

bool Pong::checkArgs(std::vector<std::string> &cmdlist, Client &client)
{
    if (static_cast<int>(cmdlist.size()) - 1 < getRequiredArgsNumber())
    {
        makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS(client.getUserName(), mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

void Pong::execute(std::vector<std::string> &cmdlist, Client &client, std::vector<struct kevent> &changelist, std::map<std::string, Channel> *channels)
{
	(void) channels;
	cmdlist[0] = "PONG";
    if (checkArgs(cmdlist, client))
        makeWriteEvent(client.getUserSock(), changelist, SUCCESS_REPL(client.getUserName(), mergeVec(cmdlist)));
}
