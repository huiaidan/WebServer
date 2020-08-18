#include"TimerQueue.h"
class TcpConnection;
using namespace std;
void TimerQueue::push(pair<int64_t,Task> mp)
{
	MutexLockGuard lock(t_lock);
	timer_map.insert(mp);
}

void TimerQueue::pop(int64_t time)
{
	MutexLockGuard lock(t_lock);
	timer_map.erase(time);
}

void TimerQueue::clear()
{
	MutexLockGuard lock(t_lock);
	timer_map.clear();
}

void TimerQueue::get_event()
{
	multimap<int64_t,Task>::iterator i;
	multimap<int64_t,Task>::iterator iter_end=timer_map.upper_bound(Timestamp().now());
	MutexLockGuard lock(t_lock);
	for (i = timer_map.begin(); i != iter_end; ++i)
	{
		timer_vec.push_back((*i).second);
	}
	timer_map.erase(timer_map.begin(),iter_end);
	do_event();
}

void TimerQueue::do_event()
{
	vector<Task> task;
	int len=timer_vec.size();
	task.swap(timer_vec);
	for(int i=0;i<len;i++)
	{
		task[i]();
	}
}
