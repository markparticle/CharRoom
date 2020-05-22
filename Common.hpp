/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef COMMON_HPP
#define COMMON_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>

const int SERVER_PORT = 8964;

const char SERVER_IP[] =  "127.0.0.1";

const int EPOLL_SIZE = 5000;

const int BUF_SIZE = 0xFFFF;

const int TIMEOUT = -1;


static void AddSocketFdToEpoll(int socketFd, int epollFd, bool enableET) 
{
    struct epoll_event ev;
    ev.data.fd = socketFd;
    ev.events = EPOLLIN;
    if(enableET) 
    {
        ev.events = EPOLLIN | EPOLLET;
    }
    epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &ev);

    //设为非阻塞
    int flags = fcntl(socketFd, F_GETFD, 0);
    fcntl(socketFd, F_SETFL, flags | O_NONBLOCK);

    printf("SocketFd(%d) added to epoll(%d)!...\n", socketFd, epollFd);
}

#endif //COMMON_HPP