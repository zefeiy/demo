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
            2. 双重检查判断保护(有个不安全的原因)   ------------------------>{ obj = new singleton_Lazy(); 
                                                                                    这句代码分三步
                                                                            1. 申请一块内存未初始化的空间
                                                                            2. 在申请的空间上创建对象     ->>> 如果执行顺序是 1，3,2 在3之后引发线程切换，便造成空悬指针隐患
                                                                            3. 保存申请的空间指针到obj  
                                                                        解决： 使用c++11 的 std::automic 原子变量保存指针，当操作指针是，保证原子操作，不引发线程切换
                                                                                                    }
            3. 使用静态局部变量解决懒汉模式的线程安全问题(可行性最高)
                                                                        


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


//3. 使用静态局部变量解决懒汉模式的线程安全问题

class singleton_Lazy_static{
public:
    ~singleton_Lazy_static(){}
    singleton_Lazy_static(const singleton_Lazy_static& t) = delete;
    singleton_Lazy_static& operator=(const singleton_Lazy_static& t) = delete;

    static singleton_Lazy_static* getobj(){

        static singleton_Lazy_static obj;
        return &obj;
    }

private:
    singleton_Lazy_static() = default;
};


int main(){


}