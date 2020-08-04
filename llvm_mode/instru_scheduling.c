#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>//ipc
#include <sys/shm.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>

void srand (unsigned int seed);

//打印当前的线程调度策略
static int get_thread_policy(pthread_attr_t *attr)
{
  int policy;
  pthread_attr_getschedpolicy(attr,&policy);
  switch(policy)
  {
  case SCHED_FIFO:
    printf("policy= SCHED_FIFO\n");
    break;
  case SCHED_RR:
    printf("policy= SCHED_RR\n");
    break;
  case SCHED_OTHER:
    printf("policy=SCHED_OTHER\n");
    break;
  default:
    printf("policy=UNKNOWN\n");
    break;
  }
  return policy;
}
 
//打印当前调度策略线程的最高和最低优先级
static void show_thread_priority(pthread_attr_t *attr,int policy)
{
  int priority = sched_get_priority_max(policy);
  printf("max_priority=%d\n",priority);
  priority= sched_get_priority_min(policy);
  printf("min_priority=%d\n",priority);
}
 
//打印当前线程的优先级
static int get_thread_priority(pthread_attr_t *attr)
{
  struct sched_param param;
  pthread_attr_getschedparam(attr,&param);
  printf("priority=%d\n",param.__sched_priority);
  return param.__sched_priority;
}
 
//设置线程线程的调度策略
static void set_thread_policy(pthread_attr_t *attr,int policy)
{
  pthread_attr_setschedpolicy(attr,policy);
}

void __attribute__((constructor)) traceBegin(void) {
	srand((unsigned)time(NULL));
}

void __attribute__((destructor)) traceEnd(void) {
	;
}

void __cyg_profile_func_enter(void *func, void *caller) {
	printf("\n\nentry %p %p\n", func, caller);
	

}

void __cyg_profile_func_exit(void *func, void *caller) {
  printf("exit %p %p\n", func, caller);
}

void instru_sched(){
  pthread_attr_t attr;
	struct sched_param sched;
	int priority;
	pthread_attr_init(&attr);

	if (rand()%10 < 2){
		//设置线程的调度属性为SCHED_FIFO
    printf("set SCHED_FIFO policy:\n");  
    set_thread_policy(&attr,SCHED_FIFO);
    sched.sched_priority = rand()%100;
    pthread_attr_setschedparam(&attr,&sched);
    priority = get_thread_priority(&attr);
	} else {
    //设置线程的调度属性为SCHED_RR
    printf("set SCHED_RR policy:\n");
    set_thread_policy(&attr,SCHED_RR);
    sched.__sched_priority = rand()%100;
    pthread_attr_setschedparam(&attr,&sched);
    priority = get_thread_priority(&attr);
    usleep(rand()%1000);
  }
	printf("\n");

}