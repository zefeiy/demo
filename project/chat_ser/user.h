#ifndef USER
#define USER


#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/epoll.h>
#include"lst_timer.h"

class util_timer;
//class server;

class user{
private:
    char* _name;
    int _fd;
    int _epollfd;
    struct sockaddr_in _address;
    

public:
    bool login;

public:
    user(char* name,int fd,struct sockaddr_in address, int epollfd):_name(name),_fd(fd),_address(address), _epollfd(epollfd),login(false){};
    ~user(){};

    void close_conn(){
        epoll_ctl(_epollfd, EPOLL_CTL_DEL, this->_fd, 0);
        //server::set.erase(_fd);
        server::user_map.erase(user_map.find(_fd));
        close(_fd);
    }

    util_timer* m_util_timer;
    

    inline bool setName(char* new_name){
        strcpy(_name,new_name);
        return true;
    }

    inline char* getName(){
        return this->_name;
    }

    inline int getfd(){
        return this->_fd;
    }

};

#endif