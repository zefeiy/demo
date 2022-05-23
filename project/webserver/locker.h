#ifndef LOCKER_H 
#define LOCKER_H 

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class sem{
private:
    sem_t m_sem ;
public:
    sem();
    ~sem();
    bool wait();    //原子操作 -1 值为0是 阻塞等待
    bool post();     //+1
};

sem::sem(){
    if ( sem_init(&m_sem, 0,0) != 0){
        throw std::exception();
    }
}

sem::~sem(){
    sem_destroy(&m_sem);
}

bool sem::wait(){
    return sem_wait(&m_sem) == 0 ;
}

bool sem::post(){
    return sem_post(&m_sem) == 0;
}


class locker{
private:
    pthread_mutex_t m_mutex;
public:
    locker();
    ~locker();
    bool lock();
    bool unlock();
};

locker::locker(){
    if ( pthread_mutex_init(&m_mutex, NULL) != 0){
        throw std::exception();
    }
}

locker::~locker(){
    pthread_mutex_destroy(&m_mutex);
}

bool locker::lock(){
    return pthread_mutex_lock(&m_mutex) == 0 ;
}

bool locker::unlock(){
    return pthread_mutex_unlock(&m_mutex) == 0 ;
}

class cond{
private:
    /* data */
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
public:
    cond();
    ~cond();
    bool wait();
    bool signal();
};

cond::cond(){
    if ( pthread_mutex_init(&m_mutex,NULL) != 0 ){
        throw std::exception();
    }
    if ( pthread_cond_init(&m_cond, NULL ) != 0 ){
        pthread_mutex_destroy(&m_mutex);
        throw std::exception();
    }
}

cond::~cond(){
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}

bool cond::wait(){
    int ret = 0 ;
    pthread_mutex_lock(&m_mutex);
    ret = pthread_cond_wait(&m_cond, &m_mutex);
    pthread_mutex_unlock(&m_mutex);
    return ret == 0 ;
}

bool cond::signal(){
    return pthread_cond_signal(&m_cond) == 0;
}

#endif