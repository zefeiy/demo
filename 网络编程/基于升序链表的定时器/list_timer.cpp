#include"list_timer.h"



sort_list::~sort_list(){
    util_timer* temp = head;
    while(head->next!=NULL){
        head = head->next;
        delete temp;
        temp = head;
    }
    delete head;
    head = NULL;
    temp = NULL;
}

void sort_list::add_timer(util_timer* timer){
    if(timer == NULL)
        return;
    if(head==NULL){
        head = timer;
        tail = timer;
        head->next = NULL;
        head->prev = NULL;
        tail->next = NULL;
        tail->prev = NULL;
        return;
    }
    if(timer->expire<head->expire){  //插入头节点
        timer->next = head;
        timer->prev = NULL;
        head->prev = timer;
        this->head = timer;
        return;
    }

    if(timer->expire>tail->expire){   //插入尾节点
        tail->next = timer;
        timer->prev = tail;
        timer->next = NULL;
        this->tail = timer;
        return;
    }


    util_timer* temp = this->head;  
    while(timer->expire>temp->expire)
        temp = temp->next;
    timer->next = temp;
    timer->prev = temp->prev;
    temp->prev->next = timer;
    temp->prev = timer;                       
}

void sort_list::adjust_timer(util_timer* timer){
    //调整顺序
    if(timer==NULL)
        return;
    //util_timer* temp = timer->next;

    //if(timer == tail || (timer->expire < temp->expire))
        //return;
    
    if((timer == head && timer==tail) || timer==tail)
        return;
    
    if(timer == head){
        head = head->next;
        head->prev = NULL;
        tail->next = timer;
        timer->prev = tail;
        timer->next = NULL;
        tail = timer; 
        return;
    }
        
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    timer->prev = tail;
    timer->next = NULL;
    tail = timer;

    //del_timer(timer);
    //add_timer(timer);
    
}


void sort_list::del_timer(util_timer* timer){
    if(timer==NULL) return;
    
    if((timer==head) && (timer==tail)){
        delete timer;
        timer = NULL;
        head = NULL;
        tail = NULL;
        return;
    }

    if(timer = head){
        head = head->next;
        head->prev = NULL;
        delete timer;
        timer = NULL;
        return;
    }

    if(timer = tail){
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
        timer = NULL;
        return;
    }
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
    timer = NULL;

}

void sort_list::tick(){  //定时检测
    if(head == NULL)
        return;
    printf("开始定时检测！\n");

    time_t cur = time(NULL);
    util_timer* temp = head;
    
    while(this->head != NULL){
        if(cur < temp->expire)
            break;
        
        temp->func(temp->user_data);
        head = temp->next;

        if(head!=NULL)
            head->prev = NULL;

        delete temp;
        temp = head;
    }

}