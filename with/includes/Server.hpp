#ifndef SERVER_HPP
#define SERVER_HPP

#include "Main.hpp"
#include "Command.hpp"
#include "Client.hpp"

#define BUF_SIZE 1024
#define MAX_CONNECTION 15

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
	// // server socket
	struct sockaddr_in	_clientAddr;
	socklen_t			_clientAddrSize;
	Command				_command;
	int					_eventCnt;
	std::string			_password;
	unsigned short int	_portNum;
	struct sockaddr_in	_serverAddr;
	int					_serverSock;
	int					_kq;
	struct kevent		_eventList[256];
	struct kevent		_curr_event;
	std::vector<struct kevent>	_changeList;

	void kqueueInit();
	void inputEvent(std::vector<struct kevent> &, uint);
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
	std::map<int, Client> &getClients();
	std::string getPassword();
	std::string getMessage(int);
	std::map<std::string, Channel *> &getChannelList();
	// others
	void run();
	Channel *findChannel(std::string);
	std::map<int, Client>::iterator findClient(std::string);
	void removeChannel(std::string);
	void appendNewChannel(std::string &, int);
};

#endif