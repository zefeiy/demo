#include<sys/time.h>
#include<signal.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>

void function(int signo){
    int fd = open("mydemon.log",O_RDWR | O_CREAT | O_APPEND,0755);
    if(fd<0) return;

    time_t t;
    time(&t);
    char *p = ctime(&t);
    write(fd,p,strlen(p));
    close(fd);
    return;
}

int main(){
    pid_t pid = fork();
    if(pid!=0)
        exit(1);

    setsid();
    umask(0000);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = function;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);

    struct itimerval tm;
    tm.it_interval.tv_sec = 2;
    tm.it_interval.tv_usec = 0;
    tm.it_value.tv_sec = 3;
    tm.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL,&tm,NULL);

    printf("hello world\n");
    while(1){
        sleep(1);
    }

    return 0;
}