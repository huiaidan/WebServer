#ifndef _EVENTLOOPTHREAD_H
#define _EVENTLOOPTHREAD_H
#include<pthread.h>
#include<vector>
#include"Base/MutexLock.h"
#include"Base/Condition.h"
using namespace std;
class TaskQueue;
class HttpRequest;
class EventLoop;
class EventLoopThread
{
public:
	EventLoopThread(TaskQueue* q);
	~EventLoopThread()
	{}
	EventLoop* thread_start();
	void thread_join();
	TaskQueue* get_qptr()
	{
		return q_ptr;
	}
	static MutexLock event_mutex;
	static MutexLock mutex;
	static Condition cond;
private:
	vector<EventLoop*> vec;
        static void* Callback(void*);
	pthread_t thread_id;
	TaskQueue* q_ptr;
	EventLoop* loop;
	
};


#endif

