#ifndef _EVENTLOOPTHREADPOOL_H
#define _EVENTLOOPTHREADPOOL_H
#include<vector>
#include"EventLoopThread.h"
using namespace std;
//class EventLoopThread;
class TaskQueue;
class EventLoop;
class EventLoopThreadPool
{
public:
	EventLoopThreadPool(int threadnum_,TaskQueue* q);
	~EventLoopThreadPool()
	{}

	void start(vector<EventLoop*>& loops_);
private:
	int threadnum;
	vector<EventLoopThread> EventLoopthreads;
	vector<EventLoop*> loops;
	TaskQueue* q_ptr;
};

#endif
