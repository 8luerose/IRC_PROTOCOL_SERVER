#include "../includes/Notice.hpp"
#include "../includes/Server.hpp"

#include <sstream>

Notice::Notice() : Command(2) {}

bool  Notice::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
    Server& server = Server::getInstance();
    if (static_cast<int>(cmdlist.size()) - 1 < getRequiredArgsNumber())
    {
        makeWriteEvent(client.getUserSock(), server.getChangeList(), ERR_NEEDMOREPARAMS(client.getNickname(), mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

void Notice::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	(void) changelist;
    std::vector<Channel*> targetChannel;
    std::vector<Client*> targetUser;

    Server& server = Server::getInstance();
    if (checkArgs(cmdlist, client))
    {
        //parse target field
        std::stringstream ss(cmdlist[1]);

        std::string token;
        while (getline(ss, token, ','))
        {
            if (token[0] == '#')
            {
                token.erase(0, 1);
                std::map<std::string, Channel>::iterator found = channels->find(token);
                if (found != Server::getInstance().getChannels().end())
                    targetChannel.push_back(&found->second);
            }
            else
            {
                Client *found = const_cast<Client *>(server.findUserByNick(token));
                if (found != NULL)
                    targetUser.push_back(found);
            }
        }

        //append users from channel
        for (std::vector<Channel*>::iterator it = targetChannel.begin(); it != targetChannel.end(); ++it)
        {
            std::vector<Client*> clientsInChannel = (*it)->getClients();
            for (std::vector<Client*>::iterator it = clientsInChannel.begin(); it != clientsInChannel.end(); ++it)
            {
                if ((*it)->getUserSock() != client.getUserSock())
                    targetUser.push_back(*it);
            }
        }

        //iterate target user to make event
        for (std::vector<Client *>::iterator it = targetUser.begin(); it != targetUser.end(); ++it)
            makeWriteEvent((*it)->getUserSock(), changelist, SUCCESS_REPL(client.getNickname(), mergeMsg(cmdlist, 2)));
    }
}
