#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <netinet/in.h>
#include <string>
#include <map>
#include <set>
#include <vector>

class IRCServer {
public:
    IRCServer(int port);
    ~IRCServer();

    void run();

private:
    int server_socket;
    int port;
    struct sockaddr_in server_addr;

    std::map<int, std::string> clients; // 클라이언트 소켓과 닉네임 맵
    std::map<std::string, std::set<int>> channels; // 채널 이름과 클라이언트 소켓 집합 맵

    void create_socket();
    void bind_socket();
    void listen_for_connections();
    void set_nonblocking(int socket);
    void handle_events();
    void process_message(int client_socket, const std::string& message);

    void handle_pass(int client_socket, const std::vector<std::string>& params);
    void handle_nick(int client_socket, const std::vector<std::string>& params);
    void handle_user(int client_socket, const std::vector<std::string>& params);
    void handle_join(int client_socket, const std::vector<std::string>& params);
    void handle_part(int client_socket, const std::vector<std::string>& params);
    void handle_privmsg(int client_socket, const std::vector<std::string>& params);
    void handle_notice(int client_socket, const std::vector<std::string>& params);
    void handle_list(int client_socket, const std::vector<std::string>& params);
    void handle_ping(int client_socket, const std::vector<std::string>& params);
    void handle_oper(int client_socket, const std::vector<std::string>& params);
    void handle_kick(int client_socket, const std::vector<std::string>& params);
    void handle_quit(int client_socket, const std::vector<std::string>& params);

    void send_message(int client_socket, const std::string& message);
};

#endif // IRC_SERVER_HPP
