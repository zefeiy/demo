#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<unistd.h>

#define BUFFER_SIZE 1024 

void pro_error(int ret,char *mes){ 
	if(ret == -1){
		perror(mes);
		exit(1);
	}
}

int main(int argc, char* argv[]) {
    if(argc<=2){
        printf("agument error\n");
        exit(1);
    }
        
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    pro_error(sockfd, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    pro_error(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)), "socket connect error");

    int epollfd = epoll_create(5);
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);

    struct epoll_event ev2;
    ev2.data.fd = STDIN_FILENO;
    ev2.events = EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev2);

    struct epoll_event events[100];
    while(true){

        int nfs = epoll_wait(epollfd,events,100,-1);
        for(int i=0;i<nfs;i++){
            char buf[BUFFER_SIZE];
            bzero(&buf, sizeof(buf));
            if(events[i].data.fd == sockfd && (events[i].events | EPOLLIN)){
                ssize_t n = read(sockfd,buf,sizeof(buf));
                printf("come from other user msg :%s\n",buf);
            }

            else if(events[i].data.fd == STDIN_FILENO && (events[i].events | EPOLLIN)){
                ssize_t n = read(STDIN_FILENO,buf,sizeof(buf));
                char *temp = buf;
                temp[n] = '\0';
                write(sockfd,temp,strlen(temp));
            }
        }
    }
    close(sockfd);
    return 0;
}