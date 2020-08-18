#ifndef _TIMER_H
#define _TIMER_H
#include<sys/timerfd.h>
#include<memory>
#include"Base/Timestamp.h"
#include<boost/function.hpp>
class TcpConnection;
class TimerQueue;
class Timer
{
public:
	typedef boost::function<void()> Task;
	Timer():timestamp()
	{
		timerfd=-2;
	}
	~Timer()
	{}
        void timer_init_s(shared_ptr<TcpConnection> conn,int timer_fd,int set_time,Task task);//指定由哪一个定时器模块管理，指定定时时间，指定定时事件
        void timer_init_ms(shared_ptr<TcpConnection> conn,int timer_fd,int set_time,Task task);//指定由哪一个定时器模块管理，指定定时时间，指定定时事件
        void timer_init_us(shared_ptr<TcpConnection> conn,int timer_fd,int set_time,Task task);//指定由哪一个定时器模块管理，指定定时时间，指定定时事件
        void setTimerfd(int timerfd, int expiration);
private:
	int timerfd;
	Timestamp timestamp;
};

#endif
