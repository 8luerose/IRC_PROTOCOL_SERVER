#ifndef ERROR_HPP
#define ERROR_HPP

#include <algorithm>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <utility>
#include <string.h>
#include <iostream>
#include <sstream>

// PASS
#define ERR_NEEDMOREPARAMS "Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH "Password incorrect\r\n"
#define ERR_ALREADYREGISTRED "You may not reregister\r\n"

// NICK
#define ERR_NICKNAMEINUSE "Nickname is already in use\r\n"
#define ERR_NICKCOLLISION "Nickname collision KILL from\r\n"
#define ERR_NONICKNAMEGIVEN "No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME "Erroneus nickname\r\n"

// PRIVMSG and PART
#define ERR_NOSUCHCHANNEL "No such channel\r\n"
#define ERR_NOSUCHNICK "No such nick/channel\r\n"

// PART
#define ERR_NOTONCHANNEL "You're not on that channel\r\n"

// KICK
#define ERR_CHANOPRIVSNEEDED "You're not channel operator\r\n"
#define ERR_USERNOTINCHANNEL "They aren't on that channel\r\n"

// COMMON
#define ERR_NOTREGISTERED "You have not registered\r\n"

// MODE
#define ERR_UMODEUNKNOWNFLAG "Unknown MODE flag\r\n"
#define ERR_USERSDONTMATCH "Can't change mode for other users\r\n"
#define ERR_UNKNOWNMODE "is unknown mode char to me\r\n"
#define ERR_CHANNELISFULL "Cannot join channel (+l)\r\n"

// JOIN
#define ERR_INVITEONLYCHAN "Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY "Cannot join channel (+k)\r\n"

// TOPIC
#define RPL_NOTOPIC "No topic is set\r\n"

// INVITE
#define ERR_USERONCHANNEL "is already on channel\r\n"
#define RPL_INVITING "Inviting\r\n"

class Client;

// Exception base class
class IRCException : public std::exception
{
	private:
		std::string _message;

	public:
		IRCException(const std::string &message) : _message(message) {}
		virtual const char *what() const throw()
		{
			return (_message.c_str());
		}
};

/* nosuch error */
class NoSuchNickException : public IRCException
{
	public:
		NoSuchNickException(const std::string &nickname)
			: IRCException("401 :" + nickname + " :" + ERR_NOSUCHNICK) {}
};

class NoSuchChannelException : public IRCException
{
	public:
		NoSuchChannelException(const std::string &channel)
			: IRCException("403 :" + channel + " :" + ERR_NOSUCHCHANNEL) {}
};

/* channel error */
class UserNotInChannelException : public IRCException
{
	public:
		UserNotInChannelException(const std::string &nickname, const std::string &channel)
			: IRCException("441 :" + nickname + " " + channel + " :" + ERR_USERNOTINCHANNEL) {}
};

class NotOnChannelException : public IRCException
{
	public:
		NotOnChannelException(const std::string &channel)
			: IRCException("442 :" + channel + " :" + ERR_NOTONCHANNEL) {}
};

class UserOnChannelException : public IRCException
{
	public:
		UserOnChannelException(const std::string &nickname, const std::string &channel)
			: IRCException("443 :" + nickname + " " + channel + " :" + ERR_USERONCHANNEL) {}
};

/* 43* error */
class NoNicknameGivenException : public IRCException
{
	public:
		NoNicknameGivenException() : IRCException("431 :" + std::string(ERR_NONICKNAMEGIVEN)) {}
};

class ErroneousNicknameException : public IRCException
{
	public:
		ErroneousNicknameException() : IRCException("432 :" + std::string(ERR_ERRONEUSNICKNAME)) {}
};

class NicknameInUseException : public IRCException
{
	public:
		NicknameInUseException() : IRCException("433 :" + std::string(ERR_NICKNAMEINUSE)) {}
};

/* 45* error */
class NotRegisteredException : public IRCException
{
	public:
		NotRegisteredException() : IRCException("451 :" + std::string(ERR_NOTREGISTERED)) {}
};

/* 46* error */
class NeedMoreParamsException : public IRCException
{
	public:
		NeedMoreParamsException() : IRCException("461 :" + std::string(ERR_NEEDMOREPARAMS)) {}
};

class AlreadyRegisteredException : public IRCException
{
	public:
		AlreadyRegisteredException() : IRCException("462 :" + std::string(ERR_ALREADYREGISTRED)) {}
};

class PasswdMismatchException : public IRCException
{
	public:
		PasswdMismatchException() : IRCException("464 :" + std::string(ERR_PASSWDMISMATCH)) {}
};

/* mode 47* error */
class ChannelIsFullException : public IRCException
{
	public:
		ChannelIsFullException(const std::string &channel)
			: IRCException("471 :" + channel + " :" + ERR_CHANNELISFULL) {}
};

class UnknownModeException : public IRCException
{
	public:
		UnknownModeException(char mode)
			: IRCException("472 :" + std::string(1, mode) + " :" + ERR_UNKNOWNMODE) {}
};

class InviteOnlyChanException : public IRCException
{
	public:
		InviteOnlyChanException(const std::string &channel)
			: IRCException("473 :" + channel + " :" + ERR_INVITEONLYCHAN) {}
};

class BadChannelKeyException : public IRCException
{
	public:
		BadChannelKeyException(const std::string &channel)
			: IRCException("475 :" + channel + " :" + ERR_BADCHANNELKEY) {}
};

/* 48* error */
class ChanOpPrivsNeededException : public IRCException
{
	public:
		ChanOpPrivsNeededException(const std::string &channel)
			: IRCException("482 :" + channel + " :" + ERR_CHANOPRIVSNEEDED) {}
};

#endif
