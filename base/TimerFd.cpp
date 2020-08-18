#include"TimerFd.h"

void TimerFd::create_timerfd(int& timerfd_)
{
	timerfd_=timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
	this->timerfd=timerfd_;
}










