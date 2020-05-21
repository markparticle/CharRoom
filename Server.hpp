/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef SERVER_CPP
#define SERVER_CPP
#include "Common.hpp"

class Server {
public:
    Server();

    ~Server();

    void Init();

    void Start();

    void Connect();

    void Close();

    int SendBroadCastMessage(const int clintFd);
    
private:
    struct sockaddr_in serverAddr_;

    int listener_;

    int epollFd_;

    list<int> clintsList_;
}


#endif //SERVER_CPP