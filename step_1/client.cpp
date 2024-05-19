#include "client.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

EchoClient::EchoClient(const char* server_ip, int server_port) {
    client_socket = -1;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
}

EchoClient::~EchoClient() {
    if (client_socket != -1) {
        close(client_socket); // 소켓 종료
    }
}

void EchoClient::create_socket() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void EchoClient::connect_to_server() {
    create_socket();

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) { // 서버에 접속
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

void EchoClient::send_message(const char* message) {
    if (write(client_socket, message, strlen(message)) == -1) { // 서버로 메시지 전송
        perror("write");
        exit(EXIT_FAILURE);
    }
}

void EchoClient::receive_message() {
    char buffer[1024];
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1); // 서버로부터 메시지 수신
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // null-terminate the buffer
        std::cout << "Received from server: " << buffer << std::endl;
    } else if (bytes_read == -1) {
        perror("read");
    }
}

int main() {
    EchoClient client("127.0.0.1", 12345); // 서버 IP와 포트
    client.connect_to_server();

    client.send_message("Hello, server!");
    client.receive_message();

    return 0;
}
