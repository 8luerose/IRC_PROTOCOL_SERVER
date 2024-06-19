#include <iostream>
#include <vector>
#include <string>
#include "Command.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

int main() {
    // 서버 초기화
    Server server;

    // 클라이언트 생성 및 등록
    int fd1 = 1;
    int fd2 = 2;
    Client client1(fd1);
    Client client2(fd2);
    client1.setNickname("client1");
    client2.setNickname("client2");
    server.addClient(fd1, client1);
    server.addClient(fd2, client2);

    // 채널 생성 및 등록
    std::string channelName = "#testchannel";
    Channel* channel = new Channel(channelName, fd1); // Channel*로 수정
    server.addChannel(channelName, channel); // Channel*로 수정

    // Command 객체 생성
    Command command(server);

    // 테스트 케이스 1: 모드 조회
    std::vector<std::string> cmdVector1 = {"/MODE", "#testchannel"};
    command.mode(fd1, cmdVector1); // fd1이 조회 요청
    std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

    // 테스트 케이스 2: 모드 설정 (i 모드 추가)
    std::vector<std::string> cmdVector2 = {"/MODE", "#testchannel", "+i"};
    command.mode(fd1, cmdVector2); // fd1이 설정 요청
    std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

    // 테스트 케이스 3: 모드 설정 (t 모드 추가)
    std::vector<std::string> cmdVector3 = {"/MODE", "#testchannel", "+t"};
    command.mode(fd1, cmdVector3); // fd1이 설정 요청
    std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

    // 테스트 케이스 4: 모드 설정 (i 모드 제거)
    std::vector<std::string> cmdVector4 = {"/MODE", "#testchannel", "-i"};
    command.mode(fd1, cmdVector4); // fd1이 설정 요청
    std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

	std::vector<std::string> cmdVector5 = {"/MODE", "#testchannel", "+k", "secret"};
	command.mode(fd1, cmdVector5);
	std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

	std::vector<std::string> cmdVector6 = {"/MODE", "#testchannel", "+l", "10"};
	command.mode(fd1, cmdVector6);
	std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

	std::vector<std::string> cmdVector7 = {"/MODE", "#testchannel", "+o", "client2"};
	command.mode(fd1, cmdVector7);
	std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

	std::vector<std::string> cmdVector8 = {"/MODE", "#testchannel", "+ikl" , "secret", "10"};
	command.mode(fd1, cmdVector8);
	std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

	std::vector<std::string> cmdVector9 = {"/MODE", "#testchannel", "-olk" , "secret", "10"};
	command.mode(fd1, cmdVector9);
	std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;

	// std::vector<std::string> cmdVector10 = {"/MODE", "#testchannel", "+l 10", "-k"};
	std::vector<std::string> cmdVector10 = {"/MODE", "#testchannel", "+l", "10", "-k"};
	command.mode(fd1, cmdVector10);
	std::cout << "Client1 Buffer: " << client1.getReciveBuf() << std::endl;


    // 메모리 해제
    delete channel;

    return 0;
}
