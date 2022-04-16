#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<assert.h>
#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10
int setnonblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option; 
}

void addfd(int epollfd,int fd,bool enable_et){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(enable_et){
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);

}

/* LT*/
void lt(epoll_event *events,int number,int epollfd,int listenfd){
    char buf[BUFFER_SIZE];
    for(int i=0;i<number;i++){
        int sockfd = events[i].data.fd;
        if(sockfd == listenfd){
            struct sockaddr_in cnt_addr;
            socklen_t cnt_len = sizeof(cnt_addr);
            int connfd = accept(listenfd,(struct sockaddr*)&cnt_addr,&cnt_len);
            addfd(epollfd,connfd,false);
        }

        else if(events[i].events & EPOLLIN){
            printf("event trigger once\n");
            memset(buf,0,BUFFER_SIZE);
            int ret =recv(sockfd,buf,BUFFER_SIZE-1,0);
            if(ret<=0){
                close(sockfd);
                continue;
            }
            //else{
                //printf("读了这些：%s\n",buf);
           // }
        }
        else{
            printf("do other something!\n");
        }
    }
}

void et(epoll_event *events,int number,int epollfd,int listenfd){
    char buf[BUFFER_SIZE];
    for(int i=0;i<number;i++){
        int sockfd = events[i].data.fd;
        if(sockfd == listenfd){
            struct sockaddr_in cnt_addr;
            socklen_t cnt_len = sizeof(cnt_addr);
            int connfd = accept(listenfd,(struct sockaddr*)&cnt_addr,&cnt_len);
            addfd(epollfd,connfd,true);
        }
        else if(events[i].events & EPOLLIN){
            printf("event trigger once\n");
            while(1){
                memset(buf,0,BUFFER_SIZE);
                int ret = recv(sockfd,buf,BUFFER_SIZE-1,0);
                if(ret<0){
                    if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                        printf("读完了\n");
                        break;;
                    }
                    close(sockfd);
                    break;
                }
                else if(ret == 0){
                    close(sockfd);
                }
                else{
                    printf("get %d bytes of content:%s\n",ret,buf);
                }
            }
        }
        else{
            printf("do something");
        }
    }
}


int main(int argc,char *argv[]){
    if(argc<2){
        perror("argc error\n");
        return 1;
    }
    int ret = 0;
    struct sockaddr_in ser_add;
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    memset(&ser_add,0,sizeof ser_add);
    ser_add.sin_port = htons(atoi(argv[1]));
    ser_add.sin_family = AF_INET;
    ser_add.sin_addr.s_addr = htonl(INADDR_ANY); 
    bind(lfd,(struct sockaddr*)&ser_add,sizeof ser_add);
    listen(lfd,100);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd,lfd,true);

    while(1){
        int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if(ret<0){
            printf("epoll failure\n");
            break;
        }
        //lt(events,ret,epollfd,lfd);
        et(events,ret,epollfd,lfd);
    }
    close(lfd);

    return 0;

}
