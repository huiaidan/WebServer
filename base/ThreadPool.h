#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include<pthread.h>
#include"Thread.h"
#include<vector>
#include"MutexLock.h"
class TaskQueue;
class ThreadPool
{
public:
	ThreadPool(int threadnum_,TaskQueue* task_queue_);
	~ThreadPool()
	{}
	void threadpool_init();//初始化指定数量的线程
	void start();

private:
	int threadnum;
	vector<Thread> threads;
	TaskQueue* task_queue;
	MutexLock mutex;
};

#endif
