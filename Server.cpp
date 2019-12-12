#include<iostream>
#include "Server.h"
using namespace std;

Server::Server(){

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >=0);



    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    //inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);




    int ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
    assert(ret!=-1);

    ret = listen(listenfd,5);
    assert(ret!=-1);

    cout<<"LISTEN"<<endl;

    epfd = epoll_create(5);

    assert(epfd!=-1);
    addfd(epfd,listenfd,true);
}

int Server::sendMessage(int clientfd){

}
void Server::init(){

    static epoll_event events[5];

    while(1){
        int epoll_events_count = epoll_wait(epfd,events,5,-1);

        if (epoll_events_count<0) {
            cout<<"epoll error"<<endl;
            break;
        }
        cout<<"epoll_events_count="<<epoll_events_count<<endl;

        for (int i=0;i<epoll_events_count;i++){
            int sockfd = events[i].data.fd;

            if (sockfd == listenfd){
                sockaddr_in client_address;
                socklen_t client_addrLen = sizeof(client_address);
                int clientfd = accept(listenfd,(sockaddr*)&client_address,&client_addrLen);

                //clients[i].address = clientfd;
                addfd(epfd,clientfd,true);

                clients.push_back(clientfd);
                cout<<"Number of clients is "<<clients.size()<<endl;

                char mesg[BUFFER_SIZE];
                memset(&mesg,0,BUFFER_SIZE);
                sprintf(mesg,"Welcome!",clientfd);
                int ret = send(clientfd,mesg,BUFFER_SIZE,0);
            }
            else{
                char buf[BUFFER_SIZE],mesg[BUFFER_SIZE];
                bzero(buf,BUFFER_SIZE);
                bzero(mesg,BUFFER_SIZE);

                cout<<"message from "<<sockfd<<endl;

                int len = recv(sockfd, buf ,BUFFER_SIZE, 0 );

                if (len==0){
                    close(sockfd);
                    cout<<sockfd<<" left the chatro0m"<<endl;
                    clients.remove(sockfd);

                }
                else{
                    sprintf(mesg,FORMAT,sockfd,buf);
                    list<int>::iterator p ;
                    for (p = clients.begin();p!=clients.end();p++){
                        if (*p != sockfd){
                            int ret = send(*p, mesg, BUFFER_SIZE,0);
                        }
                    }
                }
            }

        }
    }
}
void Server::close_server(){
    close(epfd);
    close(listenfd);
}

int setnonblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
void addfd(int epfd,int fd, bool enable_et){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_et) event.events |= EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}
