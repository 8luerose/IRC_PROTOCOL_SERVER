#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <iostream>
#include <vector>

class Channel;
#include "Channel.hpp"

class Client
{
        const int _userSock;
        std::string _nickname;
		//<username> <hostname> <servername> <realname>
        std::vector<std::string> _userInfo;
        std::string _buffer;
		bool	_isPassed;
		bool	_isRegistered;
        bool    _isQuit;
        std::vector<Channel*> _joinedChannel; //
    public:
        Client(int fd);

        int getUserSock(void) const;
        const std::string& getNickname(void) const;
        const std::string& getUserName(void) const;
        const std::string& getHostName(void) const;
		std::vector<std::string>& getUserInfo(void);
        // const std::string& getServerName(void) const;
        // const std::string& getRealName(void) const;
        std::string& getBuffer(void);
		std::vector<Channel*>& getJoinedChannel(void);
		bool getIsRegistered();
		bool getIsPassed();
        bool getIsQuit();

        void setNickname(const std::string& name);
		void setUserInfo(const std::string& string);
		void setIsRegistered();
		void setIsPassed();
        void setIsQuit();
        
};

#endif
