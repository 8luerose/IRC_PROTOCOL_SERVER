#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>

class EchoServer {
public:
    EchoServer(int port);
    ~EchoServer();

    void run();

private:
    int server_socket;
    int port;
    struct sockaddr_in server_addr;

    void create_socket();
    void bind_socket();
    void listen_for_connections();
    void accept_connections();
    void handle_client(int client_socket);
    void set_nonblocking(int socket);
};

#endif // SERVER_HPP
