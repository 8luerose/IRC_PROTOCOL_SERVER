#include "irc_server.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/event.h>
#include <sys/time.h>
#include <sstream>

IRCServer::IRCServer(int port) : port(port), server_socket(-1) {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
}

IRCServer::~IRCServer() {
    if (server_socket != -1) {
        close(server_socket); // 소켓 종료
    }
}

void IRCServer::create_socket() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void IRCServer::bind_socket() {
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void IRCServer::listen_for_connections() {
    if (listen(server_socket, 10) == -1) { // 클라이언트 접속 대기
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void IRCServer::set_nonblocking(int socket) {
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

void IRCServer::handle_events() {
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

                clients[client_socket] = ""; // 클라이언트 추가
            } else {
                char buffer[1024];
                int bytes_read = read(events[i].ident, buffer, sizeof(buffer) - 1); // 클라이언트로부터 데이터 수신
                if (bytes_read <= 0) {
                    close(events[i].ident); // 클라이언트 소켓 종료
                    clients.erase(events[i].ident); // 클라이언트 제거
                } else {
                    buffer[bytes_read] = '\0'; // null-terminate the buffer
                    std::string message(buffer);
                    process_message(events[i].ident, message); // 메시지 처리
                }
            }
        }
    }
}

void IRCServer::process_message(int client_socket, const std::string& message) {
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    std::vector<std::string> params;
    std::string param;
    while (iss >> param) {
        params.push_back(param);
    }

    if (command == "PASS") {
        handle_pass(client_socket, params);
    } else if (command == "NICK") {
        handle_nick(client_socket, params);
    } else if (command == "USER") {
        handle_user(client_socket, params);
    } else if (command == "JOIN") {
        handle_join(client_socket, params);
    } else if (command == "PART") {
        handle_part(client_socket, params);
    } else if (command == "PRIVMSG") {
        handle_privmsg(client_socket, params);
    } else if (command == "NOTICE") {
        handle_notice(client_socket, params);
    } else if (command == "LIST") {
        handle_list(client_socket, params);
    } else if (command == "PING") {
        handle_ping(client_socket, params);
    } else if (command == "OPER") {
        handle_oper(client_socket, params);
    } else if (command == "KICK") {
        handle_kick(client_socket, params);
    } else if (command == "QUIT") {
        handle_quit(client_socket, params);
    }
}

void IRCServer::handle_pass(int client_socket, const std::vector<std::string>& params) {
    // 패스워드 확인 로직
    send_message(client_socket, "PASS command received\n");
}

void IRCServer::handle_nick(int client_socket, const std::vector<std::string>& params) {
    if (params.size() < 1) {
        send_message(client_socket, "ERROR: No nickname provided\n");
        return;
    }
    clients[client_socket] = params[0];
    send_message(client_socket, "Nickname set to " + params[0] + "\n");
}

void IRCServer::handle_user(int client_socket, const std::vector<std::string>& params) {
    // 유저 이름 설정 로직
    send_message(client_socket, "USER command received\n");
}

void IRCServer::handle_join(int client_socket, const std::vector<std::string>& params) {
    if (params.size() < 1) {
        send_message(client_socket, "ERROR: No channel provided\n");
        return;
    }
    std::string channel = params[0];
    channels[channel].insert(client_socket);
    send_message(client_socket, "Joined channel " + channel + "\n");
}

void IRCServer::handle_part(int client_socket, const std::vector<std::string>& params) {
    if (params.size() < 1) {
        send_message(client_socket, "ERROR: No channel provided\n");
        return;
    }
    std::string channel = params[0];
    channels[channel].erase(client_socket);
    send_message(client_socket, "Left channel " + channel + "\n");
}

void IRCServer::handle_privmsg(int client_socket, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        send_message(client_socket, "ERROR: No message or target provided\n");
        return;
    }
    std::string target = params[0];
    std::string message = params[1];
    for (size_t i = 2; i < params.size(); ++i) {
        message += " " + params[i];
    }

    if (target[0] == '#') {
        // 채널에 메시지 전송
        for (int sock : channels[target]) {
            if (sock != client_socket) {
                send_message(sock, clients[client_socket] + ": " + message + "\n");
            }
        }
    } else {
        // 특정 사용자에게 메시지 전송
        for (const auto& pair : clients) {
            if (pair.second == target) {
                send_message(pair.first, clients[client_socket] + ": " + message + "\n");
                break;
            }
        }
    }
}

void IRCServer::handle_notice(int client_socket, const std::vector<std::string>& params) {
    // NOTICE 명령어 처리 로직
    send_message(client_socket, "NOTICE command received\n");
}

void IRCServer::handle_list(int client_socket, const std::vector<std::string>& params) {
    std::string channel_list = "Available channels:\n";
    for (const auto& pair : channels) {
        channel_list += pair.first + "\n";
    }
    send_message(client_socket, channel_list);
}

void IRCServer::handle_ping(int client_socket, const std::vector<std::string>& params) {
    send_message(client_socket, "PONG\n");
}

void IRCServer::handle_oper(int client_socket, const std::vector<std::string>& params) {
    // OPER 명령어 처리 로직
    send_message(client_socket, "OPER command received\n");
}

void IRCServer::handle_kick(int client_socket, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        send_message(client_socket, "ERROR: No user or channel provided\n");
        return;
    }
    std::string channel = params[0];
    std::string user = params[1];

    for (int sock : channels[channel]) {
        if (clients[sock] == user) {
            channels[channel].erase(sock);
            send_message(sock, "You have been kicked from channel " + channel + "\n");
            break;
        }
    }
}

void IRCServer::handle_quit(int client_socket, const std::vector<std::string>& params) {
    send_message(client_socket, "Goodbye!\n");
    close(client_socket); // 클라이언트 소켓 종료
    clients.erase(client_socket); // 클라이언트 제거
}

void IRCServer::send_message(int client_socket, const std::string& message) {
    write(client_socket, message.c_str(), message.size()); // 메시지 전송
}

void IRCServer::run() {
    create_socket();
    bind_socket();
    listen_for_connections();
    handle_events();
}

int main() {
    IRCServer server(12345); // 사용할 포트 번호
    server.run();
    return 0;
}
