/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef CLINET_HPP
#define CLINET_HPP
#include "Common.hpp"

class Clinet {
public:
    Clinet();

    ~Clinet();

    void Start();

    void Connect();

    void Close();
    
private:
    struct sockaddr_in serverAddr_;

    int sockFd_;

    int epollFd_;

    int pid_;

    int pipeFd[2];

    bool isClinetWork;

    char message_[BUF_SIZE];
}


#endif //CLINET_HPP