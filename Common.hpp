/*
 * @Author       : mark
 * @Date         : 2020-05-21
 * @copyleft GPL 2.0
 */ 

#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
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

const int SERVER_PORT = 8964;

const int EPOLL_SIZE = 5000;

const int BUF_SIZE = 0xFFFF;


#endif COMMON_HPP