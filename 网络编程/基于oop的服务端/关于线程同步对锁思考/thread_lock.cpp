/*
1.创建锁  pthread_mutex_t mutex;
2.初始化锁  pthread_mutex_init(&mutex,锁的属性(NULL));
3.加锁，解锁  pthread_mutex_lock(&mutex);
				pthread_mutex_unlock(&mutex);
4.释放锁资源(销毁锁)  pthread_mutex_destory(&mutex);


容易造成死锁的常见问题
	1. 在一个线程当中，连续获取同一把锁，中间并没有释放锁操作
		使得线程被自己本身锁住（解决：记得释放锁）
	2. 线程A 持有A锁 ，想获取B锁，而线程B持有B锁，想获取A锁，造成
	一种互相等待对方释放锁的一种死锁状况。、

	解决方案： 每一个线程在持有锁时，想要获取另一把锁，必须先释放现
			持有的锁，即，一个线程不能同时持有两把锁，可避免死锁局面

	读写锁
		应用场景：主要是当写操作远远大于读操作时，读写锁可提高效率


*/

#include<stdio.h>
#include<pthread.h>
long long sum = 0;
pthread_mutex_t mutex;

void *sum1_thread(void *arg){
	for(int i=0;i<5000000;i++){
		pthread_mutex_lock(&mutex);
		sum++;
		pthread_mutex_unlock(&mutex);
	}	
	return NULL;		
}

void *sum2_thread(void* arg){
	for(int i=0;i<5000000;i++){
		pthread_mutex_lock(&mutex);
		sum++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
int main(){
	pthread_t ary[100];
	printf("sizeof long = %d\n",sizeof(long long));
	//pthread_mutex_t mutex;
	pthread_mutex_init(&mutex,NULL);

	for(int i=0;i<100;i++){
		if(i%2 == 0){
			pthread_create(&(ary[i]),NULL,sum1_thread,NULL);
		}
	
		else{
			pthread_create(&(ary[i]),NULL,sum2_thread,NULL);
		}
	}

	for(int i=0;i<100;i++){
		pthread_join(ary[i],NULL);
	}

	printf("result = %lld",sum);
	
	return 0;
}







