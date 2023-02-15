
#include<string>
#include<unordered_set>
#include<unordered_map>
#include<sys/epoll.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<vector>
#include<signal.h>
#include"lst_timer.h"
#include"user.h"


class server{

public:
    server() = delete;

    server(char* ip,int port);

    ~server();

    void add_epoll(user* tmp);

    void add_epoll(int fd);

    void rm_epoll(user* tmp);

    void add_sig(int,void(handler)(int));


    void handle_msg(char* msg, int);

    void init();   //做初始化工作

    void Eventloop();

    friend void send_sig(int sig);

    inline void setnonblocking(int fd){
        fcntl(fd, F_SETFL, fcntl(fd,F_GETFL) | O_NONBLOCK);
    }

private:
    static const int MAXEVENTS = 100;
    
    int listenfd;
    //struct epoll_event ev;
    struct epoll_event events[MAXEVENTS];
    struct sockaddr_in address;    
    char* IP;
    int PORT;

    //std::vector<user*> User;

public:
    static sort_timer_lst lst_timer;

    static int pipe[2];

    static int m_epollfd;

    //static std::unordered_set<int> set;   //用来寻找当前用户是否登录,通过fd辨别

    static std::unordered_map<int,user*> user_map;

};

int server::pipe[2];
int server::m_epollfd = -1;
std::unordered_set<int> set;   //用来寻找当前用户是否登录,通过fd辨别
std::unordered_map<int,user*> user_map;

server::server(char* ip,int port):IP(ip),PORT(port)
,listenfd(-1),address(),events(){
    init();
}

void server::add_epoll(user* tmp){
    struct epoll_event event;
    event.data.fd = tmp->getfd();
    event.events = EPOLLIN;
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,tmp->getfd(),&event);
}

void server::add_epoll(int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,&event);
}

void server::rm_epoll(user* tmp){
    epoll_ctl(m_epollfd,EPOLL_CTL_DEL,tmp->getfd(),0);
    close(tmp->getfd());
}

void server::add_sig(int sig, void(handler)(int)){
    struct sigaction sigact;
    memset(&sigact, '\0', sizeof sigact);
    sigact.sa_flags |= SA_RESTART;
    sigact.sa_handler = handler;
    sigfillset(&sigact.sa_mask);
    sigaction(sig, &sigact, NULL);
}

void send_sig(int sig){
    int save_errno = errno;
    int msg = sig;
    send(server::pipe[1], (char*)&msg, 1, 0);
    errno = save_errno;
}

void server::handle_msg(char* msg, int sockfd){
    //char buf[1024];
    //memset(buf, '\0', sizeof buf);
    //strcpy(buf, msg);
    //msg[strlen(msg)+1] = '\0';
    int count = 0;
    for(std::unordered_map<int, user*>::iterator item = this->user_map.begin(); item != this->user_map.end(); ++item){
        if(item->first != sockfd){
            int n = write(item->first, msg, strlen(msg));
            count++;
        }
    }
    printf("共发送了%d条消息\n",count);
    return;
}

void server::init(){
    
    bzero(&this->address, sizeof this->address);
    this->address.sin_family  = AF_INET;
    this->address.sin_port = this->PORT;
    inet_pton(AF_INET, this->IP, &this->address.sin_addr);

    assert( (this->listenfd = socket(AF_INET,SOCK_STREAM,0) ) >=0 );

    //端口复用
    int reuse = 1;
    setsockopt(this->listenfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));

    assert( bind(this->listenfd,(struct sockaddr*)&this->address,sizeof address) != -1 );
    
    assert( listen(this->listenfd,5) != -1 );

    assert ( (m_epollfd = epoll_create(5)) != -1  );

    //epoll_ctl(m_epollfd,EPOLL_CTL_ADD,listenfd,&ev);
    add_epoll(listenfd);

    socketpair(PF_UNIX,SOCK_STREAM,0,pipe);  //创建管道

    setnonblocking(pipe[1]);  //设置写管道非阻塞

    add_epoll(pipe[0]);

    add_sig(SIGALRM,send_sig);  

    add_sig(SIGTERM,SIG_IGN);
}


void server::Eventloop(){
    
    while( true ){
        int nfds = epoll_wait(m_epollfd,events,MAXEVENTS,-1);

        for(int i=0; i<nfds; ++i){
            int temp_socket = events[i].data.fd;
            if(temp_socket == this->listenfd){   //新的连接
                struct sockaddr_in cnt_addr;
                socklen_t len = sizeof cnt_addr;
                int cnt_fd = accept(listenfd, (struct sockaddr*)&cnt_addr, &len);

                user* tmp = new user("匿名",cnt_fd,cnt_addr, m_epollfd);
                this->user_map.insert({temp_socket,tmp});

                util_timer* new_utie_timer = new util_timer();
                
                new_utie_timer->user_data = tmp;
                new_utie_timer->fd = cnt_fd;
                time_t curr_time = time(NULL);
                new_utie_timer->expire = curr_time + 10;
                tmp->m_util_timer = new_utie_timer;
                lst_timer.add_timer(new_utie_timer);

                add_epoll(tmp);   //添加监听
            }

            else if(events[i].events & EPOLLIN){ //可读
                char buf[1024];
                memset(buf,'\0',1024);
                ssize_t n = read(temp_socket,buf,sizeof buf);  //阻塞读
                
                lst_timer.adjust_timer(user_map[temp_socket]->m_util_timer);

                if(n == 0){   //关闭连接
                    rm_epoll(this->user_map[temp_socket]);
                    lst_timer.del_timer(user_map[temp_socket]->m_util_timer);  //删除定时器
                    //set.erase(temp_socket);
                    user_map.erase(user_map.find(temp_socket));
                }
                else if(n < 0)
                    continue;
                else{
                    //有消息来了
                    if(user_map[temp_socket]->login == true){
                        handle_msg(buf, temp_socket);
                        continue;
                    }
                    //set.insert(temp_socket);
                    user* u = this->user_map[temp_socket]; //此时的buf存的是名字了
                    u->login = true;
                    if(u->setName(buf))
                        printf("用户%s 登录成功\n",u->getName());
                }
            }
            else if( temp_socket == pipe[0] && (events[i].events & EPOLLIN) ) {   //管道信号时间
                int sig;
                char signals[1024];
                ssize_t  ret = recv(pipe[0], signals, sizeof signals, 0);
                if(ret <= 0)
                    continue;
                for(int i = 0; i < ret; ++i){
                    switch (signals[i]){
                        case SIGALRM:
                            lst_timer.tick();
                    }
                }
            }
            else{
                printf("do other something!\n");
            }

        }

    } 


}

server::~server(){

}
