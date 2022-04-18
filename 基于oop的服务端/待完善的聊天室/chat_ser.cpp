#include<sys/epoll.h>
#include<sys/socket.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>

const int MAXEVENTS= 100;
static int fds[2048];

void setnonblocking(int fd){
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

void handle_msg(int fd){
    int cnt_fd = fd;
    char buf[1024];
    while(1){
        memset(buf,0,sizeof(buf));
        ssize_t n = read(cnt_fd,buf,sizeof(buf));

        if(n <= 0){
            printf("CLIENT : %d closed connected\n",cnt_fd);
            fds[cnt_fd]--;
            printf("use id :%d 退出聊天\n",cnt_fd);
            close(cnt_fd);
            break;
        }
        else{
            for(int j=0;j<1024;j++){
                if(fds[j] !=0 && j!=cnt_fd){
                    char *temp = buf;
                    temp[n] = '\0';
                    write(j,temp,strlen(temp));
                }     
            }
            break;
        }
    }
    return;
}
int main(int argc, char *argv[]){
    if (argc <= 2){
        printf("usage: %s ip_address port_number\n", argv[0]);
        return 1;
    }

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &address.sin_addr);
    address.sin_port = htons(atoi(argv[2]));

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    int epollfd = epoll_create(5);
    struct epoll_event ev;
    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    //setnonblocking(listenfd);
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);   //这里检错没写

    struct epoll_event events[MAXEVENTS];
    while(1){
        int nfds = epoll_wait(epollfd,events,MAXEVENTS,-1);

        for(int i=0;i<nfds;i++){
           //printf("nfds = %d\n",nfds);
            if(events[i].data.fd == listenfd){
                struct sockaddr_in cnt_addr;
                socklen_t len = sizeof cnt_addr;
                int cnt_fd = accept(listenfd,(struct sockaddr*)&cnt_addr,&len);
                printf("have new client connected....\n");
                fds[cnt_fd]++;
                struct epoll_event event;
                event.data.fd = cnt_fd;
                event.events = EPOLLIN;
                //setnonblocking(cnt_fd);
                epoll_ctl(epollfd,EPOLL_CTL_ADD,cnt_fd,&event);  
            }
            
            else if(events[i].events & EPOLLIN){
                handle_msg(events[i].data.fd);
            }
            else
                printf("do dodod");
        }
    }
    close(listenfd);
    return 0;

}
