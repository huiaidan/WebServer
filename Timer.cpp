#include<string.h>
#include"Timer.h"
#include"Base/Data.h"
#include"error.h"
#include"TcpConnection.h"
void Timer::timer_init_s(shared_ptr<TcpConnection> conn,int timer_fd,int set_time,Task task)//指定由哪一个定时器模块管理，指定定时时间，指定定时事件
{
	int64_t time=set_time*STOUS+timestamp.now();
	conn->set_time(time);
	setTimerfd(timer_fd,set_time);
	conn->get_tq()->push(make_pair(time,task));
}

void Timer::timer_init_ms(shared_ptr<TcpConnection> conn,int timer_fd,int set_time,Task task)//指定由哪一个定时器模块管理，指定定时时间，指定定时事件
{
	int64_t time=set_time*MSTOUS+timestamp.now();
	setTimerfd(timer_fd,time);
	conn->get_tq()->push(make_pair(time,task));
}

void Timer::timer_init_us(shared_ptr<TcpConnection> conn,int timer_fd,int set_time,Task task)//指定由哪一个定时器模块管理，指定定时时间，指定定时事件
{
	int64_t time=set_time*USTOUS+timestamp.now();
	setTimerfd(timer_fd,time);
	conn->get_tq()->push(make_pair(time,task));
}

void Timer::setTimerfd(int timerfd, int expiration)
{
	  struct itimerspec newValue;
	  struct itimerspec oldValue;
	  bzero(&newValue, sizeof newValue);
	  bzero(&oldValue, sizeof oldValue);
	  newValue.it_value.tv_sec = expiration;
	  newValue.it_value.tv_nsec = 0;
	  int ret = timerfd_settime(timerfd, 0, &newValue, &oldValue);
	  if (ret==-1)
	  {
		  perr_exit("setTimerfd error");
	  }
}
