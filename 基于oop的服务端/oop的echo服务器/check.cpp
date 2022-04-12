#include"check.h"
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

void check(bool ret,const char* msg){
	if(ret){
		perror(msg);
		exit(1);
	}	
}
