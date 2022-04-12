#include"ssock.h"
#include<sys/socket.h>
#include"check.h"
#include"Addrinit.h"
#include<unistd.h>

Socket::Socket():m_fd(-1){
    this->m_fd = socket(AF_INET,SOCK_STREAM,0);
    check(this->m_fd==-1,"CREATE LISTEN SOCKET ERRRO\n");
}

Socket::Socket(int fd):m_fd(fd){

    check(this->m_fd==-1,"CREATE CLIENT SOCKET ERRRO\n");
}

void Socket::s_bind(Addrinit *addr){
    check(bind(this->m_fd,(struct sockaddr*)&addr->gen_addr,addr->addr_len)==-1,"BIND SOCKET ERRRO\n");
}

void Socket::s_listen(){
    listen(this->m_fd,100);
}

int Socket::s_accept(Addrinit *cnt_addr){
    int cnt_fd = accept(this->m_fd,(struct sockaddr*)&cnt_addr->gen_addr,&cnt_addr->addr_len);
    check(cnt_fd==-1,"ACCEPT NEW CLIENT ERROR");
    return cnt_fd;
}

int Socket::getfd(){
    return this->m_fd;
}

Socket::~Socket(){
    if(this->m_fd!=-1)
        close(this->m_fd);
}


