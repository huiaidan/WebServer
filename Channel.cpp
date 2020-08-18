#include"Channel.h"
#include<unistd.h>
#include<ctype.h>
#include"Base/Data.h"
#include<iostream>
#include<algorithm>
#include"HttpRequest.h"
#include<fcntl.h>
#include"TcpConnection.h"
#include"EventLoop.h"
using namespace std;
char buf[MAXSIZE];
MutexLock Channel::channel_mutex;
Channel::Channel(EventLoop* loop_):
	loop(loop_),
	nready(0),
	ep(new Epoll()),
	fd_mutex()
	{}
//初始化listenfd对应的epoll_event结构体
void Channel::lfd_event_init(shared_ptr<TcpConnection> conn)
{
	this->fd_event.events=EPOLLIN;
	weak_ptr<TcpConnection> conn_=conn;
        connmap.insert(make_pair(conn->get_fd(),conn_));
	this->fd_event.data.fd=conn->get_fd();
	this->ep->epfd_add(conn->get_fd(),&this->fd_event);
}

//初始化timerfd对应的epoll_event结构体
void Channel::tfd_event_init(shared_ptr<TcpConnection> conn)
{
	this->fd_event.events=EPOLLIN|EPOLLET;
	weak_ptr<TcpConnection> conn_=conn;
        connmap.insert(make_pair(conn->get_fd(),conn_));
	this->fd_event.data.fd=conn->get_fd();
        this->ep->epfd_add(conn->get_fd(),&this->fd_event);
}

//初始化newfd对应的epoll_event结构体（读）
void Channel::newfd_revent_init(shared_ptr<TcpConnection> conn,struct epoll_event& newfd_event)
{
	weak_ptr<TcpConnection> conn_=conn;
        connmap.insert(make_pair(conn->get_fd(),conn_));
	newfd_event.events=EPOLLIN|EPOLLET;
	this->fd_event.data.fd=conn->get_fd();
}

//初始化newfd对应的epoll_event结构体（写）
void Channel::newfd_wevent_init(shared_ptr<TcpConnection> conn,struct epoll_event& newfd_event)
{
	newfd_event.events=EPOLLOUT|EPOLLET;
	this->fd_event.data.fd=conn->get_fd();
}

//设置IO属性（非阻塞）并初始化（读）
void Channel::revent_init(shared_ptr<TcpConnection> conn)
{
	int flag=fcntl(conn->get_fd(),F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(conn->get_fd(),F_SETFL,flag);
	newfd_revent_init(conn,this->fd_event);
	if(conn->get_stat()==0)
	{
		conn->set_stat(1);
		this->ep->epfd_add(conn->get_fd(),&this->fd_event);
	}
	else
		this->ep->epfd_mod(conn->get_fd(),&this->fd_event);
	conn->set_timer(this);
}

//设置IO属性（非阻塞）并初始化（写）
void Channel::wevent_init(shared_ptr<TcpConnection> conn)
{
	newfd_wevent_init(conn,this->fd_event);
	if(conn->get_stat()==0)
	{
		conn->set_stat(1);
		this->ep->epfd_add(conn->get_fd(),&this->fd_event);
	}
	else
		this->ep->epfd_mod(conn->get_fd(),&this->fd_event);
}

//获取激活的事件，判断事件类型并分别处理
void Channel::fd_event_distribution(struct epoll_event* event)
{
	for(int i=0;i<nready;i++){
                if(this->event[i].events==EPOLLIN){
			shared_ptr<TcpConnection> conn=connmap[event[i].data.fd].lock();
			if(conn.get()==NULL)
			{
				MutexLockGuard lock(channel_mutex);
				connmap_erase(event[i].data.fd);
				continue;
			}
			if(conn->get_fd()==loop->get_wakeupfd())
			{
				uint64_t one;
				int n=0;
				while((n=read(conn->get_fd(),&one,sizeof(one)))>0)
						{
							if(n==-1)
								perr_exit("read wakeupfd error");
						}
				continue;
			}
			conn->myread();
		}		
		else{
	                channel_mutex.lock();
			shared_ptr<TcpConnection> conn=connmap[event[i].data.fd].lock();
			if(conn==NULL)
			{
				connmap.erase(event[i].data.fd);
				continue;
			}
			conn->mywrite();
	                channel_mutex.unlock();
		}
	}
}

void Channel::do_event_array()
{
	this->ep->do_epoll_wait(this->nready,this->event);
	fd_event_distribution(this->event);
}

void Channel::connmap_erase(int fd)
{
	connmap.erase(fd);
}
