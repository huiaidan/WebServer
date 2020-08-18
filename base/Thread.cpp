#include"Thread.h"
#include"error.h"
#include<pthread.h>
#include<unistd.h>
#include"MutexLock.h"
using namespace std;
typedef boost::function<void()> ThreadRout;

MutexLock Thread::mutex_all;
void Thread::thread_start()
{
	int ret=pthread_create(&this->thread_id,NULL,thread_callback,q_ptr);
	if(ret!=0)
		perr_exit("thread_start error");
}

void Thread::thread_join()
{
	int ret=pthread_join(this->thread_id,NULL);
	if(ret!=0)
		perr_exit("thread_join error");
}

void* Thread::thread_callback(void* arg)
{
	while(1){
	TaskQueue* task_queue=static_cast<TaskQueue*>(arg);	
	MutexLockGuard lock(mutex_all);
        ThreadRout task=task_queue->put_out();
	task();
	}
	return NULL;
}

