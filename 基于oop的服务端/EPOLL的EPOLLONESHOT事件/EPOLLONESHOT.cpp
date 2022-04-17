#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10


struct fds{
    int epollfd;
    int sockfd;
};

int setnonblocking(int fd){
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

void addfd(int epollfd,int sockfd,bool oneshot){  //添加到事件表
        epoll_event ev;
        ev.data.fd = sockfd,
        ev.events = EPOLLIN | EPOLLET;
        if(oneshot)
            ev.events |= EPOLLONESHOT;
        epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);
        setnonblocking(sockfd);

} 


void reset_oneshot(int epollfd,int sockfd){  //重置事件  主要是重置EPOLLON    ESHOT事件
    epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,sockfd,&ev);

}


void *worker(void *arg){   //工作线程
    int epollfd = ((fds*)arg)->epollfd;
    int sockfd = ((fds*)arg)->sockfd;

    char buf[BUFFER_SIZE];
    memset(buf,0,sizeof buf);
    while(1){
        ssize_t n = read(sockfd,buf,sizeof buf);
        if(n == 0){
            close(sockfd);
            printf("Clinet close connection\n");
            break;
        }
        else if(n<0 && (errno)==EAGAIN || (errno)==EWOULDBLOCK){ //读完了
            printf("数据读完了\n");
            reset_oneshot(epollfd,sockfd);
            break;
        }

        else{
            printf("get content:%s\n",buf);
            printf("处理数据中\n");
            sleep(5);
        }
    }
    printf("end thread receving data on fd :%d\n",sockfd);
    printf("EXIT THREAD..........\n");
    pthread_exit(NULL);
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
    listen(lfd,10);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd,lfd,false);

    while(1){
        int nfds = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        
        for(int i=0;i<nfds;i++){
            if(events[i].data.fd == lfd){
                struct sockaddr_in cnt_addr;
                socklen_t len = sizeof cnt_addr;
                int cnt_fd = accept(lfd,(struct sockaddr*)&cnt_addr,&len);
                addfd(epollfd,cnt_fd,true);
            }
            else if(events[i].events & EPOLLIN){
                pthread_t thread_id;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd = epollfd;
                fds_for_new_worker.sockfd = events[i].data.fd;
                pthread_create(&thread_id,NULL,worker,(void*)&fds_for_new_worker);
                
                pthread_detach(thread_id);
            }
            else
                printf("something else haneeend。。。\n");
        }
    }
    close(lfd);
    return 0;
}


