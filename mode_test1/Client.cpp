#include "Client.hpp"

// 기본 생성자 추가
Client::Client() : fd(-1), isRegiPass(false), isRegiNick(false), isRegiUser(false) {
}

// 기존 생성자
Client::Client(int fd) : fd(fd), isRegiPass(false), isRegiNick(false), isRegiUser(false) {
}

// 복사 생성자
Client::Client(const Client& other)
    : fd(other.fd), channelList(other.channelList), nickname(other.nickname),
      username(other.username), hostname(other.hostname), servername(other.servername),
      realname(other.realname), receiveBuf(other.receiveBuf), isRegiPass(other.isRegiPass),
      isRegiNick(other.isRegiNick), isRegiUser(other.isRegiUser) {
}

// 복사 할당 연산자
Client& Client::operator=(const Client& other) {
    if (this != &other) {
        fd = other.fd;
        channelList = other.channelList;
        nickname = other.nickname;
        username = other.username;
        hostname = other.hostname;
        servername = other.servername;
        realname = other.realname;
        receiveBuf = other.receiveBuf;
        isRegiPass = other.isRegiPass;
        isRegiNick = other.isRegiNick;
        isRegiUser = other.isRegiUser;
    }
    return *this;
}

// 소멸자
Client::~Client() {
}

// getter 메서드들
int Client::getClientFd() {
    return fd;
}

std::vector<std::string>& Client::getChannelList() {
    return channelList;
}

std::string Client::getNickname() {
    return nickname;
}

std::string Client::getUsername() {
    return username;
}

std::string Client::getHostname() {
    return hostname;
}

std::string Client::getServername() {
    return servername;
}

std::string Client::getRealname() {
    return realname;
}

std::string Client::getReciveBuf() {
    return receiveBuf;
}

bool Client::getIsRegist() {
    return isRegiPass && isRegiNick && isRegiUser;
}

bool Client::getRegiPass() {
    return isRegiPass;
}

bool Client::getRegiNick() {
    return isRegiNick;
}

bool Client::getRegiUser() {
    return isRegiUser;
}

// setter 메서드들
void Client::setRegiPass(bool status) {
    isRegiPass = status;
}

void Client::setRegiNick(bool status) {
    isRegiNick = status;
}

void Client::setRegiUser(bool status) {
    isRegiUser = status;
}

void Client::setRegiAll(bool status) {
    isRegiPass = status;
    isRegiNick = status;
    isRegiUser = status;
}

void Client::setNickname(std::string setName) {
    nickname = setName;
}

void Client::setUserinfo(std::string inputUserName, std::string inputHostName, std::string inputServerName, std::string inputRealName) {
    username = inputUserName;
    hostname = inputHostName;
    servername = inputServerName;
    realname = inputRealName;
}

void Client::appendReciveBuf(std::string inputBuffer) {
    receiveBuf.append(inputBuffer);
}

void Client::appendChannelList(std::string channelName) {
    channelList.push_back(channelName);
}

// clear 메서드들
void Client::clearReciveBuf() {
    receiveBuf.clear();
}

void Client::clearAllChannel() {
    channelList.clear();
}

void Client::resetClient() {
    fd = -1;
    channelList.clear();
    nickname.clear();
    username.clear();
    hostname.clear();
    servername.clear();
    realname.clear();
    receiveBuf.clear();
    isRegiPass = false;
    isRegiNick = false;
    isRegiUser = false;
}

void Client::deleteChannel(std::string delChannel) {
    std::vector<std::string>::iterator iter = findChannel(delChannel);
    if (iter != channelList.end()) {
        channelList.erase(iter);
    }
}

std::vector<std::string>::iterator Client::findChannel(std::string targetChannel) {
    std::vector<std::string>::iterator iter = channelList.begin();
    while (iter != channelList.end()) {
        if (*iter == targetChannel) {
            return iter;
        }
        iter++;
    }
    return channelList.end();
}
