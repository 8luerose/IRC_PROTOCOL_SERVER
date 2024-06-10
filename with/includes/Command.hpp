#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "../includes/Main.hpp"
# include "../includes/Client.hpp"
# include "../includes/Server.hpp"
# include "../includes/Util.hpp"
# include "../includes/Error.hpp"

class Client;
class Server;
class Channel;

class Command
{

	private:
		/* OCCF */
		Command();
		Command &operator=(const Command&);
		Command(const Command&);

		/* member variables */
		Server		&_server;

	public:
		/* OCCF */
		Command(Server &server);
		~Command();

		void		run(int);
		void		signUp(int, std::map<int, Client>::iterator, std::vector<std::string>&, std::map<int, Client>&);
		void		notRegister(int, std::map<int, Client>::iterator, std::map<int, Client>&);
		void		signIn(int, std::vector<std::string>&);
		void		pass(int, std::vector<std::string>);
		void		nick(int, std::vector<std::string>);
		void		user(int, std::vector<std::string>);
		void		ping(int, std::vector<std::string>);
		void		privmsg(int, std::vector<std::string>);
		void		quit(int, std::vector<std::string>);
		void		part(int, std::vector<std::string>);
		void		join(int, std::vector<std::string>);
		void		kick(int, std::vector<std::string>);
		void		mode(int, std::vector<std::string>);
		void		topic(int, std::vector<std::string>);
		void		invite(int, std::vector<std::string>);
		// utils in commands/
		void		botCommand(int, std::vector<std::string>);
		void		topicMsg(int, std::string);
		bool		checkNicknameDuplicate(std::string, std::map<int, Client>&);
		bool		checkNicknameValidate(std::string);
		bool		checkRealname(std::string);
		bool		checkBotCommand(std::string);
		// utils in Util.cpp
		std::string	channelMessage(int, std::vector<std::string>);
		void		channelPRIVMSG(std::string, Client&, Channel*);
		void		channelPART(int, std::string, std::vector<std::string>);
		void		msgToAllChannel(int, std::string, std::string, std::string);
		std::string	makeFullName(int);
		void		nameListMsg(int, std::string);
};

#endif