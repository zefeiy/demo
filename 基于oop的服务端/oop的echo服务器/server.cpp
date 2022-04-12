#include"ssock.h"
#include"Addrinit.h"
#include"check.h"
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc,char*argv[]){
    if(argc!=2){
        perror("error");
        exit(1);
    }
    Socket *server = new Socket();
    Addrinit *ser_addr = new Addrinit(atoi(argv[1]));
    server->s_bind(ser_addr);
    server->s_listen();

    int cnt_fd = server->s_accept(new Addrinit());
    Socket *cnt_sock = new Socket(cnt_fd);
    if(cnt_sock->getfd()!=-1)
        printf("HAVE CLIENT CONNECT!\n");
    char buf[1024];
    ssize_t n;
    while(n = recv(cnt_sock->getfd(),buf,sizeof(buf),0)){
        buf[n] = '\0';
        printf("MES COME FROM CLINET:%s\n",buf);

        write(cnt_sock->getfd(),buf,strlen(buf));
    }
    
    delete(server);
    delete(cnt_sock);
    return 0;
}