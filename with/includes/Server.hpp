#ifndef SERVER_HPP
#define SERVER_HPP

#include "Command.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define BUF_SIZE 1024
#define MAX_CONNECTION 15

#define READ 0
#define WRITE 1

class	Client;
class	Channel;
class	Command;

class	Server
{
private:
	/* OCCF */
	Server();
	Server(const Server &);
	Server &operator=(const Server &);

	// /* member variables */
	// server socket
	std::map<int, Client> 		_clientList;		// 태현
	std::map<std::string, Channel> _channelList;	// 태현
	struct sockaddr_in			_clientAddr;
	socklen_t					_clientAddrSize;
	Command						_command;
	int							_eventCnt;
	std::string					_password;
	unsigned short int			_portNum;
	struct sockaddr_in			_serverAddr;
	int							_serverSock;
	int							_kq;
	struct kevent				_eventList[256];
	struct kevent				*_curr_event;
	std::vector<struct kevent>	_changeList;

	void kqueueInit();
	void changeEvent(int ident, int flag, void *udata);
	void setServerSock();
	void setServerAddr();
	void setServerBind();
	void setServerListen();
	void setBot();
	// others
	void execute();
	int recvMessage(int);
	void addClient(int);
	bool checkMessageEnds(int);
	void doCommand(int);

public:
	/* OCCF */
	Server(int, std::string);
	~Server();

	/* member variables */
	Client *_bot;

	/* member functions */
	// getter
	std::map<int, Client> &getClientList();		// 태현
	std::string getPassword();
	std::string getMessage(int);
	std::map<std::string, Channel> &getChannelList();
	// others
	void run();
	// Channel findChannel(std::string);
	Channel* findChannel(std::string);
	std::map<int, Client>::iterator findClient(std::string);
	void removeChannel(std::string);
	void appendNewChannel(std::string &, int);


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
		virtual const char *what(std::string& msg) const throw()
		{
			return (msg.c_str());
		}
	};
};

#endif