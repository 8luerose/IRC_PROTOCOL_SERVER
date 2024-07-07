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

		bool						_isRegiPass;
		bool						_isRegiNick;
		bool						_isRegiUser;

		std::stringstream			_eofBuffer;
		bool						_eofFlag;

	
	public:
		Client();
		Client(int);
		Client(const Client &);
		Client &operator=(const Client &);
		~Client();
		std::vector<std::string>::iterator	findChannel(std::string);

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
		std::stringstream			&getEofBuffer();
		bool						getEofFlag();

		void						setRegiPass(bool);
		void						setRegiNick(bool);
		void						setRegiUser(bool);
		void						setRegiAll(bool);
		void						setNickname(std::string);
		void						setUserinfo(std::string, std::string, std::string, std::string);
		void						appendReciveBuf(std::string);
		void						appendChannelList(std::string);
		void						setEofBuffer(std::stringstream&);
		void						setEofFlag(bool);

		void						clearReciveBuf();
		void						clearAllChannel();
		void						resetClient();
		void						deleteChannel(std::string);
};

#endif