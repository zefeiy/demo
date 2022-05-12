
#include"ThreadPool.h"
#include"TaskQueue.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void* test(void* arg){
    int number = *(int*)arg;
    printf("任务%d 执行中。。。。",number);
    sleep(1);
    printf("任务%d 执行结束。。。。",number);
    return NULL;
}


int main(){
    ThreadPool *Thread_pool = new ThreadPool();
    printf("开始\n");
    for(int i=0;i<100;i++){
        struct Task task;
        task.call_back = test;
        int *x = new int;
        *x = i;
        task.arg = x;
        //struct Task *temp = (struct Task*)test;
        Thread_pool->addTask(task);
        //printf("添加任务成功\n");
    }
    sleep(10);

    delete Thread_pool;

    return 0;
}