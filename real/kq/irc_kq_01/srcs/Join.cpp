#include "../includes/Join.hpp"
#include "../includes/Server.hpp"
#include <sstream>
#include <utility>
#include <vector>

Join::Join() : Command(1) {}

bool  Join::checkArgs(std::vector<std::string>& cmdlist, Client& client)
{
    if (static_cast<int>(cmdlist.size() -1) < getRequiredArgsNumber())
    {
        makeWriteEvent(client.getUserSock(), Server::getInstance().getChangeList(), ERR_NEEDMOREPARAMS("client.getUserName()", mergeVec(cmdlist)));
        return (false);
    }
    return (true);
}

std::string getAllClientName(Channel *channel);

void Join::execute(std::vector<std::string>& cmdlist, Client& client, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
    std::vector<Channel*> targetChannel;

    if (checkArgs(cmdlist, client))
    {
        std::stringstream ss(cmdlist[1]);
        
        std::string token;
        //parse channel and process it
        while (getline(ss, token, ','))
        {
            if(token[0] != '#')
            {
                makeWriteEvent(client.getUserSock(), changelist, ERR_NOSUCHCHANNEL(client.getUserName(), token));
                continue;
            }
            token.erase(0, 1);
            if (channels->find(token)->second.findJoinClient(client.getNickname()) == true)
                return;
            if (channels->find(token) == channels->end())
            {
                channels->insert(std::make_pair(token, Channel(token, client.getUserSock())));
                targetChannel.push_back(&(channels->find(token)->second));
            }
            else
                targetChannel.push_back(&(channels->find(token)->second));
            targetChannel.back()->addClient(&client);
            client.getJoinedChannel().push_back(targetChannel.back());
        }
        //generate write events for all the clients
        for (std::vector<Channel*>::iterator it_chan = targetChannel.begin(); it_chan != targetChannel.end(); ++it_chan)
        {
            std::vector<Client*> existingClient = (*it_chan)->getClients();
            for (std::vector<Client*>::iterator it_cli = existingClient.begin(); it_cli != existingClient.end(); ++it_cli)
            {
				std::vector<std::string> reply = cmdlist;
				reply[1] = "#" + (*it_chan)->getChannelName();
                makeWriteEvent((*it_cli)->getUserSock(), changelist,  SUCCESS_REPL(client.getNickname(), mergeVec(reply)));
                if ((*it_cli)->getUserSock() == client.getUserSock())
                {                                              
                    makeWriteEvent((*it_cli)->getUserSock(), changelist, RPL_NAMEREPLY(client.getNickname(), (*it_chan)->getChannelName(), getAllClientName(*it_chan)));
                    makeWriteEvent((*it_cli)->getUserSock(), changelist, RPL_ENDOFNAMES(client.getNickname(), (*it_chan)->getChannelName()));
                }
            }
        }
    }
}

std::string getAllClientName(Channel *channel)
{
    std::string str;
    std::vector<Client*> clients = channel->getClients();

    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (channel->getOpFd() == (*it)->getUserSock())
            str += "@";
        str += (*it)->getNickname();
        str += " ";
    }
    return (str);
}
