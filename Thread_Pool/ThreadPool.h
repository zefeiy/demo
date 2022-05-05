#pragma once
#include "TaskQueue.h"
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include<stdio.h>
class ThreadPool{
public:
    //typedef void* (*call_back)(void*);

    ThreadPool();
    ~ThreadPool();

    void addTask(struct Task task);   

    int getBusyNum();         //正在工作的线程数

    int getAliveNum();   //存活的线程数

public:

    static void* manager(void* arg);    //管理者线程，负责动态的维护线程池状态变化；

    static void* work(void* arg);       //工作者线程

    //void threadExit();

private:
    TaskQueue* Task_q;
    pthread_t managerID;
    pthread_t* threadIDs;

    int minNum;   //最小线程数
    int maxNum;        //最大线程数
    int busyNum;        //正在工作的线程数
    int liveNum;        //存活的线程数
    int exitNum;        //退出线程数
    pthread_mutex_t m_mutexPool;   
    pthread_cond_t notEmpty;

    bool shutdown;    //判断线程池是否关闭

};