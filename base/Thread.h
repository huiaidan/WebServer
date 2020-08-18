#ifndef _THREAD_H
#define _THREAD_H
#include<pthread.h>
#include<set>
#include"TaskQueue.h"
#include<iostream>
#include"MutexLock.h"

using namespace std;
class Thread
{
public:
	Thread(TaskQueue* task_queue):q_ptr(task_queue)
	{}
	~Thread()
	{}
	void thread_start();
	void thread_join();
	TaskQueue* get_qptr()
	{
		return q_ptr;
	}
        static MutexLock mutex_all;
private:
	static void* thread_callback(void*);
	pthread_t thread_id;
	TaskQueue* q_ptr;
	
};




#endif

