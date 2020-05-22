/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef SERVER_CPP
#define SERVER_CPP
#include "Common.hpp"

const char SERVER_WELCOME[] = "Welcome you join to the CharRoom! Your chat ID is: %d\n";
const char SERVER_MESSAGE[] = "Client%d say:  >> %s";  

class Server {
public:
    Server();

    void Init();

    void Start();

    void Connect();

    void Close();

    void CloseClientFd(int clientFd);

    int SendBroadCastMessage(const int clientFd);
    
private:
    int socketFd_;

    struct sockaddr_in serverAddr_;

    int epollFd_;

    std::list<int> clientsList_;
};


#endif //SERVER_CPP