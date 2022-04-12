#include"Addrinit.h"
#include<string.h>

Addrinit::Addrinit():addr_len(sizeof(gen_addr)){
      memset(&this->gen_addr,0,sizeof(this->gen_addr));
}

Addrinit::Addrinit(uint16_t port):addr_len(sizeof(gen_addr)){
    memset(&this->gen_addr,0,sizeof(this->gen_addr));
    gen_addr.sin_family = AF_INET;
    gen_addr.sin_port = htons(port);
    gen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

Addrinit::~Addrinit(){

}