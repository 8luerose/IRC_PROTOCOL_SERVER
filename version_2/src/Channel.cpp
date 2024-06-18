#include "../includes/Channel.hpp"

Channel::Channel(const std::string &channelName, int fd) 
{
    _channelName = channelName;
    _inviteStatus = false;
    _topicStatus = false;
    _keyStatus = false;
    _limitStatus = false;
	_limit = 0;

    _operatorFdList.push_back(fd);	// operator는 channel을 처음 만든 client
}

Channel::Channel() 
{
	_channelName = "";
	_inviteStatus = false;
	_topicStatus = false;
	_keyStatus = false;
	_limitStatus = false;
	_limit = 0;
}

Channel::Channel(const Channel &other)
{
	(*this) = other;
}

Channel &Channel::operator=(const Channel &other)
{
	_channelName = other._channelName;
	_operatorFdList = other._operatorFdList;
	_clientFdList = other._clientFdList;
	_inviteFdList = other._inviteFdList;
	_topic = other._topic;
	_key = other._key;
	_inviteStatus = other._inviteStatus;
	_topicStatus = other._topicStatus;
	_keyStatus = other._keyStatus;
	_limitStatus = other._limitStatus;
	_limit = other._limit;

	return (*this);
}

Channel::~Channel()
{
}


/* getters */
std::string Channel::getChannelName()
{
    return (_channelName);
}

std::vector<int> Channel::getFdListClient()
{
    return (_clientFdList);
}

std::vector<int> Channel::getFdListOperator()
{
    return (_operatorFdList);
}

std::string Channel::getMode()
{
    std::string mode;

    if (_topicStatus == true)
        mode += "t";
    if (_inviteStatus == true)
        mode += "i";
    if (_limitStatus == true)
        mode += "l";
    if (_keyStatus == true)
        mode += "k";

    return (mode);
}

unsigned int Channel::getLimit()
{
    return (_limit);
}

std::string Channel::getKey()
{
	return (_key);
}

std::string Channel::getTopic()
{
    return (_topic);
}

std::vector<int>::iterator Channel::findIterClient(int fd)
{	// fd가 clientList에 있는지 확인
	std::vector<int>::iterator iter;

	iter = std::find(_clientFdList.begin(), _clientFdList.end(), fd);
	if (iter != _clientFdList.end())
		return (iter);
	return (_clientFdList.end());
}


/* setters */
void Channel::setChannelName(std::string &channelName)
{
    _channelName = channelName;
}

void Channel::setMode(unsigned char mode, char sign, int fd)
{
	if (mode == 'i')
	{
		if (sign == '+')
			_inviteStatus = true;
		else if (sign == '-')
			_inviteStatus = false;
	}
	else if (mode == 't')
	{
		if (sign == '+')
			_topicStatus = true;
		else if (sign == '-')
			_topicStatus = false;
	}
	else if (mode == 'l')
	{
		if (sign == '+')
			_limitStatus = true;
		else if (sign == '-')
			_limitStatus = false;
	}
	else if (mode == 'k')
	{
		if (sign == '+')
			_keyStatus = true;
		else if (sign == '-')
			_keyStatus = false;
	}
	else if (mode == 'o')
    {
        setOperator(fd, sign);
    }
}

void Channel::setOperator(int fd, char sign)
{
    if (sign == '+')
    {
        addOperatorFd(fd);
    }
    else if (sign == '-')
    {
        removeOperatorFd(fd);
    }
}

void Channel::setLimit(unsigned int limit)
{
    _limit = limit;
}

void Channel::setTopic(std::string topic)
{
    _topic = topic;
}

void Channel::setKey(std::string key)
{
    _key = key;
}

void Channel::appendFdListClient(int fd)
{
    _clientFdList.push_back(fd);
}

void Channel::removeFdListClient(int fd)
{
    std::vector<int>::iterator iter;
	
	iter = findIterClient(fd);
    if (iter != _clientFdList.end())
        _clientFdList.erase(iter);
}

void Channel::addOperatorFd(int fd)
{
	std::vector<int>::iterator iter;

	iter = std::find(_operatorFdList.begin(), _operatorFdList.end(), fd);
	if (iter == _operatorFdList.end())	// op fd가 list안에 없으면 end()를 반환 -> 추가
		_operatorFdList.push_back(fd);
}

void Channel::removeOperatorFd(int fd)
{
    std::vector<int>::iterator iter;
	
	iter = std::find(_operatorFdList.begin(), _operatorFdList.end(), fd);
    if (iter != _operatorFdList.end())
        _operatorFdList.erase(iter);
}

void Channel::appendFdListInvite(int fd)
{
	std::vector<int>::iterator iter;

	iter = std::find(_inviteFdList.begin(), _inviteFdList.end(), fd);
	if (iter == _inviteFdList.end())	// invite fd가 list안에 없으면 end()를 반환 -> 추가
		_inviteFdList.push_back(fd);
}

// diff
bool Channel::diffInvite(int fd)
{
	std::vector<int>::iterator iter;

	iter = std::find(_inviteFdList.begin(), _inviteFdList.end(), fd);	// fd가 inviteList에 있는지 확인
	if (iter != _inviteFdList.end())
		return (true);
	return (false);
}

bool Channel::diffKey(std::string key)
{
    if (_key == key)
        return (true);
    return (false);
}

bool Channel::diffOperator(int fd)
{
	std::vector<int>::iterator iter;

	iter = std::find(_operatorFdList.begin(), _operatorFdList.end(), fd);	// fd가 operator인지 확인
	if (iter != _operatorFdList.end())
		return (true);
	return (false);
}

bool Channel::diffMode(unsigned char mode)
{
    if (mode == 'i')
        return (_inviteStatus);	// invite 상태를 반환. 예를 들어 +i였으면 true, -i였으면 false
    else if (mode == 't')
        return (_topicStatus);
    else if (mode == 'l')
        return (_limitStatus);
    else if (mode == 'k')
        return (_keyStatus);

	return (false);	// error: i, t, l, k가 아닌 경우가 들어왔을 때
}

bool Channel::diffClientInChannel(int fd)
{	// fd가 clientList에 있는지/없는지'만' 확인 -> 있으면 true, 없으면 false
    std::vector<int>::iterator iter;
	
	iter = findIterClient(fd);
    if (iter != _clientFdList.end())	
        return (true);
    return (false);
}
