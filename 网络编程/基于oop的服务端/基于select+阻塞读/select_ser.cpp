#include<sys/select.h>
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
int main(int argc, char *argv[]){
    if (argc <= 2)
    {
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

    fd_set tempfds;
    fd_set readfds;
    FD_ZERO(&tempfds);
    FD_ZERO(&readfds);
    FD_SET(listenfd,&readfds);
    int maxfd = listenfd;
    while(1){
        tempfds = readfds;
        int nfds = select(maxfd+1,&tempfds,NULL,NULL,NULL);
        if(nfds<0){
            if(errno == EINTR) continue;
            break;
        }
        if(FD_ISSET(listenfd,&tempfds)){
            struct sockaddr_in cnt_addr;
            socklen_t len_cnt = sizeof cnt_addr;
            int cnt_fd = accept(listenfd,(struct sockaddr*)&cnt_addr,&len_cnt);
            FD_SET(cnt_fd,&readfds);
            if(cnt_fd>maxfd)
                maxfd = cnt_fd;
            if(--nfds==0) continue;
        }
        for(int i=listenfd+1;i<=maxfd;i++){
            int sockfd = i;
            if(FD_ISSET(sockfd,&tempfds)){
                char buf[1024];          
                while(1){
                    memset(buf,0,sizeof buf);
                    ssize_t n = read(sockfd,buf,sizeof buf);
                    if(n == 0){
                        printf("client closed connection\n");
                        FD_CLR(sockfd,&readfds);
                        close(sockfd);
                        break;
                    }
                    else if(n<0 && (errno==EAGAIN) || (errno==EWOULDBLOCK)){
                        printf("数据读完了\n");
                        break;
                    }
                    else{
                        printf("come form client fd :%d message :%s\n",sockfd,buf);
                        printf("处理数据中。。。\n");
                        sleep(5);
                        printf("处理完成!\n");
                        break;
                    }
                }

            }

        }
    }
    close(listenfd);
    return 0;
}






