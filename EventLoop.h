#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H
#include<sys/types.h>
#include<vector>
#include<unistd.h>
#include<sys/eventfd.h>
#include<boost/function.hpp>
#include"Base/MutexLock.h"
using namespace std;
class Channel;
class EventLoop
{
public:
	typedef boost::function<void()> func;
	EventLoop();
	~EventLoop();
        bool isInLoopThread() const { return threadId_ == gettid(); }
	void loop();
	void do_loop();
        void runInLoop(const func& cb);
        void queueInLoop(const func& cb);
        void wakeup();
        void doPendingFunctors();//处理functor数组中的函数
	Channel* get_channel()
	{
		return this->channel;
	}
	int get_wakeupfd()
	{
		return this->wakeupfd;
	}
	static MutexLock mutex_test;
private:
	const pid_t threadId_;
	Channel* channel;
	int wakeupfd;
	vector<func> functor;
	MutexLock mutex_;
	bool callingPendingFunctors_;
};

#endif
