#ifndef SERVER_KQUEUE_HPP
#define SERVER_KQUEUE_HPP

#include <netinet/in.h>

class KQueueEchoServer {
public:
    KQueueEchoServer(int port);
    ~KQueueEchoServer();

    void run();

private:
    int server_socket;
    int port;
    struct sockaddr_in server_addr;

    void create_socket();
    void bind_socket();
    void listen_for_connections();
    void set_nonblocking(int socket);
    void handle_events();
};

#endif // SERVER_KQUEUE_HPP
