#include "Bot.hpp"

// UTILS
std::vector<std::string> split_msg(std::string &line, std::string s){
	std::vector<std::string> tab;
	std::string cmd_buf;
	size_t start = 0;
	size_t pos;

	while ((pos = line.find(s)) != std::string::npos){
		tab.push_back(line.substr(start, pos));
		if (tab.size() == 3) // 이미 {발신인, 명령어, 타겟} 까지 다 담겨있으면, 마지막은 메세지스트링
			break;
		else
			line = line.substr(pos + s.length());
	}
	// 마지막 벡터는 메세지라 ':'가 붙어있어 제거
	if (line != "")
		tab.push_back(line.substr(pos + s.length() + 1));
	return tab;
}

Bot::Bot(char *ip, int port){
	_server_ip = ip;
	_server_port = port;
	pswd = false;
}

Bot::Bot(char *ip, int port, char *pass){
	_server_ip = ip;
	_server_port = port;
	pswd = true;
	_server_pass = pass;
}

Bot::~Bot(){
	close(_my_sock);
}

void Bot::wingChicken(){
	std::string watching_ret;
	std::string thinking_ret;

	error_handle(socket_init(), "failed to create socket");
	error_handle(connect(_my_sock, reinterpret_cast<sockaddr*>(&_server_addr), sizeof(_server_addr)), "faid to connect socket");
	error_handle(server_welcome(), "failed to enter to server");
	while (true){
		watching_ret = watching();
		thinking_ret = thinking(watching_ret);
		answering(thinking_ret); // must be null guard
	}
}

int Bot::socket_init(){
	_my_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (_my_sock == -1){
		return -1;
	}
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = inet_addr(_server_ip);
	_server_addr.sin_port = htons(_server_port);
	return 0;
}

int Bot::server_welcome(){
	int result;
	
	std::string welcome;
	std::string nickuser = std::string("NICK hnrbot\r\nUSER bot ircbot tutle :laptop\r\n");
	if (pswd)
	{
		std::string pass = std::string("PASS ");
		pass += std::string(_server_pass);
		pass += "\r\n";
		welcome = pass + nickuser;
	}
	else
		welcome = nickuser;
	const char *buf; 
	buf = welcome.c_str();
	result = send(_my_sock, buf, strlen(buf), 0);
	if (result == -1) {
		perror("can't send to server");
		return 1;
	}
	PRINT_LOG("server_welcome", "connect to server", Y);
	return 0;
}

std::string Bot::watching(){
	PRINT_LOG("watching", ".......", Y);
	
	std::string watched;
	char recv_buf[1024];
	memset(recv_buf, 0, 1024);
	int received_bytes = recv(_my_sock, recv_buf, sizeof(recv_buf), 0); //recv blocking(?)
	if (received_bytes <= 0) {
		// maybe server off
		PRINT_LOG("failed to connect server", "ERROR", R);
		exit(1);
	}
	PRINT_MSG("RECEVIE", "BOT", recv_buf, G);
	watched = std::string(recv_buf);
	return watched;
}

std::string Bot::thinking(std::string watched_msg){
	size_t pos;
	int ret = 0;
	std::vector<std::string> v = split_msg(watched_msg, " ");
	if (v.size() == 2){ // PING
		return std::string("PONG :laptap makes turtle...\r\n");
	}
	else if (v.size() == 4){ // PRIVMSG
		_target = v[2]; // member variable set
		if ((pos = v[1].find("PRIVMSG")) == std::string::npos)
			return std::string("");
		// DM일 때는 메세지로 보내준 채널로 접속
		if (v[2] == "hnrbot"){
			if (v[3].find('#') != std::string::npos){ // 귓말로 채널 보낼 때만 채널로 접속하겠다 송신
				return std::string("JOIN " + v[3]);
			}
		}
		else{
			// v[3]가 특정 명령어일 때 응답하도록 구현
			return make_sense(v[3]);
		}
	}
	return std::string("");
}

std::string Bot::make_sense(std::string bot_cmd){
	std::string msg = "";
	if (bot_cmd == "!bot \n")
		msg += std::string("PRIVMSG " + _target + " :Did you miss me?\r\n");
	else if (bot_cmd == "!swang \n")
		msg += std::string("PRIVMSG " + _target + " :taylor swangft\r\n");
	else if (bot_cmd == "!jiwahn \n")
		msg += std::string("PRIVMSG " + _target + " :vim master\r\n");
	else if (bot_cmd == "!namkim \n")
		msg += std::string("PRIVMSG " + _target + " :south.k\r\n"); 
	else if (bot_cmd == "!hum \n")
		msg += std::string("PRIVMSG " + _target + " :nyaring.......\r\n"); 
	else if (bot_cmd == "!date \n"){
		time_t now;
		struct tm *tm_now;
		char time_str[9]; // YYYYMMDD\0

		time(&now);
		tm_now = localtime(&now);

		strftime(time_str, sizeof(time_str), "%Y%m%d", tm_now);
		msg = std::string("PRIVMSG " + _target + " :" + time_str + "\r\n");
	}
	return msg;
}


void Bot::answering(std::string answer){
	const char *msg;
	if (answer == "")
		return ;
	msg = answer.c_str();
	PRINT_MSG("SEND", "BOT", msg, B);
	int ret = send(_my_sock, msg, strlen(msg), 0);
	if (ret == -1) {
		std::cerr << "failed to send \n";
		exit(1);
	}
}

void Bot::error_handle(int ret, const char *noti){
	if (ret == -1){
		std::cerr << noti << "\n";
		exit(1);
	}
}
