#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/in.h>

class EchoClient {
public:
    EchoClient(const char* server_ip, int server_port);
    ~EchoClient();

    void connect_to_server();
    void send_message(const char* message);
    void receive_message();

private:
    int client_socket;
    struct sockaddr_in server_addr;

    void create_socket();
};

#endif // CLIENT_HPP
