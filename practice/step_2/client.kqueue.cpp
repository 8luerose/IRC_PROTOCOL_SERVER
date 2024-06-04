#include "client_kqueue.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <sys/time.h>

KQueueEchoClient::KQueueEchoClient(const std::string &ip, int port) : server_ip(ip), server_port(port), client_socket(-1) {}

KQueueEchoClient::~KQueueEchoClient() {
    if (client_socket != -1) {
        close(client_socket);
    }
}

void KQueueEchoClient::connect_to_server() {
    struct sockaddr_in serv_addr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

void KQueueEchoClient::run() {
    connect_to_server();

    // std::string message;
    // std::cout << "Enter a message: ";
    // std::getline(std::cin, message);

    // write(client_socket, message.c_str(), message.size());
    // char buffer[1024] = {0};
    // read(client_socket, buffer, sizeof(buffer));
    // std::cout << "Server echoed: " << buffer << std::endl;

	while (true) { // 입력을 계속 받기 위해 무한 루프 사용
        std::string message;
        std::cout << "Enter a message: ";
        std::getline(std::cin, message);

        write(client_socket, message.c_str(), message.size());
        char buffer[1024] = {0};
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // null-terminate the buffer
            std::cout << "Server echoed: " << buffer << std::endl;
        } else {
            std::cout << "Disconnected from server." << std::endl;
            break;
        }
    }
}

int main() {
    KQueueEchoClient client("127.0.0.1", 12345);
    client.run();
    return 0;
}
