#include "../includes/Client.hpp"

// OCCF
Client::Client(int fd)
{
	_fd = fd;
	_nickName = "Client";
	_isRegiPass = false;
	_isRegiNick = false;
	_isRegiUser = false;
	_reciveBuf = "";		// 석준 추가
}
// 석준 주석
// Client::Client()
// {
// 	_fd = -1;
// 	_nickName = "Client";
// 	_isRegiPass = false;
// 	_isRegiNick = false;
// 	_isRegiUser = false;
// }

Client::Client(const Client &ref)
{
	_fd = ref._fd;
	_nickName = ref._nickName;
	_userName = ref._userName;
	_hostName = ref._hostName;
	_serverName = ref._serverName;
	_realName = ref._realName;
	_reciveBuf = ref._reciveBuf;
	_isRegiPass = ref._isRegiPass;
	_isRegiNick = ref._isRegiNick;
	_isRegiUser = ref._isRegiUser;
	_channelList = ref._channelList;
}

Client::~Client()
{
}

// getter
int	Client::getClientFd()
{
	return (_fd);
}

std::vector<std::string> &Client::getChannelList()
{
	return (_channelList);
}

std::string Client::getNickname()
{
	return (_nickName);
}

std::string Client::getUsername()
{
	return (_userName);
}

std::string Client::getHostname()
{
	return (_hostName);
}

std::string Client::getServername()
{
	return (_serverName);
}

std::string Client::getRealname()
{
	return (_realName);
}

std::string Client::getReciveBuf()
{
	return (_reciveBuf);
}

bool Client::getIsRegist()
{
	if (_isRegiPass && _isRegiNick && _isRegiUser)
		return (true);
	return (false);
}

bool Client::getRegiPass()
{
	return (_isRegiPass);
}

bool Client::getRegiNick()
{
	return (_isRegiNick);
}

bool Client::getRegiUser()
{
	return (_isRegiUser);
}

// setter
void Client::setRegiPass(bool status)
{
	_isRegiPass = status;
}

void Client::setRegiNick(bool status)
{
	_isRegiNick = status;
}

void Client::setRegiUser(bool status)
{
	_isRegiUser = status;
}

void Client::setRegiAll(bool status)
{
	_isRegiPass = status;
	_isRegiNick = status;
	_isRegiUser = status;
}

void Client::setNickname(std::string setName)
{
	_nickName = setName;
}

void Client::setUserinfo(std::string inputUserName, std::string inputHostName, std::string inputServerName, std::string inputRealName)
{
	_userName = inputUserName;
	_hostName = inputHostName;
	_serverName = inputServerName;
	_realName = inputRealName;
}

void Client::appendReciveBuf(std::string inputBuffer)
{
	_reciveBuf.append(inputBuffer);
}

void Client::appendChannelList(std::string channelName)
{
	_channelList.push_back(channelName);
}

// clear
void Client::clearReciveBuf()
{
	_reciveBuf.clear();
}

void Client::clearAllChannel()
{
	_channelList.clear();
}

void Client::resetClient()
{	// 클라이언트 정보 초기화 -> 다른 클라이언트가 또 쓸 수 있게
	setRegiAll(false);
	clearAllChannel();
	clearReciveBuf();
	_fd = -1;
	_nickName = "";
	_userName = "";
	_hostName = "";
	_serverName = "";
	_realName = "";
}

void Client::deleteChannel(std::string delChannel)
{
	std::vector<std::string>::iterator iter;

	iter = findChannel(delChannel);
	if (iter != _channelList.end())
	{
		_channelList.erase(iter);		// vector의 '특정 위치'에 있는 원소를 삭제
	}
}

std::vector<std::string>::iterator Client::findChannel(std::string targetChannel)
{	// "/JOIN #channel" -> 채널은 string으로 등록
	std::vector<std::string>::iterator iter;

	iter = _channelList.begin();
	while (iter != _channelList.end())
	{
		if (*(iter) == targetChannel)	// vector의 특정 위치에 *을 붙여야 실제 string 값을 가져올 수 있음
			return (iter);
		iter++;
	}
	return (_channelList.end());		// _channelList.end()는 '벡터의 마지막 원소 다음 위치'를 가리키는 반복자
}
