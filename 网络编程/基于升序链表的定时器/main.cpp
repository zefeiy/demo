#include"list_timer.h"

static int pipefd[2];
static sort_list timer_lst;
static int epollfd = 0;

#define MAX_EVENT_NUMBER 1024
#define FD_LIMIT 256
#define TIMESLOT 2  //超时的时间


void exit_if(bool flag,char* msg){
    if(flag){
        perror(msg);
        exit(1);
    }
}

void setnonblocking(int fd){
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

void sig_handler(int sig){
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1],(char*)&msg,1,0);
    errno = save_errno;
}

void addsig(int sig){
    struct sigaction sa;
    memset(&sa,0,sizeof sa);
    sa.sa_flags |= SA_RESTART;
    sa.sa_handler = sig_handler;
    sigfillset(&sa.sa_mask);
    exit_if(sigaction(sig,&sa,NULL) == -1 ,"add sig error");
}

void addfd(int fd){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
    setnonblocking(fd);
}

void func(clinet_data* user_data){
    close(user_data->sockfd);
    epoll_ctl(epollfd,EPOLL_CTL_DEL,user_data->sockfd,0);
    printf("close fd %d\n", user_data->sockfd);
}

void timer_handler(){
    timer_lst.tick();
    alarm(TIMESLOT);
}


int main(int argc, char* argv[]){

     
    exit_if(argc <= 2, "wrong number of parameters");
    
    char* ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
   
    
    address.sin_family = AF_INET;
    
    address.sin_port = htons(port);
     
    address.sin_addr.s_addr = inet_addr(ip);
    printf("所有初始化完成\n");
    
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    exit_if(listenfd == -1, "socket error");

   
    int ret = bind(listenfd, (sockaddr*)&address, sizeof(address));
    exit_if(ret == -1, "bind error");

    ret = listen(listenfd, 5);
    exit_if(ret == -1, "listen errro");

    addsig(SIGALRM);
    addsig(SIGTERM);

    clinet_data* users = new clinet_data[FD_LIMIT];
    struct epoll_event events[MAX_EVENT_NUMBER];
    int epfd = epoll_create(5);
    epollfd = epfd;
    addfd(listenfd);

    ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);
    setnonblocking(pipefd[1]);
    addfd(pipefd[0]);
    
    bool stop_server = false;
    bool timeout = false;

    alarm(TIMESLOT);  //开始定时器

    
    while(!stop_server){
        int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        for(int i=0;i<number;i++){

            int fdd = events[i].data.fd;
            
            if(fdd == listenfd){
                struct sockaddr_in client_address;
                memset(&client_address,0x00,sizeof client_address);
                socklen_t client_addresslen = sizeof(client_address);

                int newfd = accept(listenfd,(sockaddr*)&client_address,&client_addresslen);
                addfd(newfd);

                util_timer *timer = new util_timer();
                timer->user_data = &users[newfd];
                timer->func = func;
                time_t now = time(NULL);
                timer->expire = now + TIMESLOT*2;

                users[newfd].address = client_address;
                users[newfd].sockfd = newfd;
                users[newfd].timer = timer;
                timer_lst.add_timer(timer);

                printf("有新的客户端连接\n");
            }

            else if((fdd == pipefd[0]) && (events[i].events & EPOLLIN)){
                int sig;
                char signals[1024];
                ret = recv(fdd,signals,sizeof(signals),0);
                if(ret<=0)
                    continue;
                else{
                    for(int j=0;j<ret;j++){
                        switch(signals[j]){
                        case SIGALRM:
                            timeout = true;
                            break;

                        case SIGTERM:
                            stop_server = true;
                            break;
                        }

                    }
                }

            }

            else if(events[i].events & EPOLLIN){
                //printf("可以监听到消息来了!\n");
                memset(users[fdd].buf,0,BUFFER_SIZE);
                int n = recv(fdd,users[fdd].buf,BUFFER_SIZE-1,0);
                //printf("可以读取数据到user的buf中  [n = %d]\n",n);
                util_timer* timer = users[fdd].timer;
                if(n<0){
                    if(errno != EAGAIN && errno != EWOULDBLOCK)
                        func(&users[fdd]);
                        if(timer)
                            timer_lst.del_timer(timer);
                }

                else if(n == 0){
                    printf("客户端主动断开了连接!\n");
                    func(&users[fdd]);
                    if(timer)
                        timer_lst.del_timer(timer);
                }
                
                else{

                    printf("get %d bytes of client data %s from %d \n",n,users[fdd].buf,fdd);
                    if(timer != NULL){
                        time_t cur = time(NULL);
                        timer->expire = cur + TIMESLOT+5;
                        printf("adjust timr once\n");
                        timer_lst.adjust_timer(timer);
                    }

                }
            }

        }
        if(timeout){
            timer_handler();
            timeout = false;
        }

    }

    close(listenfd);
    close(pipefd[1]);
    close(pipefd[0]);
    delete[] users;
    return 0;

}