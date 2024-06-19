// #include "../includes/Error.hpp"
#include "Error.hpp"

void ERROR_nosuchnick_401(Client &client, std::string nickname)
{
    client.appendReciveBuf("401 " + client.getNickname() + " " + nickname + " :" + ERR_NOSUCHNICK);
}

void ERROR_nosuchchannel_403(Client &client, std::string channel)
{
    client.appendReciveBuf("403 " + client.getNickname() + " " + channel + " :" + ERR_NOSUCHCHANNEL);
}

void ERROR_usernotinchannel_441(Client &client, std::string nickname, std::string channel)
{
    client.appendReciveBuf("441 " + client.getNickname() + " " + nickname + " " + channel + " :" + ERR_USERNOTINCHANNEL);
}

void ERROR_notonchannel_442(Client &client, std::string channel)
{
    client.appendReciveBuf("442 " + client.getNickname() + " " + channel + " :" + ERR_NOTONCHANNEL);
}

void ERROR_useronchannel_443(Client &client, std::string nickname, std::string channel)
{
    client.appendReciveBuf("443 " + client.getNickname() + " " + nickname + " " + channel + " :" + ERR_USERONCHANNEL);
}

void ERROR_nonicknamegiven_431(Client &client)
{
    client.appendReciveBuf("431 :" + std::string(ERR_NONICKNAMEGIVEN));
}

void ERROR_erroneusnickname_432(Client &client)
{
    client.appendReciveBuf("432 :" + std::string(ERR_ERRONEUSNICKNAME));
}

void ERROR_nicknameinuse_433(Client &client)
{
    client.appendReciveBuf("433 :" + std::string(ERR_NICKNAMEINUSE));
}

void ERROR_notregistered_451(Client &client)
{
    client.appendReciveBuf("451 :" + std::string(ERR_NOTREGISTERED));
}

void ERROR_needmoreparams_461(Client &client)
{
    client.appendReciveBuf("461 :" + std::string(ERR_NEEDMOREPARAMS));
}

void ERROR_alreadyregistred_462(Client &client)
{
    client.appendReciveBuf("462 :" + std::string(ERR_ALREADYREGISTRED));
}

void ERROR_passwdmismatch_464(Client &client)
{
    client.appendReciveBuf("464 :" + std::string(ERR_PASSWDMISMATCH));
}

void ERROR_channelisfull_471(Client &client, std::string channel)
{
    client.appendReciveBuf("471 " + client.getNickname() + " " + channel + " :" + ERR_CHANNELISFULL);
}

void ERROR_unknownmode_472(Client &client, char mode)
{
    client.appendReciveBuf("472 " + client.getNickname() + " " + mode + " :" + ERR_UNKNOWNMODE);
}

void ERROR_inviteonlychan_473(Client &client, std::string channel)
{
    client.appendReciveBuf("473 " + client.getNickname() + " " + channel + " :" + ERR_INVITEONLYCHAN);
}

void ERROR_badchannelkey_475(Client &client, std::string channel)
{
    client.appendReciveBuf("475 " + client.getNickname() + " " + channel + " :" + ERR_BADCHANNELKEY);
}

void ERROR_chanoprivsneeded_482(Client &client, std::string channel)
{
    client.appendReciveBuf("482 " + client.getNickname() + " " + channel + " :" + ERR_CHANOPRIVSNEEDED);
}
