#include"ThreadPool.h"
#include<iostream>
#include"TaskQueue.h"
#include<iostream>
using namespace std;
ThreadPool::ThreadPool(int threadnum_,TaskQueue* task_queue_):
		threadnum(threadnum_),
		task_queue(task_queue_)
	{}
void ThreadPool::threadpool_init()//初始化指定数量的线程
{
	for(int i=0;i<this->threadnum;i++){
		threads.push_back(Thread(task_queue));
		threads[i].thread_start();
	}	
}

void ThreadPool::start()
{
	threadpool_init();
}
