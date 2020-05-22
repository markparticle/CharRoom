/*
 * @Author       : mark
 * @Date         : 2020-05-22
 * @copyleft GPL 2.0
 */
#include <iostream>
#include "Server.hpp"

using namespace std;

Server::Server()
{
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(SERVER_PORT);
    serverAddr_.sin_addr.s_addr = inet_addr(SERVER_IP);
    socketFd_ = 0;
    epollFd_ = 0;
}


void Server::Init()
{
    int ret;

    cout << "Init Server..." << endl;

    socketFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd_ < 0)
    {
        perror("Create socket error!...");
        exit(-1);
    }

    ret = bind(socketFd_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_));
    if (ret < 0)
    {
        perror("Bind socket error!...");
        exit(-1);
    }

    ret = listen(socketFd_, 6);
    if (ret < 0)
    {
        perror("Listen socket error!...");
        exit(-1);
    }

    epollFd_ = epoll_create(EPOLL_SIZE);
    if (epollFd_ < 0)
    {
        perror("Create epoll error!...");
        exit(-1);
    }

    AddSocketFdToEpoll(socketFd_, epollFd_, true);
}

void Server::Close()
{
    
    close(socketFd_);
    close(epollFd_);
    cout << "Server Closed!..." << endl;
}

void Server::CloseClientFd(int clientFd)
{
    close(clientFd);
    clientsList_.remove(clientFd);

    cout << "ClientID : " << clientFd
         << " closed!...\n Now there are "
         << clientsList_.size()
         << " client in the char room"
         << endl;
}

void Server::Start()
{
    static struct epoll_event events[EPOLL_SIZE];
    Init();
    int ret;
    while (true)
    {
        int eventsCnt = epoll_wait(epollFd_, events, EPOLL_SIZE, TIMEOUT);
        if (eventsCnt < 0)
        {
            perror("Epoll events error!...");
            break;
        }
        cout << "Epoll events count = " << eventsCnt << endl;
        for (int i = 0; i < eventsCnt; i++)
        {
            int eventSockFd = events[i].data.fd;
            //简历一个新的连接
            if (eventSockFd == socketFd_)
            {

                struct sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(struct sockaddr);
                int clinetFd = accept(socketFd_, (struct sockaddr *)&clientAddr, &addrLen);

                cout << "Clinent connection from: "
                     << inet_ntoa(clientAddr.sin_addr) << " : "
                     << ntohs(clientAddr.sin_port) << ", clinetFd : "
                     << clinetFd << endl;

                AddSocketFdToEpoll(clinetFd, epollFd_, true);
                clientsList_.push_back(clinetFd);
                cout << "Add newClinent : " << clinetFd << " to epoll..." << endl;
                cout << "Now there are " << clientsList_.size() << " clients in the ChatRoom..." << endl;

                char message[BUF_SIZE];
                bzero(message, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clinetFd);
                ret = send(clinetFd, message, BUF_SIZE, 0);
                if (ret < 0)
                {
                    perror("Send message to Client:%d error!...");
                    CloseClientFd(clinetFd);
                    exit(-1);
                }
            }
            else
            {
                ret = SendBroadCastMessage(socketFd_);
                if (ret < 0)
                {
                    perror("Send broadCast message error!...");
                    Close();
                    exit(-1);
                }
            }
        }
    }
    Close();
}

int Server::SendBroadCastMessage(int clientFd)
{
    char buff[BUF_SIZE], message[BUF_SIZE];
    bzero(buff, BUF_SIZE);
    bzero(message, BUF_SIZE);
    int ret;

    int len = recv(clientFd, buff, BUF_SIZE, 0);
    if (len == 0)
    {
        CloseClientFd(clientFd);
    }
    else
    {
        cout << "Client:%d SendBroadCastMessage..." << clientFd << endl;
        sprintf(message, SERVER_MESSAGE, clientFd, buff);
        for (auto &item : clientsList_)
        {
            if (item != clientFd)
            {
                ret = send(item, message, BUF_SIZE, 0);
                if (ret < 0)
                    return -1;
            }
        }
    }
    return len;
}

int main()
{
    Server server;
    server.Start();
}