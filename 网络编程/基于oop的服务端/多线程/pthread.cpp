#include<stdio.h>
#include"Addrinit.h"
#include"check.h"
#include"ssock.h"
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<stdio.h>
void* function(void* arg){
    printf("HAVE NEW  CLIENT CONNECT AND CREATE NEW THREAD\n");
    printf("当前线程pthread_id为:%lu\n",pthread_self());
    int cnt_fd = *(int*)arg; //假设现在arg = 6
    char buf[4096];
    memset(buf,0,sizeof(buf));
    while(1){
        memset(buf,0,sizeof(buf));
        ssize_t n = recv(cnt_fd,buf,sizeof(buf),0);
        if(n<=0){
            pthread_exit(NULL);
            break;
        }
        printf("come from message:%s\n",buf); 
        write(cnt_fd,buf,strlen(buf));
    }
    close(cnt_fd);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc,char *argv[]){
    if(argc!=2){
        perror("main error\n");
        exit(1);
    }
    Socket *server = new Socket();
    server->s_bind(new Addrinit(atoi(argv[1])));
    server->s_listen();
    pthread_t thread_id;
    while(1){
        int cnt_fd = server->s_accept(new Addrinit());
        pthread_create(&thread_id,NULL,function,&cnt_fd);
        
        pthread_detach(thread_id);  //将线程设置为脱离属性
    }
    delete(server);
    return 0;
}