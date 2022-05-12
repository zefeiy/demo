#pragma once
#include<queue>
#include<pthread.h>

struct Task{
    void* arg;     //任务，一个参数
    void* (*call_back)(void *arg);   //一个回调函数
};


class TaskQueue{
public:
    TaskQueue();
    ~TaskQueue();

void addTask(struct Task task);  //添加任务(同步)


Task getTask();  //取出一个任务(同步)

inline int taskNumber(){
    return Task_q.size();  //获取当前任务个数(同步)
}

private:
    pthread_mutex_t queue_mutex;

    std::queue<struct Task> Task_q;

};