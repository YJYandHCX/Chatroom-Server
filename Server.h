#ifndef SERVER_H
#define SERVER_H

#include<iostream>
#include<stdio.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<list>
#define port 12345
#define ip "127.0.0.1"
#define BUFFER_SIZE 64
#define FORMAT "%d say %s"
using namespace std;

class Server{
public:
    Server();
    void close_server();
    //void Connect;
    int sendMessage(int clientfd);
    //void close();
    void init();
    //void init();
private:
    struct sockaddr_in address;
    int listenfd;
    list<int> clients;
    int epfd;

};
int setnonblocking(int fd);
void addfd(int epfd, int fd, bool enable_et);
#endif // SERVER_H
