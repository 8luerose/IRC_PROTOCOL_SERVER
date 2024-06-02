#include "../includes/Client.hpp"

Client::Client(int fd): _userSock(fd), _nickname("*"), _isPassed(false), _isRegistered(false), _isQuit(false){}

int	Client::getUserSock(void) const
{
	return (_userSock);
}

const std::string&	Client::getNickname(void) const
{
	return (_nickname);
}

const std::string&	Client::getUserName(void) const
{
	return (_userInfo[0]);
}

const std::string&	Client::getHostName(void) const
{
	return (_userInfo[1]);
}

std::vector<std::string>& Client::getUserInfo(void)
{
	return (_userInfo);
}

std::string& Client::getBuffer(void)
{
    return (_buffer);
}

std::vector<Channel *> & Client::getJoinedChannel(void){
	return (_joinedChannel);
}

void	Client::setNickname(const std::string& name)
{
	_nickname = name;
}

void	Client::setUserInfo(const std::string& string)
{
	_userInfo.push_back(string);
}

bool	Client::getIsRegistered(void){ return ( _isRegistered ); }
bool	Client::getIsPassed(void){ return ( _isPassed ); }

bool	Client::getIsQuit(void){ return ( _isQuit ); }
void	Client::setIsQuit(void){ _isQuit = true; }

void	Client::setIsRegistered(void){ _isRegistered = true; }
void	Client::setIsPassed(void){ _isPassed = true; }
