#include "server.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

EchoServer::EchoServer(int port) : port(port), server_socket(-1) {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
}

EchoServer::~EchoServer() {
    if (server_socket != -1) {
        close(server_socket); // 소켓 종료
    }
}

void EchoServer::create_socket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void EchoServer::bind_socket() {
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void EchoServer::listen_for_connections() {
    if (listen(server_socket, 10) == -1) { // 클라이언트 접속 대기
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void EchoServer::accept_connections() {
    while (true) {
        int client_socket = accept(server_socket, NULL, NULL); // 클라이언트 접속 허용
        if (client_socket == -1) {
            perror("accept");
            continue;
        }
        handle_client(client_socket); // 클라이언트 처리
    }
}

void EchoServer::handle_client(int client_socket) {
    char buffer[1024];
    int bytes_read;

    while ((bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0'; // null-terminate the buffer
        std::cout << "Received: " << buffer << std::endl;
        send(client_socket, buffer, bytes_read, 0); // 클라이언트로 에코
    }

    if (bytes_read == -1) {
        perror("recv");
    }

    close(client_socket); // 클라이언트 소켓 종료
}

void EchoServer::run() {
    create_socket();
    bind_socket();
    listen_for_connections();
    accept_connections();
}

int main() {
    EchoServer server(12345); // 사용할 포트 번호
    server.run();
    return 0;
}
