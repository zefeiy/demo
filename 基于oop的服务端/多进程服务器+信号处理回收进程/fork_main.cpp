#include<stdio.h>
#include"Addrinit.h"
#include"check.h"
#include"ssock.h"
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<sys/wait.h>


void kill_fork(int sig){
	int status;
	waitpid(-1,&status,WNOHANG);
	printf("子进程以通过信号处理器回收完成!\n");	
}

int main(int argc,char *argv[]){
    if(argc!=2){
        perror("error");
        exit(1);
    }

    struct sigaction act;
	act.sa_handler = kill_fork;
	act.sa_flags = SA_RESTART;   //重新启动被打断的系统调用 ！！
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD,&act,0);  // 完成注册信号，当有进程终止时



    Socket *server = new Socket();
    Addrinit *ser_add = new Addrinit(atoi(argv[1]));
    server->s_bind(ser_add);
    server->s_listen();

    char sip[16];
    while(1){
        memset(sip,0,sizeof(sip));
        Addrinit *cnt_add = new Addrinit();
        int cnt_fd = server->s_accept(cnt_add);
        Socket *client = new Socket(cnt_fd);
        printf("new clinet:[%s] connect use_port :%d\n",inet_ntop(AF_INET,&cnt_add->gen_addr.sin_addr.s_addr,sip,sizeof(sip)),ntohs(cnt_add->gen_addr.sin_port));
        pid_t pid = fork();
        if(pid<0){
            perror("fork error");
        }
        if(pid>0){
            delete (client); //关闭新的套接字
            delete(cnt_add);
        }

        else if(pid==0){
            delete(server);
            delete(ser_add);
            char buffer[1024];
            ssize_t n;
            while(n = recv(client->getfd(),buffer,sizeof(buffer),0)){
                buffer[n] = '\0';
                printf("come from use_id %d message:%s\n",ntohs(cnt_add->gen_addr.sin_port),buffer);

                write(client->getfd(),buffer,strlen(buffer));
            }
            delete (client);
            break;
        }
    }
    delete(server);
    delete(ser_add);
    return 0; 
}
