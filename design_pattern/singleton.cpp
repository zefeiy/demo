#include<iostream>
#include<pthread.h>

//Hunger-han model(普遍更实用)
//不涉及到修改操作, 线程安全, 多个线程同时获取对象是没有问题的.
class TaskQueue{

public:
    ~TaskQueue(){};

    static TaskQueue* getobj(){
        return Taskq;
    }

private:
    TaskQueue() = default;
    TaskQueue(const TaskQueue& taskq){};
    TaskQueue& operator=(const TaskQueue& taskq){};
    static TaskQueue* Taskq;
};
TaskQueue* TaskQueue::Taskq = new TaskQueue();


/*Lazy modle
当需要使用时再进行创建对象，存在了一个修改操作，线程不安全
{              处理方法
            1. 互斥锁保护(效率不足)
            2. 双重判断保护(可行)
}   

*/
class singleton_Lazy{

public:
    ~singleton_Lazy(){}
    singleton_Lazy(const singleton_Lazy& t) = delete;
    singleton_Lazy& operator=(const singleton_Lazy& t) = delete;

    singleton_Lazy* getobj(){
        if(obj == nullptr){   //双重判断保护
            pthread_mutex_lock(&m_mutex);
            if(obj == nullptr){
                obj = new singleton_Lazy();
                pthread_mutex_unlock(&m_mutex);
            }
        }
        return obj;
    }

    
private:
    static singleton_Lazy* obj;
    static pthread_mutex_t m_mutex;
    singleton_Lazy(){
        pthread_mutex_init(&m_mutex,nullptr); 
    };
};

singleton_Lazy* singleton_Lazy::obj = nullptr;
pthread_mutex_t singleton_Lazy::m_mutex;


int main(){


}