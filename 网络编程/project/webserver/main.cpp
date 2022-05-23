#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>
#include "locker.h"
#include "threadpool.h"
#include "http_conn.h"
#include <iostream>
 
#define MAX_FD 65536
#define MAX_EVENT_NUMBER 10000
 
static int all_process = 0;

extern int addfd( int epollfd, int fd, bool one_shot );
extern int removefd( int epollfd, int fd );
 
void addsig( int sig, void(handler)(int), bool restart = true ){
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    if( restart ){
        sa.sa_flags |= SA_RESTART;      //重启被信号打断的系统调用
    }
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );
}
 
void show_error( int connfd, const char* info ){
    printf( "%s", info );
    send( connfd, info, strlen( info ), 0 );
    close( connfd );
}
 
 
int main( int argc, char* argv[] ){
    if( argc <= 2 ){
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );
 
    /*忽略SIGPIPE信号，，，当服务端向一个已经关闭连接的client写数据是产生sigpipe信号，默认操作为退出进程，因此要忽略该信号*/ 
    addsig( SIGPIPE, SIG_IGN );
 
    /*创建线程池*/
    threadpool< http_conn >* pool = NULL;  //创建线程池
    try{
        pool = new threadpool< http_conn >;
    }
    catch( ... ){
        return 1;
    }
 
    /*预先为每个可能的客户连接分配一个http_conn对象*/
    http_conn* users = new http_conn[ MAX_FD ];   //创建对象
    assert( users );
    int user_count = 0; //客户端的数量
 
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );
    struct linger tmp = { 1, 0 };    //close()立刻返回，但不会发送未发送完成的数据，而是通过一个REST包强制的关闭socket描述符，即强制退出。
    setsockopt( listenfd, SOL_SOCKET, SO_LINGER, (void*)&tmp, sizeof( tmp ) );  //设置套接字属性
 
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
 
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret >= 0 );
 
    ret = listen( listenfd, 5 );
    assert( ret >= 0 );
 
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd, false );  //监听描述符不要设置EPOLLONESHOT事件
    http_conn::m_epollfd = epollfd;
 
    while( true ){
        int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if ( ( number < 0 ) && ( errno != EINTR ) ){      //系统调用被中断errno==EINTR
            printf( "epoll failure\n" );
            break;
        }

        for ( int i = 0; i < number; i++ ){
            int sockfd = events[i].data.fd;
            if( sockfd == listenfd ){
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                if ( connfd < 0 ){
                    printf( "errno is: %d\n", errno );
                    continue;
                }
                if( http_conn::m_user_count >= MAX_FD ){ //当前连接用户数超过了最大文件描述符数量，则不予建立该连接
                    show_error( connfd, "Internal server busy" );
                    continue;
                }
            /*初始化客户连接*/
                users[connfd].init( connfd, client_address );
            }                                                                     //EPOLLERR 这一侧操作不当
            else if( events[i].events & ( EPOLLRDHUP | EPOLLHUP | EPOLLERR ) ){  //EPOLLRDHUP客户端主动断开连接时产生，因此不必在多一次read==0 系统调用来判断连接关闭
            	/*如果有异常，直接关闭客户连接*/    //EPOLLHUP  当套接字不可能监听到事件时产生，例如，未调用listen就将listenfd 添加到epool事件树中，epool不会返回任何的事件，而是产生了EPOLLHUP
                users[sockfd].close_conn();                 //因为服务器并不知道客户端发来的四元组的存在        
            }
            else if( events[i].events & EPOLLIN ){
            	/*根据读的结果，决定是将任务添加到线程池，还是关闭连接*/
                if( users[sockfd].read() ){
                    pool->append( users + sockfd );
                }
                else{
                    users[sockfd].close_conn();
                }
            }
            else if( events[i].events & EPOLLOUT ){
            	/*根据写的结果，决定是否关闭连接*/
                if( !users[sockfd].write() ){
                    users[sockfd].close_conn();
                    all_process++;
                }
            }
            else{
                
            }
        }
    }
 
    close( epollfd );
    close( listenfd );
    delete [] users;
    delete pool;
    printf("共处理请求:%d",all_process);
    return 0;
}