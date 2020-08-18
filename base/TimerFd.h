#ifndef _TIMERFD_H
#define _TIMERFD_H
#include<sys/timerfd.h>
#include"TimerQueue.h"
class TimerFd
{
public:
	TimerFd():timerfd(-2),timerqueue(new TimerQueue())
	{}
	~TimerFd()
	{}
        void create_timerfd(int& timerfd_);
	TimerQueue* get_timerqueue()
	{
		return this->timerqueue;
	}
private:
	int timerfd;
	TimerQueue* timerqueue;
};

#endif
