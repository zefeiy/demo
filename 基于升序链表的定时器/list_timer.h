#include<time.h>
#include<netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>


const int BUFFER_SIZE = 1024;


class util_timer;

struct clinet_data{   //用户数据结构
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer* timer;
};


//定时器类
class util_timer{
public:
    util_timer():prev(NULL),next(NULL){};

public:
    util_timer *prev;  //前驱节点
    util_timer *next;   //后继节点

    time_t expire;
    clinet_data* user_data;
    void (*func)(clinet_data*);   //定时器处理回调函数

};

//定时器链表类
class sort_list{
public:
    sort_list():head(NULL),tail(NULL){};

    ~sort_list();   //析构释放所有定时器

    void add_timer(util_timer* timer);   

    void adjust_timer(util_timer* timer);

    void del_timer(util_timer* timer);

    void tick();

private:
    util_timer* head;
    util_timer* tail;

};
