#ifndef ERROR_HPP
# define ERROR_HPP

# include "../includes/Client.hpp"

// PASS
# define ERR_NEEDMOREPARAMS "Not enough parameters\r\n"
# define ERR_PASSWDMISMATCH "Password incorrect\r\n"
# define ERR_ALREADYREGISTRED "You may not reregister\r\n"

// NICK
# define ERR_NICKNAMEINUSE "Nickname is already in use\r\n"
# define ERR_NICKCOLLISION "Nickname collision KILL from\r\n"
# define ERR_NONICKNAMEGIVEN "No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME "Erroneus nickname\r\n"

// PRIVMSG and PART
# define ERR_NOSUCHCHANNEL "No such channel\r\n"
# define ERR_NOSUCHNICK "No such nick/channel\r\n"

// PART
# define ERR_NOTONCHANNEL "You're not on that channel\r\n"

// KICK
# define ERR_CHANOPRIVSNEEDED "You're not channel operator\r\n"
# define ERR_USERNOTINCHANNEL "They aren't on that channel\r\n"

// COMMON
# define ERR_NOTREGISTERED "You have not registered\r\n"

// MODE
# define ERR_UMODEUNKNOWNFLAG "Unknown MODE flag\r\n"
# define ERR_USERSDONTMATCH "Can't change mode for other users\r\n"
# define ERR_UNKNOWNMODE "is unknown mode char to me\r\n"
# define ERR_CHANNELISFULL "Cannot join channel (+l)\r\n"

// JOIN
# define ERR_INVITEONLYCHAN "Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY "Cannot join channel (+k)\r\n"

// TOPIC
# define RPL_NOTOPIC "No topic is set\r\n"

// INVITE
# define ERR_USERONCHANNEL "is already on channel\r\n"
# define RPL_INVITING "Inviting\r\n"
# define ERR_UNKNOWNCOMMAND "Unknown command\r\n"

class Client;

void ERROR_nosuchnick_401(Client &, std::string);
void ERROR_nosuchchannel_403(Client &, std::string);
void ERROR_usernotinchannel_441(Client &, std::string, std::string);
void ERROR_notonchannel_442(Client &, std::string);
void ERROR_useronchannel_443(Client &, std::string, std::string);
void ERROR_nonicknamegiven_431(Client &);
void ERROR_erroneusnickname_432(Client &);
void ERROR_nicknameinuse_433(Client &);
void ERROR_notregistered_451(Client &);
void ERROR_needmoreparams_461(Client &);
void ERROR_alreadyregistred_462(Client &);
void ERROR_passwdmismatch_464(Client &);
void ERROR_channelisfull_471(Client &, std::string);
void ERROR_unknownmode_472(Client &, char);
void ERROR_inviteonlychan_473(Client &, std::string);
void ERROR_badchannelkey_475(Client &, std::string);
void ERROR_chanoprivsneeded_482(Client &, std::string);
void ERROR_unknowncommand_421(Client &, std::string);

#endif
