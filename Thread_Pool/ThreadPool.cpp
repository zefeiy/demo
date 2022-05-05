#include "ThreadPool.h"

ThreadPool::ThreadPool(){
    this->Task_q = new TaskQueue();       //实例化任务队列

    do{
        //this.threadIDs = new pthread_t[max];
        this->threadIDs = new pthread_t[20];   //初始化最大线程数

        if(this->threadIDs == NULL){
            std::cout<<"malloc threadIDs fail.."<<std::endl;
            break;
        }
        memset(this->threadIDs,0,sizeof(pthread_t)*20);

        //初始化条件变量和互斥锁
        if(pthread_mutex_init(&this->m_mutexPool,NULL)!=0 || pthread_cond_init(&this->notEmpty,NULL)!=0){
            std::cout<<"mutex or condition init faile.."<<std::endl;
            break;
        }

        this->shutdown = false;  //开启线程池 

        pthread_create(&this->managerID,NULL,manager,this);   //创建管理者线程

        for(int i=0;i<10;i++){
            pthread_create(&this->threadIDs[i],NULL,work,this);
        }
        printf("初始化线程池完成!!\n");
        return;

    }while(0);

    //释放资源
    if(threadIDs != NULL){
        delete[] threadIDs;
        threadIDs = NULL;
    }
    if(Task_q!=NULL){
        delete Task_q;
        Task_q = NULL;
    }
    
}

void* ThreadPool::manager(void*arg){

    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    printf("管理者线程创建!!!\n");
    while(!pool->shutdown){
        sleep(3);
    }
    return NULL;
}



void* ThreadPool::work(void* arg){
    ThreadPool* pool = static_cast<ThreadPool*> (arg);
    //printf("工作者线程创建一个了!!!\n");
    while(1){
        
        /*
        if(pool->shutdown==false){
            printf("线程池未关闭\n");
            printf("尝试获得任务\n");
        }
        */

        pthread_mutex_lock(&pool->m_mutexPool);
        while(pool->Task_q->taskNumber()==0 && !pool->shutdown)
            pthread_cond_wait(&pool->notEmpty,&pool->m_mutexPool);

        struct Task temp = pool->Task_q->getTask();
        //this->busyNum++;
        pthread_mutex_unlock(&pool->m_mutexPool);
        printf("获得到任务啦，准备执行\n");

        temp.call_back(temp.arg);
        delete (int*)temp.arg;
        
    }

    return NULL;

}


void ThreadPool::addTask(struct Task task){
    pthread_mutex_lock(&this->m_mutexPool);
    if(this->shutdown){  //线程池关闭状态，不可添加任务
        pthread_mutex_unlock(&this->m_mutexPool);
        return;
    }
    this->Task_q->addTask(task);
    if(this->Task_q->taskNumber()!=0)
        printf("添加任务成功\n");
    pthread_cond_signal(&this->notEmpty);

    pthread_mutex_unlock(&this->m_mutexPool);

}



ThreadPool::~ThreadPool(){
    this->shutdown = true;
    if(this->Task_q!=NULL){
        delete Task_q;
        Task_q = NULL;
    }
    if(this->threadIDs!=NULL){
        delete[] threadIDs;
        threadIDs = NULL;
    }
    pthread_mutex_destroy(&this->m_mutexPool);

}

