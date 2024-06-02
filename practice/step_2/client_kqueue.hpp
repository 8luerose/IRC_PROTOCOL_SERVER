#ifndef CLIENT_KQUEUE_HPP
#define CLIENT_KQUEUE_HPP

#include <string>

class KQueueEchoClient {
public:
    KQueueEchoClient(const std::string &ip, int port);
    ~KQueueEchoClient();

    void connect_to_server();
    void run();

private:
    int client_socket;
    std::string server_ip;
    int server_port;
};

#endif // CLIENT_KQUEUE_HPP
