#include"user.h"
#include"lst_timer.h"
#include"server.cpp"


int main(){

    server* start = new server("127.0.0.1",10000);
    
    start->init();

    start->Eventloop();

    delete start;

    return 0;
}