#ifndef BOT_HPP
#define BOT_HPP

// socket
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// c++
#include <iostream>
#include <vector>
#include <string>

// date
#include <time.h>

// for print
# define N "\033[0m"
# define R "\033[31m"
# define G "\033[32m"
# define Y "\033[33m" 
# define B "\033[34m" 
# define PRINT_LOG(fd, str, color) std::cout << color <<  str << "[" << fd << "]" << N << std::endl;
# define PRINT_MSG(fd, str, buffer, color) std::cout << color <<  str << "[" << fd << "]: " << buffer << N;



class Bot {
private :
  char * _server_ip;
  char * _server_pass;
  int _server_port;
  bool pswd;
  std::string _target;

  int _my_sock;
  sockaddr_in _server_addr;

  // socket init and socket connect
  int socket_init();
  
  // connect to server and register server
  int server_welcome();
	
  std::string watching();
  std::string thinking(std::string watched_msg);
  std::string make_sense(std::string bot_cmd);
  
  void answering(std::string answer);

	void error_handle(int ret, const char *noti);

public :
  Bot(char *ip, int port);
  Bot(char *ip, int port, char *pass);
  ~Bot();
  void wingChicken();
 };

#endif