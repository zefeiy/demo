#pragma once
#include<stdio.h>
#include<netinet/in.h>

class Addrinit{
public:
    socklen_t addr_len;
    struct sockaddr_in gen_addr;
    Addrinit(uint16_t port);
    Addrinit();
    ~Addrinit();
};