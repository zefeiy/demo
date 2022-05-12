#include "TaskQueue.h"
class Taskqueue;

TaskQueue::TaskQueue(){
    pthread_mutex_init(&this->queue_mutex,NULL);  //初始化一下任务队列的互斥锁
}

TaskQueue::~TaskQueue(){
    pthread_mutex_destroy(&this->queue_mutex);
}

void TaskQueue::addTask(struct Task task){
    pthread_mutex_lock(&this->queue_mutex);
    this->Task_q.push(task);
    pthread_mutex_unlock(&this->queue_mutex);
}

struct Task TaskQueue::getTask(){
    pthread_mutex_lock(&this->queue_mutex);
    struct Task t;
    if(!this->Task_q.empty()){
        t = this->Task_q.front();
        this->Task_q.pop();
    }
    pthread_mutex_unlock(&this->queue_mutex);
    return t;
}




