#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <map>

class Server; // Forward declaration

class Client {
private:
    int fd;
    std::vector<std::string> channelList;
    std::string nickname;
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
    std::string receiveBuf;
    bool isRegiPass;
    bool isRegiNick;
    bool isRegiUser;

public:
    Client();
    Client(int fd);
    Client(const Client& other); // 복사 생성자
    Client& operator=(const Client& other); // 복사 할당 연산자
    ~Client();

    std::vector<std::string>::iterator findChannel(std::string);

    // Getter methods
    int getClientFd();
    std::vector<std::string>& getChannelList();
    std::string getNickname();
    std::string getUsername();
    std::string getHostname();
    std::string getServername();
    std::string getRealname();
    std::string getReciveBuf();
    bool getIsRegist();
    bool getRegiPass();
    bool getRegiNick();
    bool getRegiUser();

    // Setter methods
    void setRegiPass(bool status);
    void setRegiNick(bool status);
    void setRegiUser(bool status);
    void setRegiAll(bool status);
    void setNickname(std::string setName);
    void setUserinfo(std::string inputUserName, std::string inputHostName, std::string inputServerName, std::string inputRealName);
    void appendReciveBuf(std::string inputBuffer);
    void appendChannelList(std::string channelName);

    // Clear methods
    void clearReciveBuf();
    void clearAllChannel();
    void resetClient();
    void deleteChannel(std::string delChannel);
};

#endif
