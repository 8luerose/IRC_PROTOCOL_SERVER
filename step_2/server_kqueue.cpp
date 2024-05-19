#include "server_kqueue.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/event.h>
#include <sys/time.h>

KQueueEchoServer::KQueueEchoServer(int port) : port(port), server_socket(-1) {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
}

KQueueEchoServer::~KQueueEchoServer() {
    if (server_socket != -1) {
        close(server_socket); // 소켓 종료
    }
}

void KQueueEchoServer::create_socket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void KQueueEchoServer::bind_socket() {
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void KQueueEchoServer::listen_for_connections() {
    if (listen(server_socket, 10) == -1) { // 클라이언트 접속 대기
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void KQueueEchoServer::set_nonblocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

void KQueueEchoServer::handle_events() {
    int kq = kqueue(); // kqueue 생성
    if (kq == -1) {
        perror("kqueue");
        exit(EXIT_FAILURE);
    }

    struct kevent change;
    EV_SET(&change, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // 서버 소켓을 kqueue에 등록

    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        exit(EXIT_FAILURE);
    }

    struct kevent events[10];
    while (true) {
        int nev = kevent(kq, NULL, 0, events, 10, NULL); // 이벤트 대기
        if (nev == -1) {
            perror("kevent");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nev; ++i) {
            if (events[i].ident == server_socket) {
                int client_socket = accept(server_socket, NULL, NULL); // 클라이언트 접속 허용
                if (client_socket == -1) {
                    perror("accept");
                    continue;
                }

                set_nonblocking(client_socket); // 클라이언트 소켓을 non-blocking 모드로 설정

                EV_SET(&change, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // 클라이언트 소켓을 kqueue에 등록
                if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
                    perror("kevent");
                    exit(EXIT_FAILURE);
                }
            } else {
                char buffer[1024];
                int bytes_read = read(events[i].ident, buffer, sizeof(buffer) - 1); // 클라이언트로부터 데이터 수신
                if (bytes_read <= 0) {
                    close(events[i].ident); // 클라이언트 소켓 종료
                } else {
                    buffer[bytes_read] = '\0'; // null-terminate the buffer
                    std::cout << "Received: " << buffer << std::endl;
                    write(events[i].ident, buffer, bytes_read); // 클라이언트로 에코
                }
            }
        }
    }
}

void KQueueEchoServer::run() {
    create_socket();
    bind_socket();
    listen_for_connections();
    handle_events();
}

int main() {
    KQueueEchoServer server(12345); // 사용할 포트 번호
    server.run();
    return 0;
}
