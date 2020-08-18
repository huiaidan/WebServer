#include"EventLoopThread.h"
#include"error.h"
#include<pthread.h>
#include"Epoll.h"
#include"Channel.h"
#include"EventLoop.h"
using namespace std;
MutexLock EventLoopThread::event_mutex;
MutexLock EventLoopThread::mutex;
Condition EventLoopThread::cond(EventLoopThread::mutex);
EventLoopThread::EventLoopThread(TaskQueue* q):
		q_ptr(q),
		loop(NULL)
	{}
EventLoop* EventLoopThread::thread_start()
{
	
	int ret=pthread_create(&this->thread_id,NULL,Callback,this);
	if(ret!=0)
		perr_exit("thread_start error");
	MutexLockGuard lock(EventLoopThread::mutex);
	cond.wait();
	return loop;
}

void EventLoopThread::thread_join()
{
	int ret=pthread_join(this->thread_id,NULL);
	if(ret!=0)
		perr_exit("thread_join error");
}

void* EventLoopThread::Callback(void* arg)
{
        EventLoopThread* thread=static_cast<EventLoopThread*>(arg);
	EventLoop loop_;
	thread->loop=&loop_;
	{
	MutexLockGuard lock(EventLoopThread::mutex);
	cond.notify();
	}
	thread->loop->loop();
	return NULL;
}
