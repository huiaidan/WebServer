#ifndef _TIMERQUEUE_H
#define _TIMERQUEUE_H

#include<map>
#include<algorithm>
#include<vector>
#include"Timestamp.h"
#include"MutexLock.h"
#include<boost/function.hpp>
typedef boost::function<void()> Task; 
class TcpConnection;
class TimerQueue
{
public:
	TimerQueue():timer_map(),t_lock()
	{}
	~TimerQueue()
	{}
	void push(pair<int64_t,Task>);
	void pop(int64_t);
	void clear();
	void get_event();
        void do_event();
private:
	multimap<int64_t,Task> timer_map;
	vector<Task> timer_vec;
	MutexLock t_lock;
};

#endif
