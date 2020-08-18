#include<pthread.h>
#include"EventLoopThreadPool.h"
#include"Base/TaskQueue.h"
#include"EventLoop.h"
using namespace std;
EventLoopThreadPool::EventLoopThreadPool(int threadnum_,TaskQueue* q):
		threadnum(threadnum_),
		q_ptr(q)
	{}
void EventLoopThreadPool::start(vector<EventLoop*> &loops_)
{
	for(int i=0;i<this->threadnum;i++){
		EventLoopthreads.push_back(EventLoopThread(this->q_ptr));
		EventLoop* ptr=EventLoopthreads[i].thread_start();
		loops.push_back(ptr);
	}	
	loops_=loops;
}

