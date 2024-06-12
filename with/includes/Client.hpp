#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"

class Client
{
	private:
		int							_fd;
		std::vector<std::string>	_channelList;

		std::string					_reciveBuf;
		std::string					_nickName;
		std::string					_userName;
		std::string					_realName;
		std::string					_hostName;
		std::string					_serverName;

		// Regist status
		bool						_isRegiPass;
		bool						_isRegiNick;
		bool						_isRegiUser;

		// OCCF
		Client();
	
	public:
		Client(int);
		Client(const Client &);
		Client &operator=(const Client &);
		~Client();
		std::vector<std::string>::iterator	findChannel(std::string);

		// getter
		int							getClientFd();
		std::vector<std::string>	&getChannelList();
		std::string					getNickname();
		std::string					getUsername();
		std::string					getHostname();
		std::string					getServername();
		std::string					getRealname();
		std::string					getReciveBuf();
		bool						getIsRegist();
		bool						getRegiPass();
		bool						getRegiNick();
		bool						getRegiUser();

		// setter
		void						setRegiPass(bool);
		void						setRegiNick(bool);
		void						setRegiUser(bool);
		void						setRegiAll(bool);
		void						setNickname(std::string);
		void						setUserinfo(std::string, std::string, std::string, std::string);
		void						appendReciveBuf(std::string);
		void						appendChannel(std::string);

		// clear
		void						clearReciveBuf();
		void						clearAllChannel();
		void						resetClient();
		void						deleteChannel(std::string);
};

#endif