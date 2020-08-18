#ifndef _CHANNEL_H
#define _CHANNEL_H
#include<iostream>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include"Epoll.h"
#include"Base/Data.h"
#include"Base/MutexLock.h"
#include<map>
#include<memory>
using namespace std;
class TcpConnection;
class EventLoop;
class Channel
{
public:
	Channel(EventLoop* loop_);
	~Channel()
	{}
	void lfd_event_init(shared_ptr<TcpConnection>);//初始化listenfd的epoll_event结构
        void tfd_event_init(shared_ptr<TcpConnection>);
	void newfd_revent_init(shared_ptr<TcpConnection>,struct epoll_event&);//......newfd.....epoll_event....
	void newfd_wevent_init(shared_ptr<TcpConnection>,struct epoll_event&);//......newfd.....epoll_event....
	void revent_init(shared_ptr<TcpConnection>);
	void wevent_init(shared_ptr<TcpConnection>);
	void fd_event_distribution(struct epoll_event* event);//将listenfd的事件给Acceptor,将newfd的事件放入线程池的任务队列中；
	void do_event_array();
        void connmap_erase(int fd);
	Epoll* get_ep()
	{
		return ep;
	}
	static MutexLock channel_mutex;
private:
	EventLoop* loop;
	struct epoll_event fd_event;
	struct epoll_event event[MAXSIZE];
	int nready;
	Epoll* ep;
	MutexLock fd_mutex;
	map<int,weak_ptr<TcpConnection>> connmap;
};
#endif
