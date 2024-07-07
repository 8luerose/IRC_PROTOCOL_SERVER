#ifndef SERVER_HPP
#define SERVER_HPP
#define BUF_SIZE 1024
#define MAX_CONNECTION 5

#define READ 0
#define WRITE 1

#include "../includes/Main.hpp"

class	Client;
class	Channel;
class	Command;

class	Server
{
	private:
		Server();
		Server(const Server &);
		Server &operator=(const Server &);

		std::map<int, Client> 				_clientList;
		std::map<std::string, Channel *>	_channelList;
		struct sockaddr_in					_clientAddr;
		socklen_t							_clientAddrSize;
		Command								*_command;
		std::string							_password;
		unsigned short int					_portNum;
		struct sockaddr_in					_serverAddr;
		int									_serverSock;
		int									_kq;
		struct kevent						_eventList[256];
		struct kevent						*_curr_event;
		std::vector<struct kevent>			_changeList;

		void								kqueueInit();
		void								changeEvent(int ident, int flag, void *udata);
		void								setServerSock();
		void								setServerAddr();
		void								setServerBind();
		void								setServerListen();
		void								execute();
		int									recvMessage(int);
		void								addClient(int);
		bool								checkMessageEnds(int);
		void								doCommand(int);

		void								closeClient();
		void								disconnectClient(int client_fd);

	public:
		Server(int, std::string);
		~Server();

		void								run();

		std::map<int, Client>&				getClientList();
		std::string							getPassword();
		std::string							getMessage(int);
		std::map<std::string, Channel *>&	getChannelList();
		
		Channel*							findChannel(std::string);
		std::map<int, Client>::iterator		findClient(std::string);
		void								removeChannel(std::string);
		void								appendNewChannel(int, std::string &);

		class bindError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("bind error");
			}
		};
		class listenError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("listen error");
			}
		};
		class kqueueError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("kqueue error");
			}
		};
		class keventError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("kevent error");
			}
		};
		class acceptError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("accept error");
			}
		};
		class readError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("read error");
			}
		};
		class unknownError : public std::exception
		{
		public:
			virtual const char *what() const throw()
			{
				return ("unknown error");
			}
		};
};

#endif