#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"

#define MAX_FD 65536   // 最大的文件描述符个数
#define MAX_EVENT_NUMBER 10000  // 监听的最大的事件数量
#define TIMESLOT 5  //超时时间

static int pipefd[2];
//static sort_timer_lst timer_lst;
//static http_conn users[ MAX_FD ]; // 一次最大的连接数量

// 添加文件描述符
extern void addfd( int epollfd, int fd, bool one_shot ); //在每一个连接中设置，添加描述符，同时要重置一下epolloneshop事件
extern void removefd( int epollfd, int fd );            

void addsig(int sig, void( handler )(int)){
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );
}

void sig_handlder(int sig){
    int save_errno = errno;
    int msg = sig;
    write(pipefd[1],(char*)& msg,1);
    errno = save_errno;
}

void timer_handler()
{
    // 定时处理任务，实际上就是调用tick()函数
    http_conn::timer_lst.tick();
    // 因为一次 alarm 调用只会引起一次SIGALARM 信号，所以我们要重新定时，以不断触发 SIGALARM信号。
    alarm(TIMESLOT);
}

int main( int argc, char* argv[] ) {
    
    if( argc <= 1 ) {
        EMlog(LOGLEVEL_ERROR,"run as: %s port_number\n", basename(argv[0]));   //argv[0]可能带路径，用basename转换
        return 1;
    }

    int port = atoi( argv[1] );
    addsig( SIGPIPE, SIG_IGN );

    threadpool< http_conn >* pool = NULL; //每一个任务类就是一条连接，也就是一个线程负责
    try {
        pool = new threadpool<http_conn>;
    } catch( ... ) {
        return 1;
    }


    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );

    int ret = 0;
    struct sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons( port );

    // 端口复用
    int reuse = 1;
    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    ret = listen( listenfd, 5 );
    

    // 创建epoll对象，和事件数组，添加
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    // 添加到epoll对象中
    addfd( epollfd, listenfd, false );
    http_conn::m_epollfd = epollfd;

    
    //新添加的
    //1.创建管道并设置信号处理函数
    ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);
    http_conn::setnonblocking(pipefd[1]);
    addfd(http_conn::m_epollfd,pipefd[0],false);
    addsig(SIGALRM,sig_handlder);
    alarm(TIMESLOT);

    http_conn* users = new http_conn[ MAX_FD ]; // 一次最大的连接数量

    while(true) {
        
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        
        if ( ( number < 0 ) && ( errno != EINTR ) ) {
            EMlog(LOGLEVEL_ERROR,"EPOLL failed.\n");
            break;
        }

        for ( int i = 0; i < number; i++ ) {
            
            int sockfd = events[i].data.fd;
            
            if( sockfd == listenfd ) { //新的连接
                
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                
                if ( connfd < 0 ) {
                    printf( "errno is: %d\n", errno );
                    continue;
                } 

                if( http_conn::m_user_count >= MAX_FD ) {
                    close(connfd);
                    continue;
                }
                users[connfd].init( connfd, client_address);   //初始化新的连接
            } 
            else if( (events[i].data.fd == pipefd[0]) && (events[i].events & EPOLLIN) ){   //处理信号
                int sig = 0;
                char signals[1024];
                ret = read(pipefd[0],signals,sizeof signals);
                if(ret <= 0)
                    continue;
                else{
                    for(int i=0; i<ret; ++i){
                        if(signals[i] == SIGALRM)
                            timer_handler();
                    }
                }
            }
            else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) ){
                EMlog(LOGLEVEL_DEBUG,"-------EPOLLRDHUP | EPOLLHUP | EPOLLERR--------\n");
                users[sockfd].close_conn();  //出现异常情况，直接断开连接
                http_conn::timer_lst.del_timer(users[sockfd].m_util_timer);
            } 
            else if(events[i].events & EPOLLIN) {  //可读事件
                EMlog(LOGLEVEL_DEBUG,"-------EPOLLIN-------\n\n");
                if(users[sockfd].read()) {   //将数据全部读出
                    pool->append(users + sockfd); //向池中添加任务
                } 
                else {
                    users[sockfd].close_conn();  //读失败，直接关闭连接,并移除定时器
                    http_conn::timer_lst.del_timer(users[sockfd].m_util_timer);
                }

            }  
            else if( events[i].events & EPOLLOUT ) {   //可写事件
                EMlog(LOGLEVEL_DEBUG, "-------EPOLLOUT--------\n\n");
                if( !users[sockfd].write() ) {      //写失败，直接关闭连接
                    users[sockfd].close_conn();
                    http_conn::timer_lst.del_timer(users[sockfd].m_util_timer);
                }
            }

        }
    }
    
    close( epollfd );
    close( listenfd );
    close(pipefd[0]);
    close(pipefd[1]);
    delete [] users;
    delete pool;
    return 0;
}