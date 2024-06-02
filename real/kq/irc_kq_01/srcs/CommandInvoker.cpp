#include "../includes/CommandInvoker.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include "../includes/Nick.hpp"
#include "../includes/Pass.hpp"
#include "../includes/Join.hpp"
#include "../includes/Part.hpp"
#include "../includes/Pong.hpp"
#include "../includes/Kick.hpp"
#include "../includes/Privmsg.hpp"
#include "../includes/Notice.hpp"
#include "../includes/Quit.hpp"

CommandInvoker::CommandInvoker()
{
  _commandMap.insert(std::make_pair("NICK", new Nick()));
  _commandMap.insert(std::make_pair("USER", new User()));
  _commandMap.insert(std::make_pair("PASS", new Pass()));
   _commandMap.insert(std::make_pair("PING", new Pong()));	//
   _commandMap.insert(std::make_pair("JOIN", new Join()));
  _commandMap.insert(std::make_pair("PART", new Part()));
  _commandMap.insert(std::make_pair("KICK", new Kick()));
  _commandMap.insert(std::make_pair("QUIT", new Quit()));
   _commandMap.insert(std::make_pair("PRIVMSG", new Privmsg()));
   _commandMap.insert(std::make_pair("NOTICE", new Notice()));
}

void	CommandInvoker::executeCommand(std::vector<std::string> &cmdline, int ident, std::vector<struct kevent>& changelist, std::map<std::string, Channel>* channels)
{
	Server& server = Server::getInstance();
	Client &client = server.getClients().find(ident)->second;
	if (_commandMap.find(cmdline[0]) == _commandMap.end())
	{
		_commandMap["nick"]->makeWriteEvent(ident, changelist, ERR_UNKNOWNCOMMAND(client.getNickname(), cmdline[0]));
		return ;
	}
	if (client.getIsPassed() == false)
	{
		if (cmdline[0] != "PASS")
		{
			_commandMap["pass"]->makeWriteEvent(ident, changelist, ERR_NOTREGISTERED(client.getNickname(), cmdline[0]));
			return ;
		}
	}
	else if (client.getIsRegistered() == false)
	{
		if (cmdline[0] != "NICK" && cmdline[0] != "USER")
		{
			_commandMap["pass"]->makeWriteEvent(ident, changelist, ERR_NOTREGISTERED(client.getNickname(), cmdline[0]));
			return ;
		}
	}
	Command *command = _commandMap.find(cmdline[0])->second;
	command->execute(cmdline, client, changelist, channels);	//
}


void  CommandInvoker::parseString(const std::string& raw, std::vector<std::string>& cmds)
{
  size_t  idx = 0;
  size_t  len = 0;
  size_t  end = raw.length();

  cmds.clear();
  while (idx < end)
  {
    while (raw[idx + len] != '\n' && raw[idx + len] != '\r' && raw[idx + len] != '\0')
      len++;
    if (len > 0)
      cmds.push_back(raw.substr(idx, len));
    idx += (len + 1);
    len = 0;
  }
}

void  CommandInvoker::parseLine(const std::string& msg, std::vector<std::string>& cmdline)
{
  size_t idx = 0;
  size_t len = 0;
  size_t end = msg.length();

  cmdline.clear();
  while (idx < end)
  {
    while (msg[idx + len] != ' ' && msg[idx + len] != ':' && msg[idx + len] != '\0')
      len++;
    if (len > 0)
      cmdline.push_back(msg.substr(idx, len));
    if (msg[idx + len] == ':')
    {
      cmdline.push_back(msg.substr(idx + len));
      break ;
    }
    idx += (len + 1);
    len = 0;
  }
}

void CommandInvoker::commandConnector(int ident, const std::string& message)
{
  Server& server = Server::getInstance();
  std::vector<std::string> commands;
  std::vector<std::string> cmdline;

  parseString(message, commands);
  std::vector<std::string>::iterator  it;
  for (it = commands.begin(); it < commands.end(); it++)
  {
	if (*it == "CAP LS" || isStartWith(*it, "MODE"))
		continue ;
	std::cout << B << *it << N << std::endl;
    parseLine(*it, cmdline);
    executeCommand(cmdline, ident, server.getChangeList(), &server.getChannels());
	if (cmdline[0] == "PASS" && cmdline[1] != std::string(server.getPswd()))
		return ;
  }
}

bool  CommandInvoker::isStartWith(const std::string& target, const std::string& ref) const
{
	int	i;
	int	len;

	len = ref.size();
	for (i = 0; i < len + 1; i++)
	{
		if (target[i] != ref[i])
			return (false);
	}
	return (true);
}
