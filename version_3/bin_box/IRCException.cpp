#include "../includes/IRCException.hpp"

/* nosuch error */
void err_nosuchnick_401(Client &client, std::string nickname)
{
	throw NoSuchNickException(nickname);
}

void err_nosuchchannel_403(Client &client, std::string channel)
{
	throw NoSuchChannelException(channel);
}

/* channel error */
void err_usernotinchannel_441(Client &client, std::string nickname, std::string channel)
{
	throw UserNotInChannelException(nickname, channel);
}

void err_notonchannel_442(Client &client, std::string channel)
{
	throw NotOnChannelException(channel);
}

void err_useronchannel_443(Client &client, std::string nickname, std::string channel)
{
	throw UserOnChannelException(nickname, channel);
}

/* 43* error */
void err_nonicknamegiven_431()
{
	throw NoNicknameGivenException();
}

void err_erroneusnickname_432()
{
	throw ErroneousNicknameException();
}

void err_nicknameinuse_433()
{
	throw NicknameInUseException();
}

/* 45* error */
void err_notregistered_451()
{
	throw NotRegisteredException();
}

/* 46* error */
void err_needmoreparams_461()
{
	throw NeedMoreParamsException();
}

void err_alreadyregistred_462()
{
	throw AlreadyRegisteredException();
}

void err_passwdmismatch_464()
{
	throw PasswdMismatchException();
}

/* mode 47* error */
void err_channelisfull_471(std::string channel)
{
	throw ChannelIsFullException(channel);
}

void err_unknownmode_472(char mode)
{
	throw UnknownModeException(mode);
}

void err_inviteonlychan_473(std::string channel)
{
	throw InviteOnlyChanException(channel);
}

void err_badchannelkey_475(std::string channel)
{
	throw BadChannelKeyException(channel);
}

/* 48* error */
void err_chanoprivsneeded_482(std::string channel)
{
	throw ChanOpPrivsNeededException(channel);
}
